#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>

int main_soft (int argc, char* argv[])
{
    auto stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName ("c:/stl/1.stl");
    stlReader->Update();

    // 1.平滑法线生成器（自动计算平滑的 Vertex 法线）
    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection (stlReader->GetOutputPort());
    normals->SetFeatureAngle (10.0);  // 调整角度越小越锐利
    normals->SplittingOff();          // 不分割锐角边缘，保证连续性
    normals->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (normals->GetOutputPort());  // 2.这里采用normals。而非直接采用stlReader

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);
    actor->GetProperty()->SetInterpolationToPhong();  // 3. 平滑着色

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (actor);
    renderer->SetBackground (0.1, 0.2, 0.3);

    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (1080, 780);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    renderWindow->Render();
    interactor->Start();

    return 0;
}
