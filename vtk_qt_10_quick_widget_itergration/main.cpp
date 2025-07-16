#include <QApplication>
#include <QQmlApplicationEngine>
#include <QPushButton>
#include <QQmlContext>
#include "widgetcontainer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 必须的渲染设置
    app.setAttribute(Qt::AA_UseSoftwareOpenGL);
    app.setAttribute(Qt::AA_ShareOpenGLContexts);

    qmlRegisterType<WidgetContainer>("com.dyt",  1, 0, "WidgetContainer");

    QQmlApplicationEngine engine;

    // 创建示例Widget
    QPushButton *button = new QPushButton("Qt Widget按钮");
    button->resize(200, 50);

    // 通过rootContext设置
    WidgetContainer container;
    container.setWidget(button);
    engine.rootContext()->setContextProperty("widgetContainer",  &container);

    engine.load(QUrl("qrc:/qt_quick_widget_intergration_demo/Main.qml"));

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&container](){
        container.setWidget(nullptr);  // 清理widget
    });

    return app.exec();
}
