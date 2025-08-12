#include "OcclusionWidget.h"
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>

#include <QDebug>

OcclusionWidget::OcclusionWidget (QWidget *parent)
    : QVTKOpenGLNativeWidget (parent)
{
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderWindow()->AddRenderer (m_renderer);
    this->setMinimumSize (400, 400);

    m_upperActor = vtkSmartPointer<vtkActor>::New();
    m_lowerActor = vtkSmartPointer<vtkActor>::New();

    m_renderer->AddActor (m_upperActor);
    m_renderer->AddActor (m_lowerActor);

    resetCamera();

    markInitialTip();
}

void OcclusionWidget::loadUpperModel (const QString &filePath)
{
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (filePath.toUtf8().constData());
    reader->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (reader->GetOutputPort());
    m_upperActor->SetMapper (mapper);
    m_upperActor->GetProperty()->SetColor (1, 1, 1);
}

void OcclusionWidget::loadLowerModel (const QString &filePath)
{
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (filePath.toUtf8().constData());
    reader->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (reader->GetOutputPort());
    m_lowerActor->SetMapper (mapper);
    m_lowerActor->GetProperty()->SetColor (1, 0.8, 0.8);

    // 默认 tip 点为下颌中心
    m_tipPoint[0] = 0;
    m_tipPoint[1] = 0;
    m_tipPoint[2] = 0;
    m_tipNormal[0] = 0;
    m_tipNormal[1] = 0;
    m_tipNormal[2] = -1;

    resetCamera();
}

void OcclusionWidget::updateFromData (const QJsonObject &data)
{
    qDebug() << "驱动咬合动作";

    if (!data.contains ("success") || !data["success"].toBool())
        return;

    QJsonArray centroid = data["centroid"].toArray();
    QJsonArray norm = data["norm"].toArray();

    if (centroid.size() != 3 || norm.size() != 3)
        return;

    // 单位转换：centroid 从米 -> 毫米
    constexpr double metersToMillimeters = 1000.0;

    for (int i = 0; i < 3; ++i)
    {
        m_tipPoint[i] = centroid[i].toDouble() * metersToMillimeters;
        m_tipNormal[i] = norm[i].toDouble(); // 单位向量，无需转换
    }

    applyLowerJawTransform (1.0); // 默认向法向方向后退 1 毫米
    this->renderWindow()->Render();
}

void OcclusionWidget::applyLowerJawTransform (double offsetMm)
{
    // 实时目标牙尖位置（含法向偏移）
    double currentTarget[3];
    for (int i = 0; i < 3; ++i)
    {
        currentTarget[i] = m_tipPoint[i] - offsetMm * m_tipNormal[i];
    }

    // 计算从初始牙尖到目标牙尖的偏移
    double delta[3];
    for (int i = 0; i < 3; ++i)
    {
        delta[i] = currentTarget[i] - m_initialLowerTipPosition[i];
    }

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    transform->Translate (delta);

    m_lowerActor->SetUserTransform (transform);
}

void OcclusionWidget::markInitialTip()
{
    // 你已经把模型整体平移，使牙尖居中为原点，所以直接设为 0
    m_initialLowerTipPosition[0] = 0;
    m_initialLowerTipPosition[1] = 0;
    m_initialLowerTipPosition[2] = 0;
}


void OcclusionWidget::resetCamera()
{
    m_renderer->ResetCamera();
    m_renderer->ResetCameraClippingRange();
    this->renderWindow()->Render();
}
