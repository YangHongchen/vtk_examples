#include "CustomWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

// VTK
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

VtkRenderWidget::VtkRenderWidget(QWidget *parent) : QWidget(parent),
    m_vtkWidget(new QVTKOpenGLNativeWidget(this))
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_vtkWidget);

    // 添加返回按钮（可选）
    QPushButton* backBtn = new QPushButton("返回QML页面", this);
    connect(backBtn, &QPushButton::clicked, this, &VtkRenderWidget::requestBack);
    layout->addWidget(backBtn);

    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void VtkRenderWidget::activate()
{
    if(!m_initialized) {
        setupPipeline();
        m_initialized = true;
    }
    resetCamera();
}

void VtkRenderWidget::setupPipeline()
{
    // 创建立方体
    vtkNew<vtkCubeSource> cube;
    cube->SetXLength(2.0);
    cube->SetYLength(1.5);
    cube->SetZLength(1.0);

    // 创建映射器和演员
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cube->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.2, 0.6, 0.9);  // 蓝色立方体
    actor->GetProperty()->EdgeVisibilityOn();  // 显示边线

    // 设置渲染器
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->AddActor(actor);
    m_renderer->SetBackground(0.1, 0.2, 0.3);  // 深蓝背景

    // 配置渲染窗口
    vtkNew<vtkGenericOpenGLRenderWindow> window;
    window->AddRenderer(m_renderer);
    m_vtkWidget->setRenderWindow(window);
}

void VtkRenderWidget::resetCamera()
{
    if(m_renderer) {
        m_renderer->ResetCamera();
        m_vtkWidget->renderWindow()->Render();
    }
}

void VtkRenderWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    activate();  // 显示时自动激活
}

void VtkRenderWidget::hideEvent(QHideEvent* event)
{
    // 可选：隐藏时释放资源
    QWidget::hideEvent(event);
}
