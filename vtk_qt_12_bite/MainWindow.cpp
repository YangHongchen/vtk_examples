#include "MainWindow.h"

MainWindow::MainWindow (QWidget *parent): QMainWindow{parent}
{

    // 创建一个中心部件，并设置布局
    QWidget *central = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout (central);

    m_vtkWidget = new VTKOcclusionSimulation (this);
    layout->addWidget (m_vtkWidget);

    setCentralWidget (central);

}
