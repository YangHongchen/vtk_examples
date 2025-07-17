#include "MainWindow.h"
#include <QPushButton>
#include <QQuickItem>
#include <QLabel>
#include "CustomWidget.h"
#include <QQmlContext>

// MainWindow.cpp
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_patientController = new PatientController(this);

    setupUI();

}

void MainWindow::handlePageChange(int index)
{
    qDebug() << "收到页面切换信号，索引:" << index;
    m_contentStack->setCurrentIndex(index);
    // 双重验证
    if (m_contentStack->currentIndex() != index) {
        qWarning() << "页面切换失败！当前索引:" << m_contentStack->currentIndex();
    }
}

void MainWindow::addQmlPage(const QString &qmlPath)
{
    QQuickWidget *qmlWidget = new QQuickWidget();
    if(m_patientController) {
         qmlWidget->rootContext()->setContextProperty("patientController", m_patientController);  // ✅ 先设置 context
    } else {
        qCritical() << "Pateint 服务初始化失败！";
    }

    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    qmlWidget->setSource(QUrl(qmlPath));
    m_contentStack->addWidget(qmlWidget);
}

void MainWindow::setupUI()
{
    qDebug() << 111;
    // 主布局
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
   qDebug() << 222;
    // 1. 左侧导航 (QML实现)
    m_navWidget = new QQuickWidget(this);
   qDebug() << 333;
    // 设置页面
    m_navWidget->setSource(QUrl("qrc:/qml/NavigationMenu.qml"));
    m_navWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    QObject *navQmlRoot = m_navWidget->rootObject();
    if(!navQmlRoot) {
        qCritical() << "QML根对象未创建！";
        return;
    }
       qDebug() << 555;
    bool connectResult = QObject::connect(navQmlRoot, SIGNAL(pageChanged(int)),this, SLOT(handlePageChange(int)));
    qDebug() << "信号连接结果:" << connectResult; // 必须输出true
     qDebug() << 666;
    // 2. 右侧内容区 (Widget实现： 这里的加载顺序，要和导航qml组件定义的pageIndex的顺序一致）
    m_contentStack = new QStackedWidget();
         qDebug() << 777;
    addQmlPage("qrc:/qml/patient/PatientIndex.qml");
    addQmlPage("qrc:/qml/detection/DetectionIndex.qml");
    addQmlPage("qrc:/qml/analysis/AnalysisModel.qml");
    addQmlPage("qrc:/qml/analysis/AnalysisTracks.qml");
    addQmlPage("qrc:/qml/analysis/AnalysisAnalyze.qml");


     qDebug() << 888;
    // 3. 添加自定义Widget页面（索引3）
    m_vtkWidget = new VtkRenderWidget();


    qDebug() << 999;
    m_contentStack->addWidget(m_vtkWidget);  // 索引3
 qDebug() << 000;
    // 3.布局设置（导航站1/5，右侧空间内容占比4/5）
    mainLayout->addWidget(m_navWidget, 1);
 qDebug() << 101;
    mainLayout->addWidget(m_contentStack, 4);
    setCentralWidget(centralWidget);
    qDebug() << 102;

}

void MainWindow::refreshUI()
{

}

void MainWindow::setupCpps()
{
    qDebug()<< "初始化类:";
    m_patientController = new PatientController;
    m_navWidget->rootContext()->setContextProperty("patientController", m_patientController);
}


