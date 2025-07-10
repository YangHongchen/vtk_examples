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

    // ✅ 添加灯光_1
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetColor (0, 1, 0);
    light->SetPosition (0, 0, 1);
    light->SetFocalPoint (renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight (light);

    // ✅ 添加灯光_2
    vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();
    light2->SetColor (0, 0, 1);
    light2->SetPosition (0, 0, -1);
    light2->SetFocalPoint (renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight (light2);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (600, 600);
    renderWindow->SetWindowName ("VTK Light Demo");

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle (style);

    interactor->Initialize();
    renderWindow->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}

