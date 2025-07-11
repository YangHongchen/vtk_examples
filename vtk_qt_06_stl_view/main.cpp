#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

int main (int argc, char* argv[])
{
    // 1. 读取 STL 文件
    auto stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName ("c:/stl/1.stl");

    // 2.创建mapper
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (stlReader->GetOutputPort());

    // 3.创建actor
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);

    // 4.渲染器
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (actor);
    renderer->SetBackground (0.1, 0.2, 0.3);

    // 5.渲染窗口
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (1080, 780);

    // 6. 交互器
    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    // 渲染 window
    renderWindow->Render();
    // 启动交互（否则立即退出：非报错）
    interactor->Start();

    return 0;
}
