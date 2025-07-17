#include "mainwindow.h"

#include <QApplication>
#include "LoginWidget.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{

    // 必须在QApplication之前设置
    qputenv("QSG_RHI_BACKEND", "opengl");  // 强制使用 OpenGL 渲染后端
    QApplication app(argc, argv);

    LoginWidget loginWidget;
    MainWindow mainWindow;

    // 监听登录成功
    QObject::connect(&loginWidget, &LoginWidget::loginSuccess, [&]() {
        loginWidget.hide();  // or loginWidget.close() to destroy it
        // mainWindow.show();
        mainWindow.showMaximized();

    });
    // 监听登录失败
    QObject::connect(&loginWidget, &LoginWidget::loginFail, [&](int code, const QString& errorMessage) {
        QMessageBox::critical(&loginWidget, "Login Failed", errorMessage);
    });

    loginWidget.show();
    return app.exec();
}
