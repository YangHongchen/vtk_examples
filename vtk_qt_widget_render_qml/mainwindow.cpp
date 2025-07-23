#include "MainWindow.h"
#include <QPushButton>
#include <QQuickItem>
#include <QQuickView>
#include <QLabel>
#include <QQmlContext>

#include "src/mesure/MesureWidget.h"

// 引入事件总线
#include "src/common/EventBus.h"

// MainWindow.cpp
MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
    m_patientController = PatientController::instance();
    m_patientModel = PatientModel::instance();
    m_patientFileTransferManager = PatientFileTransferManager::instance();
    m_mesureRecordController = MesureRecordController::instance();
    m_mesureRecordModel = MesureRecordModel::instance();

    // 检测界面（Widget）
    m_mesureWidget = new MesureWidget (this);

    setupUI();
}

void MainWindow::handlePageChange (int index)
{
    qDebug() << "收到页面切换信号，索引:" << index;
    m_contentStack->setCurrentIndex (index);
    // 双重验证
    if (m_contentStack->currentIndex() != index)
    {
        qWarning() << "页面切换失败！当前索引:" << m_contentStack->currentIndex();
    }
}

void MainWindow::addQmlPage (const QString &qmlPath)
{
    qDebug() << "正在添加 QML 页面:" << qmlPath;

    // 创建 QQuickView（比 QQuickWidget 更稳定，不会重复加载）
    QQuickView *view = new QQuickView();

    // 设置上下文属性（必须在 setSource 之前）
    auto *viewContext = view->rootContext();

    // 事件总线 -----
    viewContext->setContextProperty ("EventBus", &EventBus::instance());

    if (m_patientFileTransferManager)
        viewContext->setContextProperty ("PatientFileTransferManager", m_patientFileTransferManager);

    // 病例管理 -----
    if (m_patientController)
        viewContext->setContextProperty ("PatientController", m_patientController);
    if (m_patientModel)
        viewContext->setContextProperty ("PatientModel", m_patientModel);

    // 检测记录 -----
    if (m_mesureRecordController)
        viewContext->setContextProperty ("MesureRecordController", m_mesureRecordController);
    if (m_mesureRecordModel)
        viewContext->setContextProperty ("MesureRecordModel", m_mesureRecordModel);

    // 加载 QML 页面
    view->setSource (QUrl (qmlPath));
    view->setResizeMode (QQuickView::SizeRootObjectToView);

    // 将 QQuickView 封装为 QWidget 并加入页面堆栈
    QWidget *container = QWidget::createWindowContainer (view, this);
    container->setMinimumSize (800, 600); // 可根据实际需求调整
    container->setFocusPolicy (Qt::StrongFocus); // 支持键盘焦点等

    m_contentStack->addWidget (container);
}

void MainWindow::setupUI()
{
    qDebug() << "setup ui-------------------------------- ";
    // 0.主布局
    QWidget *centralWidget = new QWidget (this);
    QHBoxLayout *mainLayout = new QHBoxLayout (centralWidget);

    // 1. 左侧导航 (QML实现)
    m_navWidget = new QQuickWidget (this);
    // 事件总线注入到导航页面
    m_navWidget->rootContext()->setContextProperty ("EventBus", &EventBus::instance());

    m_navWidget->setSource (QUrl ("qrc:/qml/_widgets/SideMenu.qml"));
    m_navWidget->setResizeMode (QQuickWidget::SizeRootObjectToView);
    QObject *navQmlRoot = m_navWidget->rootObject();
    if (!navQmlRoot)
    {
        qCritical() << "QML根对象未创建！";
        return;
    }
    bool connectResult = QObject::connect (navQmlRoot, SIGNAL (pageChanged (int)), this, SLOT (handlePageChange (int)));

    // 2. 右侧内容区 (Widget实现： 这里的加载顺序，要和导航qml组件定义的pageIndex的顺序一致）
    m_contentStack = new QStackedWidget();
    addQmlPage ("qrc:/qml/patient/PatientIndex.qml");
    // addQmlPage ("qrc:/qml/mesure/MesureIndex.qml");

    m_contentStack->addWidget (m_mesureWidget);

    addQmlPage ("qrc:/qml/analysis/AnalysisModel.qml");
    addQmlPage ("qrc:/qml/analysis/AnalysisTracks.qml");
    addQmlPage ("qrc:/qml/analysis/AnalysisAnalyze.qml");

    // 3. 添加自定义Widget页面（索引3）
    // m_vtkWidget = new VtkRenderWidget();
    // m_contentStack->addWidget (m_vtkWidget);

    // 4.布局设置（导航站1/5，右侧空间内容占比4/5）
    mainLayout->addWidget (m_navWidget, 1);
    mainLayout->addWidget (m_contentStack, 6);
    setCentralWidget (centralWidget);
}

void MainWindow::refreshUI()
{

}

void MainWindow::setupCpps()
{
    qDebug() << "初始化类:";
    // m_navWidget->rootContext()->setContextProperty("patientController", m_patientController);
}


