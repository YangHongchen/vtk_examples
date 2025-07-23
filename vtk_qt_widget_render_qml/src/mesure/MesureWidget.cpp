#include "MesureWidget.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <QObject>
#include <QQuickItem>

MesureWidget::MesureWidget (QWidget* parent) : QWidget (parent)
{
    m_patientModel = PatientModel::instance();
    m_patient = m_patientModel->currentPatient();
    connect (m_patientModel, &PatientModel::currentPatientChanged, this, &MesureWidget::onCurrentPatientChanged);

    // 主布局 (水平分隔)
    QHBoxLayout* mainLayout = new QHBoxLayout (this);
    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->setSpacing (0);

    // 1. VTK 渲染区 (75%)
    QWidget* vtkContainer = new QWidget (this);
    QVBoxLayout* vtkLayout = new QVBoxLayout (vtkContainer);
    vtkLayout->setContentsMargins (0, 0, 0, 0);

    m_vtkWidget = new QVTKOpenGLNativeWidget (vtkContainer);

    vtkLayout->addWidget (m_vtkWidget);

    mainLayout->addWidget (vtkContainer, 3); // 3:1 比例

    // 2. QML 控制区 (30%)
    m_qmlControl = new QQuickWidget (this);
    m_qmlControl->setResizeMode (QQuickWidget::SizeRootObjectToView);
    m_qmlControl->setSource (QUrl ("qrc:/qml/mesure/MesureControl.qml"));
    mainLayout->addWidget (m_qmlControl, 1);

    // 初始化
    setupVTK();

    // 初始化测量步骤
    setupSteps();

    // 初始化
    setupPatientWidget();

    setupConnections();
}

MesureWidget::~MesureWidget()
{

}

void MesureWidget::setupVTK()
{
    // 创建VTK渲染窗口
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    m_vtkWidget->setRenderWindow (renderWindow);

    // 初始化渲染器
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground (0.2, 0.3, 0.4);
    renderWindow->AddRenderer (m_renderer);

    // 添加测试几何体
    vtkNew<vtkSphereSource> sphere;
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection (sphere->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper (mapper);
    m_renderer->AddActor (actor);
    m_renderer->ResetCamera();
}

void MesureWidget::setupQML()
{

}

void MesureWidget::setupConnections()
{

}

void MesureWidget::setupSteps()
{
    m_stepsOverlay =  new QQuickWidget (this);
    m_stepsOverlay->setResizeMode (QQuickWidget::SizeRootObjectToView);
    m_stepsOverlay->setSource (QUrl ("qrc:/qml/mesure/MesureSteps.qml"));
    // 样式设置
    m_stepsOverlay->setClearColor (Qt::transparent);
    m_stepsOverlay->setAttribute (Qt::WA_AlwaysStackOnTop);
    m_stepsOverlay->setAttribute (Qt::WA_TranslucentBackground);
    // 精确定位 (左侧16px，顶部76px)
    m_stepsOverlay->setFixedSize (200, 300); // 根据实际内容调整
    m_stepsOverlay->move (16, 76);
    // 事件穿透设置
    m_stepsOverlay->setWindowFlags (Qt::FramelessWindowHint);
    m_stepsOverlay->setMouseTracking (true);
}

void MesureWidget::setupControl()
{

}

void MesureWidget::setupPatientWidget()
{
    m_patientOverlay =  new QQuickWidget (this);
    m_patientOverlay->setResizeMode (QQuickWidget::SizeRootObjectToView);
    // 先连接信号再设置源
    connect (m_patientOverlay, &QQuickWidget::statusChanged, this, [this] (QQuickWidget::Status status)
    {
        if (status == QQuickWidget::Ready)
        {
            this->updateQmlPatientData();
        }
    });
    m_patientOverlay->setSource (QUrl ("qrc:/qml/_widgets/PatientCurrent.qml"));
    // 样式设置
    m_patientOverlay->setClearColor (Qt::transparent);
    m_patientOverlay->setAttribute (Qt::WA_AlwaysStackOnTop);
    m_patientOverlay->setAttribute (Qt::WA_TranslucentBackground);
    // 精确定位 (左侧16px，顶部76px)
    m_patientOverlay->setFixedSize (320, 40); // 根据实际内容调整
    m_patientOverlay->move (16, 16);
    // 事件穿透设置
    m_patientOverlay->setWindowFlags (Qt::FramelessWindowHint);
    m_patientOverlay->setMouseTracking (true);
}

void MesureWidget::updateQmlPatientData()
{
    if (!m_patientOverlay || !m_patient) return;
    if (auto root = m_patientOverlay->rootObject())
    {
        root->setProperty ("name", m_patient->fullName());
        root->setProperty ("birthDay", m_patient->birthDay());
        root->setProperty ("gender", m_patient->gender());
        root->setProperty ("status", m_patient->status());
    }
}

// ===== 公共接口实现 =====
void MesureWidget::startMeasurement()
{
    if (m_measurementActive) return;

    m_measurementActive = true;
    // 执行测量逻辑...
    m_renderer->GetRenderWindow()->Render();

    emit measurementCompleted (42.0); // 模拟结果
    m_measurementActive = false;
}

void MesureWidget::setMeasurementMode (int mode)
{

}

void MesureWidget::saveMeasurement (const QString &filePath)
{

}

void MesureWidget::handleQmlEvent (const QVariant &data)
{

}

void MesureWidget::onCurrentPatientChanged (PatientObject *patient)
{
    if (m_patient != patient)
    {
        m_patient = patient;
        updateQmlPatientData();
    }
}

