#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLight.h>
#include <vtkCamera.h>

/**
 * - 观众的眼睛，好比三维渲染场景中的相机。
 * - 在vtk中用vtkCamera类表示
 * - vtkCamera负责把三维场景投影到二维平面。
 * - 相机投影有以下几个元素：
 * -  1. 相机位置（vtkCamera::SetPosition 方法设置）
 * -  2. 相机焦点（vtkCamera::SetFocusPoint 方法设置，默认焦点位置在世界坐标系的原点）
 *    3. 朝上方向（朝上方向即哪个方向为相机朝上的方向，就好比治理看东西，方向为头朝上，看到的东西也是直立的，如果倒立看东西这时候方向为头朝下，看到的东西就是倒立的。
 *       相机位置，相机焦点，朝上方向三个元素确定了相机的实际方向，即确定相机的视图。）
 *    4. 投影方向（要该元素用于确定actor是如何映射到想平面的。）
 *    5. 投影方法
 *    6. 视角
 *    7. 前后裁剪平面

 */


int main (int argc, char* argv[])
{
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetHeight (3.0);
    cylinder->SetRadius (1.0);
    cylinder->SetResolution (64);
    cylinder->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (cylinder->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (actor);
    renderer->SetBackground (0.1, 0.2, 0.4);

    // 设置一个顶部视角（Top View）
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetPosition (0, 100, 0);        // 高空俯视
    camera->SetFocalPoint (0, 0, 0);        // 看向原点
    camera->SetViewUp (0, 0, -1);           // 让 Z 轴朝下
    camera->Azimuth (30);                   // 绕 up 向右旋转（水平旋转）
    camera->Elevation (20);                 // 绕右轴旋转（垂直旋转）
    camera->SetParallelProjection (true);   // 启用正交投影
    camera->SetParallelScale (5.0);         // 设置视野范围（上下边界的一半）
    camera->Dolly (1.5);                    // 放大视野
    camera->SetParallelProjection (true);   // 投影方式
    renderer->SetActiveCamera (camera);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (600, 600);
    renderWindow->SetWindowName ("VTK Camera Demo");

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle (style);

    interactor->Initialize();
    renderWindow->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}

