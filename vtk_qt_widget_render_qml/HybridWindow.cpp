#include "HybridWindow.h"
#include <QVBoxLayout>


HybridWindow::HybridWindow(QWidget *parent): QMainWindow(parent)
{
    // 创建分割布局
    m_splitter = new QSplitter(Qt::Horizontal, this);
    // 1. VTK Widget部分
    m_vtkContainer = new QWidget(this);
    QVBoxLayout *vtkLayout = new QVBoxLayout(m_vtkContainer);


    // 2. QML部分
    m_qmlWidget = new QQuickWidget();
    m_qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_qmlWidget->setSource(QUrl("qrc:/Interface.qml"));



    // 设置布局比例 (VTK占70%，QML占30%)
    // m_splitter->addWidget(m_vtkContainer);
    m_splitter->addWidget(m_qmlWidget);
    m_splitter->setSizes({700, 300});

   setCentralWidget(m_splitter);
}
