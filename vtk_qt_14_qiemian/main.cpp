#include "src/vtk/style/DisabledInteractorStyle.h"
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>

#include <vtkInteractorStyleRubberBandPick.h>


int main (int argc, char* argv[])
{


    // 1. 读取 STL
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName ("c:/stl/1.stl");
    reader->Update();

    // 获取 STL 范围
    double bounds[6];
    reader->GetOutput()->GetBounds (bounds);
    std::cout << "STL Bounds: "
              << bounds[0] << ", " << bounds[1] << " | "
              << bounds[2] << ", " << bounds[3] << " | "
              << bounds[4] << ", " << bounds[5] << std::endl;

    // 2. 定义切平面（放在模型中间 Z）
    double centerZ = 0.5 * (bounds[4] + bounds[5]);
    auto plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin (0.0, 0.0, centerZ);
    plane->SetNormal (0.0, 0.0, 1.0);

    // 3. 切割
    auto cutter = vtkSmartPointer<vtkCutter>::New();
    cutter->SetCutFunction (plane);
    cutter->SetInputConnection (reader->GetOutputPort());
    cutter->Update();

    // ============ 左侧 3D 视图 ============
    auto stlMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    stlMapper->SetInputConnection (reader->GetOutputPort());

    auto stlActor = vtkSmartPointer<vtkActor>::New();
    stlActor->SetMapper (stlMapper);
    stlActor->GetProperty()->SetOpacity (0.3);

    auto cutMapper3D = vtkSmartPointer<vtkPolyDataMapper>::New();
    cutMapper3D->SetInputConnection (cutter->GetOutputPort());

    auto cutActor3D = vtkSmartPointer<vtkActor>::New();
    cutActor3D->SetMapper (cutMapper3D);
    cutActor3D->GetProperty()->SetColor (1, 0, 0);
    cutActor3D->GetProperty()->SetLineWidth (3.0);

    auto renderer3D = vtkSmartPointer<vtkRenderer>::New();
    renderer3D->AddActor (stlActor);
    renderer3D->AddActor (cutActor3D);
    renderer3D->SetBackground (0.1, 0.1, 0.1);
    renderer3D->SetViewport (0.0, 0.0, 0.66, 1.0); // 左边 2/3

    // ============ 右侧 2D 视图 ============
    auto cutMapper2D = vtkSmartPointer<vtkPolyDataMapper>::New();
    cutMapper2D->SetInputConnection (cutter->GetOutputPort());
    cutMapper2D->Update();

    auto cutActor2D = vtkSmartPointer<vtkActor>::New();
    cutActor2D->SetMapper (cutMapper2D);
    cutActor2D->GetProperty()->SetColor (0, 1, 0);
    cutActor2D->GetProperty()->SetLineWidth (2.0);

    auto renderer2D = vtkSmartPointer<vtkRenderer>::New();
    renderer2D->AddActor (cutActor2D);
    renderer2D->SetBackground (1.0, 1.0, 1.0);
    renderer2D->SetViewport (0.66, 0.0, 1.0, 1.0); // 右边 1/3

    // 设置 2D 相机
    double cutBounds[6];
    cutter->GetOutput()->GetBounds (cutBounds);
    double cx = 0.5 * (cutBounds[0] + cutBounds[1]);
    double cy = 0.5 * (cutBounds[2] + cutBounds[3]);
    double cz = 0.5 * (cutBounds[4] + cutBounds[5]);

    auto camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetFocalPoint (cx, cy, cz);
    camera->SetPosition (cx, cy, cz + 200); // 从 Z+ 方向俯视
    camera->ParallelProjectionOn();
    renderer2D->SetActiveCamera (camera);
    renderer2D->ResetCamera();

    // ============ 渲染窗口 ============
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer3D);
    renderWindow->AddRenderer (renderer2D);

    auto style = vtkSmartPointer<DisabledInteractorStyle>::New();

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);
    interactor->SetInteractorStyle (style);


    renderWindow->SetSize (900, 600);
    renderWindow->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}
