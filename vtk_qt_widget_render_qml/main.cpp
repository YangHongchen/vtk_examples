#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    // 必须在QApplication之前设置
    qputenv("QSG_RHI_BACKEND", "opengl");  // 强制使用 OpenGL 渲染后端

    QSurfaceFormat format;
    format.setVersion(3,  3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(8);  // 多重采样抗锯齿
    QSurfaceFormat::setDefaultFormat(format);


    QApplication app(argc, argv);


    MainWindow w;
    w.show();


    return app.exec();
}
