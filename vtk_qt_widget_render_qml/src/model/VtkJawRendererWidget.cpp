#include "VtkJawRendererWidget.h"
#include <QTimer>
#include <QDebug>

// VTK 9.3.0 必需组件
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkOBBTree.h>
#include <vtkTransformPolyDataFilter.h>

VtkJawRendererWidget::VtkJawRendererWidget (QWidget* parent)
    : QVTKOpenGLNativeWidget (parent)
{
    // 初始化渲染系统
    initializeRenderSystem();

    // 配置动画定时器 (60fps)
    m_animationTimer = new QTimer (this);
    connect (m_animationTimer, &QTimer::timeout,
             this, &VtkJawRendererWidget::updateBiteAnimation);
}

void VtkJawRendererWidget::initializeRenderSystem()
{
    // 1. 创建并设置正确的渲染窗口
    m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->setRenderWindow (m_renderWindow);

    // 2. 配置渲染参数
    m_renderWindow->SetMultiSamples (0); // 禁用多重采样
    this->setEnableHiDPI (true);         // 支持高DPI显示

    // 3. 初始化渲染器
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground (0.9, 0.9, 0.9); // 浅灰背景
    m_renderWindow->AddRenderer (m_renderer);

    // 4. 设置交互器
    vtkNew<vtkRenderWindowInteractor> iren;
    m_renderWindow->SetInteractor (iren);
    iren->SetInteractorStyle (vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());
    iren->Initialize();
}

bool VtkJawRendererWidget::loadUpperJaw (const QString& filePath)
{
    // 清理现有模型
    if (m_upperJawActor)
    {
        m_renderer->RemoveActor (m_upperJawActor);
    }

    // 加载STL文件
    vtkNew<vtkSTLReader> reader;
    reader->SetFileName (filePath.toUtf8().constData());
    reader->Update();

    if (!reader->GetOutput() || reader->GetOutput()->GetNumberOfPoints() == 0)
    {
        emit errorOccurred (tr ("Failed to load upper jaw STL"));
        return false;
    }

    // 创建可视化管线
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection (reader->GetOutputPort());

    m_upperJawActor = vtkSmartPointer<vtkActor>::New();
    m_upperJawActor->SetMapper (mapper);
    m_upperJawActor->GetProperty()->SetColor (0.8, 0.6, 0.6); // 浅红色
    m_upperJawActor->GetProperty()->SetOpacity (0.8);        // 80%透明度

    m_renderer->AddActor (m_upperJawActor);
    m_renderer->ResetCamera();
    return true;
}

bool VtkJawRendererWidget::loadLowerJaw (const QString& filePath)
{
    // 清理现有模型
    if (m_lowerJawActor)
    {
        m_renderer->RemoveActor (m_lowerJawActor);
    }

    // 加载STL文件
    vtkNew<vtkSTLReader> reader;
    reader->SetFileName (filePath.toUtf8().constData());
    reader->Update();

    if (!reader->GetOutput() || reader->GetOutput()->GetNumberOfPoints() == 0)
    {
        emit errorOccurred (tr ("Failed to load lower jaw STL"));
        return false;
    }

    // 初始化变换系统
    m_lowerJawTransform = vtkSmartPointer<vtkTransform>::New();
    m_lowerJawTransform->PostMultiply();

    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetTransform (m_lowerJawTransform);
    transformFilter->SetInputConnection (reader->GetOutputPort());

    // 创建可视化管线
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection (transformFilter->GetOutputPort());

    m_lowerJawActor = vtkSmartPointer<vtkActor>::New();
    m_lowerJawActor->SetMapper (mapper);
    m_lowerJawActor->GetProperty()->SetColor (0.6, 0.6, 0.8); // 浅蓝色
    m_lowerJawActor->GetProperty()->SetOpacity (0.8);

    m_renderer->AddActor (m_lowerJawActor);
    autoAlignJaws();
    return true;
}

void VtkJawRendererWidget::autoAlignJaws()
{
    if (!m_upperJawActor || !m_lowerJawActor) return;

    m_lowerJawTransform->Identity();

    // 获取包围盒
    double upperBounds[6], lowerBounds[6];
    m_upperJawActor->GetBounds (upperBounds);
    m_lowerJawActor->GetMapper()->GetInput()->GetBounds (lowerBounds);

    // 计算校准参数
    double verticalOffset = upperBounds[2] - lowerBounds[3] + 5.0; // 5mm间隙
    double horizontalOffset = (upperBounds[0] + upperBounds[1]) / 2 -
                              (lowerBounds[0] + lowerBounds[1]) / 2;

    // 应用变换
    m_lowerJawTransform->Translate (horizontalOffset, verticalOffset, 0);
    m_renderWindow->Render();
}

void VtkJawRendererWidget::startBiteSimulation()
{
    if (m_animationTimer->isActive()) return;

    m_biteProgress = 0.0f;
    m_currentRotation = 0.0f;
    m_animationTimer->start (16); // ~60fps
    emit simulationStarted();
}

void VtkJawRendererWidget::stopSimulation()
{
    m_animationTimer->stop();
    emit simulationFinished();
}

void VtkJawRendererWidget::updateBiteAnimation()
{
    if (m_biteProgress >= 1.0f)
    {
        stopSimulation();
        return;
    }

    // 更新进度
    m_biteProgress += 0.01f;
    m_biteProgress = qMin (m_biteProgress, 1.0f);
    m_currentRotation = 30.0f * m_biteProgress;

    // 应用变换
    m_lowerJawTransform->Identity();
    m_lowerJawTransform->RotateX (-m_currentRotation); // 绕X轴旋转
    m_lowerJawTransform->Translate (0, -3.0 * m_biteProgress, 0); // 垂直移动

    m_renderWindow->Render();
    emit biteProgressChanged (m_biteProgress);
}

void VtkJawRendererWidget::setJawPosition (float x, float y, float z, float rotation)
{
    if (!m_lowerJawTransform) return;

    m_lowerJawTransform->Identity();
    m_lowerJawTransform->RotateX (-rotation);
    m_lowerJawTransform->Translate (x, y, z);

    m_biteProgress = rotation / 30.0f;
    m_currentRotation = rotation;

    m_renderWindow->Render();
    emit biteProgressChanged (m_biteProgress);
}

void VtkJawRendererWidget::resetPosition()
{
    if (!m_lowerJawTransform) return;

    m_lowerJawTransform->Identity();
    autoAlignJaws();

    m_biteProgress = 0.0f;
    m_currentRotation = 0.0f;
    m_renderWindow->Render();
    emit biteProgressChanged (0.0f);
}

VtkJawRendererWidget::~VtkJawRendererWidget()
{
    stopSimulation();
}
