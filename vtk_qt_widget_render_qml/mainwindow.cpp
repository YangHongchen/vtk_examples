#include "mainwindow.h"
#include <QPushButton>
#include <QQuickItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     m_stackedWidget(new QStackedWidget(this))
{
    // 主布局
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 1. 添加导航栏 (左侧)
    QVBoxLayout *navLayout = new QVBoxLayout();
    QStringList pageNames = {"仪表盘", "设置", "数据分析"};

    for (int i = 0; i < pageNames.size();  ++i) {
        QPushButton *btn = new QPushButton(pageNames[i], this);
        connect(btn, &QPushButton::clicked, [this, i](){ switchPage(i); });
        navLayout->addWidget(btn);
    }

    // 2. 添加QML页面栈 (右侧)
    m_stackedWidget->setMinimumWidth(800);

    // 加载多个QML页面
    QStringList qmlFiles = {
        "qrc:/qml/Dashboard.qml",
        "qrc:/qml/Settings.qml",
        "qrc:/qml/Analysis.qml"
    };

    foreach (const QString &file, qmlFiles) {
        QQuickWidget *qmlWidget = new QQuickWidget();
        qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        qmlWidget->setSource(QUrl(file));
        m_qmlPages.append(qmlWidget);
        m_stackedWidget->addWidget(qmlWidget);
    }

    // 组合布局
    mainLayout->addLayout(navLayout, 1);
    mainLayout->addWidget(m_stackedWidget, 4);
    setCentralWidget(centralWidget);

    // 初始化显示第一页
    switchPage(0);
}

MainWindow::~MainWindow()
{
    if(m_stackedWidget) {
        delete m_stackedWidget;
    }
    m_qmlPages.clear();
}

void MainWindow::switchPage(int index)
{
    if (index >= 0 && index < m_qmlPages.size())  {
        m_stackedWidget->setCurrentIndex(index);

        // 强制刷新QML页面
        QQuickWidget *current = m_qmlPages[index];
        current->rootObject()->setProperty("active", true);
    }
}
