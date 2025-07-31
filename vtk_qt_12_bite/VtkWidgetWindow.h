#ifndef VTKWIDGETWINDOW_H
#define VTKWIDGETWINDOW_H

#include <vtkActor.h>                   // 渲染图元的实体（Actor）
#include <vtkPolyDataMapper.h>         // 将数据转换为图形几何结构
#include <vtkSTLReader.h>              // STL 文件读取器
#include <vtkSmartPointer.h>           // 智能指针（用于自动释放）
#include <vtkRenderer.h>               // 场景渲染器
#include <vtkRenderWindow.h>           // 渲染窗口
#include <vtkRenderWindowInteractor.h> // 窗口交互器
#include <vtkPolyDataNormals.h>        // 用于生成平滑法线
#include <vtkProperty.h>               // 控制模型属性（颜色、透明度等）
#include <limits>

class QVTKOpenGLNativeWidget;

class VtkWidgetWindow : public QMainWindow {
    Q_OBJECT
  public:
    VtkWidgetWindow (QWidget *parent = nullptr);
    ~VtkWidgetWindow() override;  // 显式声明析构函数
    void init();
  private:

  private:
    QVTKOpenGLNativeWidget *vtkWidget = nullptr;
};

#endif // VTKWIDGETWINDOW_H
