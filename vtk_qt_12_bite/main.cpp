#include <QApplication>
#include <QPushButton>

#include "MainWindow.h"

int main (int argc, char *argv[])
{
    QApplication a (argc, argv);

    MainWindow w;
    w.setWindowTitle ("VTK 模拟真人咬合动画");
    w.resize (900, 600);
    w.show();

    return a.exec();
}
