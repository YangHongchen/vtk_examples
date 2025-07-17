#include "mainwindow.h"
#include <QPushButton>
#include <QQuickItem>
#include <QLabel>
#include "CustomWidget.h"

// MainWindow.cpp
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // 主布局
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 1. 左侧导航 (QML实现)
    m_navWidget = new QQuickWidget(QUrl("qrc:/qml/NavigationMenu.qml"),this);
    m_navWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    QObject *navQmlRoot = m_navWidget->rootObject();
    if(!navQmlRoot) {
        qCritical() << "QML根对象未创建！";
        return;
    }
    bool connectResult = QObject::connect(navQmlRoot, SIGNAL(pageChanged(int)),this, SLOT(handlePageChange(int)));
    qDebug() << "信号连接结果:" << connectResult; // 必须输出true

    // 2. 右侧内容区 (Widget实现)
    m_contentStack = new QStackedWidget();
    addQmlPage("qrc:/qml/Dashboard.qml");
    addQmlPage("qrc:/qml/Settings.qml");
    addQmlPage("qrc:/qml/Analysis.qml");

    // 3. 添加自定义Widget页面（索引3）
    m_vtkWidget = new VtkRenderWidget();
    m_contentStack->addWidget(m_vtkWidget);  // 索引3

    // 3.布局设置（导航站1/5，右侧空间内容占比4/5）
    mainLayout->addWidget(m_navWidget, 1);
    mainLayout->addWidget(m_contentStack, 4);
    setCentralWidget(centralWidget);
}

void MainWindow::handlePageChange(int index)
{
    if(index == 3) {  // VTK页面特殊处理
        qDebug() << "切换到VTK渲染页面";
        m_vtkWidget->activate();  // 激活VTK渲染
    }
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
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    qmlWidget->setSource(QUrl(qmlPath));
    m_contentStack->addWidget(qmlWidget);
}


