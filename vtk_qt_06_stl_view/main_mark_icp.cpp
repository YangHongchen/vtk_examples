#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <limits>
#include <iostream>

// 创建带平滑法线的Actor
vtkSmartPointer<vtkActor> createSmoothedActor (const std::string& filePath, double r, double g, double b)
{
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (filePath.c_str());
    reader->Update();

    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection (reader->GetOutputPort());
    normals->SetFeatureAngle (60.0);
    normals->SplittingOff();
    normals->ConsistencyOn();
    normals->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (normals->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);
    actor->GetProperty()->SetColor (r, g, b);
    actor->GetProperty()->SetInterpolationToPhong();

    return actor;
}

// 创建指定位置的小球标记
vtkSmartPointer<vtkActor> createMarkerSphere (const double pos[3], double radius = 0.3, const double color[3] = nullptr)
{
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetCenter (pos);
    sphere->SetRadius (radius);
    sphere->SetPhiResolution (20);
    sphere->SetThetaResolution (20);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (sphere->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);
    if (color)
        actor->GetProperty()->SetColor (color[0], color[1], color[2]);
    else
        actor->GetProperty()->SetColor (1.0, 0.0, 0.0); // 默认红色
    actor->GetProperty()->SetOpacity (1.0);

    return actor;
}

// 根据规则定位牙尖（Z最小）、左髁突（X最小）、右髁突（X最大区域内Z最大）
void detectKeyPoints (vtkSmartPointer<vtkSTLReader> reader,
                      double toothTip[3], double condyleLeft[3], double condyleRight[3])
{
    auto polyData = reader->GetOutput();
    vtkIdType numPoints = polyData->GetNumberOfPoints();

    double maxZ = std::numeric_limits<double>::lowest();
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();

    for (vtkIdType i = 0; i < numPoints; ++i)
    {
        double p[3];
        polyData->GetPoint (i, p);

        // 牙尖：Z 最大（最前端）
        if (p[2] > maxZ)
        {
            maxZ  = p[2];
            std::copy (p, p + 3, toothTip);
        }

        // 左髁突：X 最小
        if (p[0] < minX)
        {
            minX = p[0];
            std::copy (p, p + 3, condyleLeft);
        }

        // 右髁突：X 最大
        if (p[0] > maxX)
        {
            maxX = p[0];
            std::copy (p, p + 3, condyleRight);
        }
    }
}


int main()
{
    // 读取下颌模型
    auto lowerReader = vtkSmartPointer<vtkSTLReader>::New();
    lowerReader->SetFileName ("c:/stl/2.stl");
    lowerReader->Update();

    // 读取上颌模型并创建Actor
    auto upperActor = createSmoothedActor ("c:/stl/1.stl", 1.0, 0.7, 0.6);
    auto lowerActor = createSmoothedActor ("c:/stl/2.stl", 0.6, 0.8, 1.0);

    // 定位牙尖和髁突（未平移前）
    double tip[3], left[3], right[3];
    detectKeyPoints (lowerReader, tip, left, right);

    std::cout << "牙尖坐标: " << tip[0] << ", " << tip[1] << ", " << tip[2] << std::endl;
    std::cout << "左髁突坐标: " << left[0] << ", " << left[1] << ", " << left[2] << std::endl;
    std::cout << "右髁突坐标: " << right[0] << ", " << right[1] << ", " << right[2] << std::endl;

    // 平移上下颌使牙尖居中
    upperActor->SetPosition (-tip[0], -tip[1], -tip[2]);
    lowerActor->SetPosition (-tip[0], -tip[1], -tip[2]);

    // 平移关键点坐标，使标记球位置正确
    for (int i = 0; i < 3; ++i)
    {
        left[i] -= tip[i];
        right[i] -= tip[i];
        tip[i] = 0.0; // 牙尖现在为原点
    }

    // 创建标记球（小一点）
    const double red[3] = {1, 0, 0};
    const double green[3] = {0, 1, 0};
    const double blue[3] = {0, 0.7, 1};

    auto toothMarker = createMarkerSphere (tip, 0.3, red);
    auto condyleLMarker = createMarkerSphere (left, 0.3, green);
    auto condyleRMarker = createMarkerSphere (right, 0.3, blue);

    // 渲染设置
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (upperActor);
    renderer->AddActor (lowerActor);
    renderer->AddActor (toothMarker);
    renderer->AddActor (condyleLMarker);
    renderer->AddActor (condyleRMarker);
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
