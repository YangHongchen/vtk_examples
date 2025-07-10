// #include <vtkSmartPointer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindowInteractor.h>
// #include <vtkInteractorStyleTrackballCamera.h>
// #include <vtkCylinderSource.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>

// int main (int argc, char *argv[])
// {
//     vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
//     cylinder->SetHeight (3.0);
//     cylinder->SetRadius (1.0);
//     cylinder->SetResolution (10);

//     vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     cylinderMapper->SetInputConnection (cylinder->GetOutputPort());

//     vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
//     cylinderActor->SetMapper (cylinderMapper);

//     vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//     renderer->AddActor (cylinderActor);
//     renderer->SetBackground (0.1, 0.2, 0.4);

//     vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
//     renWin->AddRenderer (renderer);
//     renWin->SetSize (300, 300);

//     vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//     iren->SetRenderWindow (renWin);

//     vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//     iren->SetInteractorStyle (style);

//     iren->Initialize();
//     iren->Start();

//     return 0;

// }


#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

int main (int argc, char* argv[])
{
    // ✅ 1. 创建圆柱体几何体数据源
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetHeight (3.0);     // 设置高度
    cylinder->SetRadius (1.0);     // 设置半径
    cylinder->SetResolution (64);  // 设置分段数（越高越平滑）
    cylinder->Update();            // 立即执行生成数据（不是必须）

    // ✅ 2. 将几何数据连接到 Mapper（用于将数据转为图形）
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (cylinder->GetOutputPort()); // 数据输出口连接到 Mapper 输入

    // ✅ 3. 创建 Actor（演员：绑定 Mapper 并可控制外观、位置等）
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper); // 设置演员使用的映射器

    // ✅ 4. 创建渲染器（Renderer：管理所有 Actor，控制场景背景、光照等）
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (actor);                     // 添加演员
    renderer->SetBackground (0.1, 0.2, 0.4);         // 设置背景颜色（深蓝）

    // ✅ 5. 创建渲染窗口（RenderWindow：实际显示图形的窗口）
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);           // 添加渲染器
    renderWindow->SetSize (600, 600);               // 设置窗口大小
    renderWindow->SetWindowName ("VTK Cylinder Demo");

    // ✅ 6. 创建交互器（RenderWindowInteractor：捕捉鼠标/键盘事件）
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);     // 绑定渲染窗口

    // ✅ 7. 设置交互风格（TrackballCamera：默认旋转、缩放等交互）
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle (style);         // 设置交互风格

    // ✅ 8. 初始化并启动交互循环
    interactor->Initialize();  // 初始化内部事件循环机制
    renderWindow->Render();    // 初次渲染（必需）
    interactor->Start();       // 进入交互循环（阻塞）

    return EXIT_SUCCESS;
}

