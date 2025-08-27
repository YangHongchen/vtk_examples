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

int main_001 (int argc, char* argv[])
{
    // if (argc < 2)
    // {
    //     std::cerr << "Usage: " << argv[0] << " model.stl" << std::endl;
    //     return EXIT_FAILURE;
    // }

    // 1. 读取 STL
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName ("c:/stl/1.stl");

    // 2. 定义切平面 (Z=0)
    auto plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin (0.0, 0.0, 0.0); // 平面过原点
    plane->SetNormal (0.0, 0.0, 1.0); // Z 方向

    // 3. 用 Cutter 截取模型
    auto cutter = vtkSmartPointer<vtkCutter>::New();
    cutter->SetCutFunction (plane);
    cutter->SetInputConnection (reader->GetOutputPort());
    cutter->Update();

    // 4. 映射 STL 模型
    auto stlMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    stlMapper->SetInputConnection (reader->GetOutputPort());

    auto stlActor = vtkSmartPointer<vtkActor>::New();
    stlActor->SetMapper (stlMapper);
    stlActor->GetProperty()->SetOpacity (0.3); // 半透明，方便看到切面

    // 5. 映射切割结果 (轮廓线)
    auto cutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cutMapper->SetInputConnection (cutter->GetOutputPort());

    auto cutActor = vtkSmartPointer<vtkActor>::New();
    cutActor->SetMapper (cutMapper);
    cutActor->GetProperty()->SetColor (1.0, 0.0, 0.0); // 红色
    cutActor->GetProperty()->SetLineWidth (3.0);

    // 6. Renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    renderer->AddActor (stlActor);
    renderer->AddActor (cutActor);
    renderer->SetBackground (0.1, 0.1, 0.1);

    renderWindow->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}
