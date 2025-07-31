#include <vtkActor.h>                  // 渲染图元的实体（Actor）
#include <vtkPolyDataMapper.h>         // 将数据转换为图形几何结构
#include <vtkSTLReader.h>              // STL 文件读取器
#include <vtkSmartPointer.h>           // 智能指针（用于自动释放）
#include <vtkRenderer.h>               // 场景渲染器
#include <vtkRenderWindow.h>           // 渲染窗口
#include <vtkRenderWindowInteractor.h> // 窗口交互器
#include <vtkPolyDataNormals.h>        // 用于生成平滑法线
#include <vtkProperty.h>               // 控制模型属性（颜色、透明度等）
#include <limits>                      // 获取浮点最大值

// 工具函数：根据 STL 文件创建带平滑法线的 Actor
vtkSmartPointer<vtkActor> createSmoothedActor (const std::string& filePath)
{
    // 读取 STL 文件
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (filePath.c_str());
    reader->Update(); // 立即读取

    // 生成平滑法线
    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection (reader->GetOutputPort());
    normals->SetFeatureAngle (60.0); // 小于此角度的面将插值
    normals->SplittingOff();        // 禁止边缘分裂，保持连续性
    normals->ConsistencyOn();       // 保证法线方向一致
    normals->Update();

    // 创建 mapper（数据映射器）
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (normals->GetOutputPort());

    // 创建 actor
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);

    // 设置为 Phong 插值（更平滑的光照效果）
    actor->GetProperty()->SetInterpolationToPhong();

    return actor;
}

// 工具函数：根据下颌最低点（牙尖）将上下颌整体平移，使牙尖居中
void centerByLowestPoint (vtkSmartPointer<vtkActor> upperActor,
                          vtkSmartPointer<vtkActor> lowerActor,
                          vtkSmartPointer<vtkSTLReader> lowerReader)
{
    auto polyData = lowerReader->GetOutput();
    // polyData->Update(); // STLReader->Update() 已经执行，此处不需要重复

    double minZ = std::numeric_limits<double>::max(); // 初始化为最大值
    double lowestPoint[3] = {0, 0, 0}; // 存储最低点坐标

    // 遍历每个点，找到 Z 最小的点
    for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); ++i)
    {
        double p[3];
        polyData->GetPoint (i, p);
        if (p[2] < minZ)
        {
            minZ = p[2];
            lowestPoint[0] = p[0];
            lowestPoint[1] = p[1];
            lowestPoint[2] = p[2];
        }
    }

    // 将上下颌整体移动，使牙尖点对准原点
    upperActor->SetPosition (-lowestPoint[0], -lowestPoint[1], -lowestPoint[2]);
    lowerActor->SetPosition (-lowestPoint[0], -lowestPoint[1], -lowestPoint[2]);
}

int main (int argc, char* argv[])
{
    // 1. 创建 STL 读取器（上颌和下颌）
    auto upperReader = vtkSmartPointer<vtkSTLReader>::New();
    upperReader->SetFileName ("c:/stl/1.stl");
    upperReader->Update();

    auto lowerReader = vtkSmartPointer<vtkSTLReader>::New();
    lowerReader->SetFileName ("c:/stl/2.stl");
    lowerReader->Update();

    // 2. 生成平滑法线（上颌）
    auto upperNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    upperNormals->SetInputConnection (upperReader->GetOutputPort());
    upperNormals->SetFeatureAngle (60.0);
    upperNormals->SplittingOff();
    upperNormals->ConsistencyOn();
    upperNormals->Update();

    // 3. 生成平滑法线（下颌）
    auto lowerNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    lowerNormals->SetInputConnection (lowerReader->GetOutputPort());
    lowerNormals->SetFeatureAngle (60.0);
    lowerNormals->SplittingOff();
    lowerNormals->ConsistencyOn();
    lowerNormals->Update();

    // 4. 设置数据映射器
    auto upperMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    upperMapper->SetInputConnection (upperNormals->GetOutputPort());

    auto lowerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    lowerMapper->SetInputConnection (lowerNormals->GetOutputPort());

    // 5. 创建 Actor（上颌）
    auto upperActor = vtkSmartPointer<vtkActor>::New();
    upperActor->SetMapper (upperMapper);
    upperActor->GetProperty()->SetColor (1.0, 0.7, 0.6);          // 淡粉色
    upperActor->GetProperty()->SetInterpolationToPhong();         // 光滑着色

    // 6. 创建 Actor（下颌）
    auto lowerActor = vtkSmartPointer<vtkActor>::New();
    lowerActor->SetMapper (lowerMapper);
    lowerActor->GetProperty()->SetColor (0.6, 0.8, 1.0);          // 浅蓝色
    lowerActor->GetProperty()->SetInterpolationToPhong();

    // 7. 居中牙尖位置
    centerByLowestPoint (upperActor, lowerActor, lowerReader);

    // 8. 渲染器设置
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (upperActor);
    renderer->AddActor (lowerActor);
    renderer->SetBackground (0.1, 0.2, 0.3); // 背景色：深蓝灰

    // 9. 渲染窗口
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (1080, 780);

    // 10. 交互器
    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow (renderWindow);

    // 11. 显示窗口并开始交互
    renderWindow->Render();
    interactor->Start();

    return 0;
}
