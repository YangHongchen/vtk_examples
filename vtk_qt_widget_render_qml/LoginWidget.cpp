#include "LoginWidget.h"

#include <QQuickWidget>
#include <QVBoxLayout>
#include <QQmlContext>
#include <QQuickItem>


LoginWidget::LoginWidget(QWidget *parent): QWidget{parent}
{
    // 要这样写（widget要继承当前类） 否则无法接收到qml发出的信号
    m_quickWidget = new QQuickWidget(QUrl("qrc:/qml/LoginPage.qml"), this);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_quickWidget->setContentsMargins(0,0,0,0);

    QObject *loginQmlRoot = m_quickWidget->rootObject();
    if(!loginQmlRoot) {
        qCritical() << "QML根对象未创建！";
        return;
    }
    bool connectResult = QObject::connect(loginQmlRoot, SIGNAL(loginRequest(QString, QString)), this, SLOT(handleLoginRequest(QString, QString)));
    qDebug() << "信号连接登录QML结果:" << connectResult; // 必须输出true

    bool connectResult3 = QObject::connect(loginQmlRoot, SIGNAL(loginCanceled(int)), this, SLOT(handleLoginCanceled(int)));
    qDebug() << "信号连接登录取消QML结果:" << connectResult3; // 必须输出true

    // 布局管理
    auto *layout = new QVBoxLayout;
    layout->addWidget(m_quickWidget);
    setLayout(layout);

    // 初始化窗口
    setWindowTitle("登录");
    resize(1024,640);
    setContentsMargins(0,0,0,0);
    setStyleSheet("margin: 0px; padding: 0px;");

}

void LoginWidget::handleLoginRequest(QString username, QString password)
{
    qDebug() << "用户登录请求：username="<<username<<", password="<< password;

    // DEBUG: 模拟数据库中的正确的用户名 & 密码
    const QString origin_username = "admin";
    const QString origin_password = "123456";
    // 校验用户名密码
    if(username.isEmpty() || password.isEmpty())
    {
        qDebug() << "用户或密码不能为空";
        emit loginFail(500, "用户或密码不能为空");
        return;
    }
    // 比对用户名和密码
    if(username == origin_username && password == origin_password )
    {
        qDebug() << "登录成功！";
        emit loginSuccess();
    }
    else
    {
        qDebug() << "用户名或密码错误";
        emit loginFail(500, "用户名或密码错误");
    }
}

void LoginWidget::handleLoginCanceled(int code)
{
    qDebug() << "接受到用户登录取消：code="<<code;
}

void LoginWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Set window flags to only show minimize and close buttons
    if (windowHandle()) {
        windowHandle()->setFlags(
            Qt::Window |                    // It's a window
            Qt::WindowTitleHint |           // Has a title bar
            Qt::WindowSystemMenuHint |      // Has system menu
            Qt::WindowMinimizeButtonHint |  // Show minimize button
            Qt::WindowCloseButtonHint       // Show close button
            );
    }
}
