#ifndef VTKOCCLUSIONSIMULATION_H
#define VTKOCCLUSIONSIMULATION_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>                  // 渲染图元的实体（Actor）
#include <vtkPolyDataMapper.h>         // 将数据转换为图形几何结构
#include <vtkSTLReader.h>              // STL 文件读取器
#include <vtkSmartPointer.h>           // 智能指针（用于自动释放）
#include <vtkRenderer.h>               // 场景渲染器
#include <vtkRenderWindow.h>           // 渲染窗口
#include <vtkRenderWindowInteractor.h> // 窗口交互器
#include <vtkPolyDataNormals.h>        // 用于生成平滑法线
#include <vtkProperty.h>               // 控制模型属性（颜色、透明度等）
#include <QVTKOpenGLNativeWidget.h>


class VTKOcclusionSimulation : public QVTKOpenGLNativeWidget {

    Q_OBJECT
  public:
    explicit VTKOcclusionSimulation (QWidget *parent = nullptr);

  private:

    // 工具函数：根据 STL 文件创建带平滑法线的 Actor
    vtkSmartPointer<vtkActor> createSmoothedActor (const std::string& filePath);

    // 工具函数：根据下颌最低点（牙尖）将上下颌整体平移，使牙尖居中
    void centerByLowestPoint (vtkSmartPointer<vtkActor> upperActor,
                              vtkSmartPointer<vtkActor> lowerActor,
                              vtkSmartPointer<vtkSTLReader> lowerReader);

    // 从资源文件路径，加载stl模型
    vtkSmartPointer<vtkSTLReader> loadStlFromQrc (const QString &qrcPath);

    // 创建平滑效果
    vtkSmartPointer<vtkPolyDataNormals> smoothNormals (vtkSmartPointer<vtkSTLReader> reader);

    //创建actor
    vtkSmartPointer<vtkActor> createActor (vtkSmartPointer<vtkPolyDataNormals> normals, const std::array<double, 3> &color);

};

#endif // VTKOCCLUSIONSIMULATION_H
