#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

int main (int argc, char *argv[])
{
    // ✅ 1. 创建渲染窗口
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize (800, 600);                        // 设置窗口大小
    renderWindow->SetPosition (200, 150);                    // 设置窗口初始位置
    renderWindow->SetWindowName ("VTK 基础示例：球体");        // 设置窗口标题

    // ✅ 2. 创建渲染器并添加到窗口
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground (0.2, 0.3, 0.4);                  // 设置背景颜色（深蓝色调）
    renderWindow->AddRenderer (renderer);                     // 将渲染器加入到窗口中

    // ✅ 3. 构建可视化数据管线：球体 → 映射器 → 演员
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetThetaResolution (32);                         // 设置经度分辨率
    sphere->SetPhiResolution (32);                           // 设置纬度分辨率

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (sphere->GetOutputPort());    // 将球体输出连接到 mapper

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);                               // 设置几何映射器
    renderer->AddActor (actor);                              // 将演员添加到渲染器中

    // ✅ 4. 创建交互器并关联渲染窗口
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);              // 将交互器绑定到窗口

    // ✅ 5. 初始化渲染并启动交互循环
    renderer->ResetCamera();                                 // 自动重置相机以完整展示对象
    renderWindow->Render();                                  // 手动触发首次渲染
    interactor->Start();                                     // 启动交互循环（鼠标/键盘事件）

    return EXIT_SUCCESS;
}
