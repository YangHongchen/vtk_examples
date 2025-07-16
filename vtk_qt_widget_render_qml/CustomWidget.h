#pragma once

#include <QWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

class VtkRenderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VtkRenderWidget(QWidget *parent = nullptr);
    void activate();  // 激活/刷新渲染
    void resetCamera();  // 重置视角

signals:
    void requestBack();  // 请求返回信号

protected:
    void showEvent(QShowEvent*) override;
    void hideEvent(QHideEvent*) override;

private:
    void setupPipeline();  // 初始化VTK管线

    QVTKOpenGLNativeWidget* m_vtkWidget;
    vtkSmartPointer<vtkRenderer> m_renderer;
    bool m_initialized = false;
};
