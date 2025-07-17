#include "MainWindow.h"
#include <QApplication>
#include "LoginWidget.h"
#include <QMessageBox>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    // 必须在QApplication之前设置
    // 强制使用 OpenGL 渲染后端
    qputenv("QSG_RHI_BACKEND", "opengl");
    QApplication app(argc, argv);

    // 初始化登录页面
    LoginWidget loginWidget;

    // 初始化主窗口
    MainWindow mainWindow;

    // 监听登录成功
    QObject::connect(&loginWidget, &LoginWidget::loginSuccess, [&]() {
        loginWidget.hide();
        mainWindow.showMaximized();
    });

    // 监听登录失败
    QObject::connect(&loginWidget, &LoginWidget::loginFail, [&](int code, const QString& errorMessage) {
        QMessageBox::critical(&loginWidget, "Login Failed", errorMessage);
    });

    loginWidget.show();

    return app.exec();
}
