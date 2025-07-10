// #include <vtkSmartPointer.h>
// #include <vtkCylinderSource.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>

// #include <vtkRenderer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>
// #include <vtkInteractorStyleTrackballCamera.h>
// #include <vtkLight.h>
// // #include <vtkCamera.h>
// #include <vtkPNGReader.h>
// #include <vtkTexture.h>
// #include <vtkPlaneSource.h>

// int main (int argc, char* argv[])
// {
//     vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
//     cylinder->SetHeight (3.0);
//     cylinder->SetRadius (1.0);
//     cylinder->SetResolution (64);
//     cylinder->Update();


//     vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

//     vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//     renderer->AddActor (actor);
//     renderer->SetBackground (0.1, 0.2, 0.4);

//     vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//     renderWindow->AddRenderer (renderer);
//     renderWindow->SetSize (600, 600);
//     renderWindow->SetWindowName ("VTK Camera Demo");

//     // 设置纹理1: 加载纹理图片
//     vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
//     reader->SetFileName ("C:\\texture.png");

//     vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
//     texture->SetInputConnection (reader->GetOutputPort());
//     texture->InterpolateOn();

//     vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();

//     vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     mapper->SetInputConnection (plane->GetOutputPort());

//     actor->SetMapper (mapper);
//     actor->SetTexture (texture);




//     vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//     interactor->SetRenderWindow (renderWindow);

//     vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//     interactor->SetInteractorStyle (style);

//     interactor->Initialize();
//     renderWindow->Render();
//     interactor->Start();

//     return EXIT_SUCCESS;
// }

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTextureMapToSphere.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkPNGReader.h>
#include <vtkTexture.h>
#include <vtkLight.h>

int main (int argc, char* argv[])
{
    // 1. 创建球体几何体
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius (1.0);
    sphere->SetThetaResolution (64);
    sphere->SetPhiResolution (64);
    sphere->Update();

    // 2. 生成纹理坐标（TextureMapToSphere）
    auto textureMapper = vtkSmartPointer<vtkTextureMapToSphere>::New();
    textureMapper->SetInputConnection (sphere->GetOutputPort());
    textureMapper->PreventSeamOn();  // 避免接缝
    textureMapper->Update();

    // 3. 映射器
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (textureMapper->GetOutputPort());

    // 4. 加载纹理图像
    auto reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName ("C:\\texture.png"); // 替换为你自己的纹理路径

    auto texture = vtkSmartPointer<vtkTexture>::New();
    texture->SetInputConnection (reader->GetOutputPort());
    texture->InterpolateOn();  // 平滑纹理

    // 5. 演员（Actor）
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);
    actor->SetTexture (texture);

    // 6. 渲染器和灯光
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground (0.1, 0.2, 0.3);
    renderer->AddActor (actor);

    auto light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition (5, 5, 5);
    light->SetFocalPoint (0, 0, 0);
    light->SetIntensity (0.8);
    renderer->AddLight (light);

    // 7. 渲染窗口
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (800, 600);
    renderWindow->SetWindowName ("VTK Sphere with Texture");

    // 8. 交互器
    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle (style);

    // 9. 渲染并启动交互
    interactor->Initialize();
    renderWindow->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}


