#include "MainWindow.h"

#include <QPushButton>

MainWindow::MainWindow (QWidget *parent): QMainWindow{parent}
{

    // 创建一个中心部件，并设置布局
    auto *central = new QWidget (this);
    setCentralWidget (central);

    auto *mainLayout = new QVBoxLayout (central);
    auto *buttonLayout = new QHBoxLayout();

    // 加入相机，控制头骨的视角
    QPushButton *btnFront = new QPushButton ("前视图");
    QPushButton *btnBack = new QPushButton ("后视图");
    QPushButton *btnLeft = new QPushButton ("左视图");
    QPushButton *btnRight = new QPushButton ("右视图");
    QPushButton *btnTop = new QPushButton ("顶部视图");
    QPushButton *btnBottom = new QPushButton ("底部视图");

    buttonLayout->addWidget (btnFront);
    buttonLayout->addWidget (btnBack);
    buttonLayout->addWidget (btnLeft);
    buttonLayout->addWidget (btnRight);
    buttonLayout->addWidget (btnTop);
    buttonLayout->addWidget (btnBottom);

    mainLayout->addLayout (buttonLayout);

    m_vtkWidget = new VTKOcclusionSimulation (this);
    mainLayout->addWidget (m_vtkWidget);

    // 信号连接
    connect (btnFront, &QPushButton::clicked, this, [ = ]() { m_vtkWidget->setCameraView ("front"); });
    connect (btnBack, &QPushButton::clicked, this, [ = ]() { m_vtkWidget->setCameraView ("back"); });
    connect (btnLeft, &QPushButton::clicked, this, [ = ]() { m_vtkWidget->setCameraView ("left"); });
    connect (btnRight, &QPushButton::clicked, this, [ = ]() { m_vtkWidget->setCameraView ("right"); });
    connect (btnTop, &QPushButton::clicked, this, [ = ]() { m_vtkWidget->setCameraView ("top"); });
    connect (btnBottom, &QPushButton::clicked, this, [ = ]() { m_vtkWidget->setCameraView ("bottom"); });
}
