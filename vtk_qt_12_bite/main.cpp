#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

int main (int argc, char* argv[])
{
    // 1. 加载上下颌STL模型
    vtkSmartPointer<vtkSTLReader> upperReader = vtkSmartPointer<vtkSTLReader>::New();
    upperReader->SetFileName ("c:/stl/1.stl");

    vtkSmartPointer<vtkSTLReader> lowerReader = vtkSmartPointer<vtkSTLReader>::New();
    lowerReader->SetFileName ("c:/stl/2.stl");

    // 2. 法线计算和平滑处理
    auto upperSmoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    upperSmoother->SetInputConnection (upperReader->GetOutputPort());
    upperSmoother->SetNumberOfIterations (50); // 平滑迭代次数
    upperSmoother->SetRelaxationFactor (0.1);  // 平滑强度

    auto lowerSmoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    lowerSmoother->SetInputConnection (lowerReader->GetOutputPort());
    lowerSmoother->SetNumberOfIterations (50);
    lowerSmoother->SetRelaxationFactor (0.1);

    // 3. 计算法线（关键步骤）
    auto upperNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    upperNormals->SetInputConnection (upperSmoother->GetOutputPort());
    upperNormals->ComputePointNormalsOn();
    upperNormals->ComputeCellNormalsOn();
    upperNormals->SetFeatureAngle (60.0);
    upperNormals->SplittingOff();  // 避免法线分裂导致伪影

    auto lowerNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    lowerNormals->SetInputConnection (lowerSmoother->GetOutputPort());
    lowerNormals->ComputePointNormalsOn();
    lowerNormals->ComputeCellNormalsOn();
    lowerNormals->SetFeatureAngle (60.0);
    lowerNormals->SplittingOff();

    // 4. 创建Mapper和Actor
    auto upperMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    upperMapper->SetInputConnection (upperNormals->GetOutputPort());

    auto lowerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    lowerMapper->SetInputConnection (lowerNormals->GetOutputPort());

    auto upperActor = vtkSmartPointer<vtkActor>::New();
    upperActor->SetMapper (upperMapper);
    upperActor->GetProperty()->SetColor (1.0, 0.5, 0.5); // 上颌颜色
    upperActor->GetProperty()->SetOpacity (0.7);         // 设置透明度
    upperActor->GetProperty()->SetBackfaceCulling (1);   // 关键：背面剔除
    upperActor->GetProperty()->SetFrontfaceCulling (0);  // 保留正面

    auto lowerActor = vtkSmartPointer<vtkActor>::New();
    lowerActor->SetMapper (lowerMapper);
    lowerActor->GetProperty()->SetColor (0.5, 0.5, 1.0); // 下颌颜色
    lowerActor->GetProperty()->SetOpacity (0.7);
    lowerActor->GetProperty()->SetBackfaceCulling (1);
    lowerActor->GetProperty()->SetFrontfaceCulling (0);

    // 5. 渲染设置（关键优化）
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (upperActor);
    renderer->AddActor (lowerActor);
    renderer->SetBackground (0.1, 0.2, 0.4);

    // 启用深度剥离（关键抗重影技术）
    renderer->SetUseDepthPeeling (1);
    renderer->SetMaximumNumberOfPeels (100);
    renderer->SetOcclusionRatio (0.1);

    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (1200, 800);
    renderWindow->SetAlphaBitPlanes (1); // 启用alpha通道支持
    renderWindow->SetMultiSamples (0);   // 禁用多重采样（与深度剥离冲突）

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    // 6. 相机定位
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth (30);
    renderer->GetActiveCamera()->Elevation (30);
    renderer->ResetCameraClippingRange();

    // 7. 开始渲染
    renderWindow->Render();
    interactor->Start();

    return 0;
}
