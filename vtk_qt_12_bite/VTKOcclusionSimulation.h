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
#include <vtkTransform.h>
#include <vtkCollisionDetectionFilter.h>


class VTKOcclusionSimulation : public QVTKOpenGLNativeWidget {

    Q_OBJECT
  public:
    explicit VTKOcclusionSimulation (QWidget *parent = nullptr);

    // 切换相机视角
    void setCameraView (const QString &direction);

    // 标记：左髁突
    void markPoints();


  private:
    // 工具函数：根据 STL 文件创建带平滑法线的 Actor
    vtkSmartPointer<vtkActor> createSmoothedActor (const std::string& filePath);
    // 工具函数：根据下颌最低点（牙尖）将上下颌整体平移，使牙尖居中
    void centerByLowestPoint (vtkSmartPointer<vtkActor> upperActor, vtkSmartPointer<vtkActor> lowerActor,
                              vtkSmartPointer<vtkSTLReader> lowerReader);
    // 从资源文件路径，加载stl模型
    vtkSmartPointer<vtkSTLReader> loadStlFromQrc (const QString &qrcPath);
    // 创建平滑效果
    vtkSmartPointer<vtkPolyDataNormals> smoothNormals (vtkSmartPointer<vtkSTLReader> reader);
    // 创建actor
    vtkSmartPointer<vtkActor> createActor (vtkSmartPointer<vtkPolyDataNormals> normals, const std::array<double, 3> &color);
    // 检测左髁突，右髁突，牙尖位坐标
    void detectKeyPoints (vtkSmartPointer<vtkSTLReader> reader, double toothTip[3], double condyleLeft[3],
                          double condyleRight[3]);
    // 创建指定位置的小球标记
    vtkSmartPointer<vtkActor> createMarkerSphere (const double pos[3], double radius = 0.3,
            const double color[3] = nullptr);

  private:
    // reader
    vtkSmartPointer<vtkSTLReader> m_upperReader;
    vtkSmartPointer<vtkSTLReader> m_lowerReader;

    // 绑定的上下颌actor，方便动画变换
    vtkSmartPointer<vtkActor> m_upperActor;
    vtkSmartPointer<vtkActor> m_lowerActor;

    // 变换控制下颌运动
    vtkSmartPointer<vtkTransform> m_lowerTransform;
    vtkSmartPointer<vtkTransform> m_upperTransform;

    // 碰撞检测过滤器
    vtkSmartPointer<vtkCollisionDetectionFilter> m_collisionFilter;


};

#endif // VTKOCCLUSIONSIMULATION_H
