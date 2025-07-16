#include <QApplication>
#include "VtkWidgetWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VtkWidgetWindow window;

    window.resize(800, 600);
    window.setWindowTitle("VTK + QVTKOpenGLNativeWidget 示例");
    window.show();



    return a.exec();
}
