#ifndef VTKWIDGETWINDOW_H
#define VTKWIDGETWINDOW_H

#include <QMainWindow>

class QVTKOpenGLNativeWidget;

class VtkWidgetWindow : public QMainWindow
{
    Q_OBJECT
public:
    VtkWidgetWindow(QWidget *parent = nullptr);
    ~VtkWidgetWindow() override;  // 显式声明析构函数
    void init();
private:

private:
    QVTKOpenGLNativeWidget *vtkWidget = nullptr;
};

#endif // VTKWIDGETWINDOW_H
