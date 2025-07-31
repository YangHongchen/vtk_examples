#include "CustomInteractorStyle.h"
#include "VTKOcclusionSimulation.h"
#include <QFile>
#include <QIODevice>
#include <QStandardPaths>
#include <QFileInfo>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>

VTKOcclusionSimulation::VTKOcclusionSimulation (QWidget *parent) : QVTKOpenGLNativeWidget{parent}
{
    // 1. 加载 STL 模型（通过 QRC 转临时文件）
    auto upperReader = loadStlFromQrc (":/assets/stl/upper_jaw.stl");
    auto lowerReader = loadStlFromQrc (":/assets/stl/lower_jaw.stl");

    // 2. 法线平滑
    auto upperNormals = smoothNormals (upperReader);
    auto lowerNormals = smoothNormals (lowerReader);

    // 3. Mapper 和 Actor
    auto upperActor = createActor (upperNormals, {1.0, 1.0, 1.0}); // 淡粉
    auto lowerActor = createActor (lowerNormals, {1.0, 1.0, 1.0}); // 浅蓝

    // 4. 居中（牙尖）
    centerByLowestPoint (upperActor, lowerActor, lowerReader);

    // 5. 渲染器设置
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (upperActor);
    renderer->AddActor (lowerActor);
    // VTK渲染背景色：#F0F0F0
    renderer->SetBackground (0xF0 / 255.0, 0xF0 / 255.0, 0xF0 / 255.0); // ≈ (0.941, 0.941, 0.941)

    // 6. 绑定渲染窗口
    auto renderWindow = this->renderWindow();
    renderWindow->AddRenderer (renderer);

    // 7. 禁用缩放交互器（可选）
    auto interactor = this->interactor();
    auto style = vtkSmartPointer<CustomInteractorStyle>::New();
    style->SetCurrentRenderer (renderer);
    interactor->SetInteractorStyle (style);
    // connect (this, &QVTKOpenGLNativeWidget::windowChanged, this, [ = ]
    // {
    //     renderWindow->Render();
    // });
}

void VTKOcclusionSimulation::setCameraView (const QString &direction)
{
    auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    if (!renderer) return;

    auto camera = renderer->GetActiveCamera();
    if (!camera) return;

    double bounds[6];
    renderer->ComputeVisiblePropBounds (bounds);
    double center[3] =
    {
        (bounds[0] + bounds[1]) / 2.0,
        (bounds[2] + bounds[3]) / 2.0,
        (bounds[4] + bounds[5]) / 2.0
    };

    double xSize = bounds[1] - bounds[0];
    double ySize = bounds[3] - bounds[2];
    double zSize = bounds[5] - bounds[4];
    double distance = std::max ({xSize, ySize, zSize}) * 2.5;

    if (direction == "front")
    {
        // 模型的脸在 +Z
        camera->SetPosition (center[0], center[1], center[2] + distance); // 正脸
        camera->SetViewUp (0, 1, 0); // Y向上
    }
    else if (direction == "back")
    {
        // 模型后脑勺是 -Z
        camera->SetPosition (center[0], center[1], center[2] - distance); // 后脑勺
        camera->SetViewUp (0, 1, 0);
    }
    else if (direction == "left")
    {
        // 模型左脸是 -X
        camera->SetPosition (center[0] - distance, center[1], center[2]); // 左脸
        camera->SetViewUp (0, 1, 0);
    }
    else if (direction == "right")
    {
        // 模型右脸是 +X
        camera->SetPosition (center[0] + distance, center[1], center[2]); // 右脸
        camera->SetViewUp (0, 1, 0);
    }
    else if (direction == "top")
    {
        // 模型下巴朝 +Y
        camera->SetPosition (center[0], center[1] + distance, center[2]); // 下巴
        camera->SetViewUp (0, 0, 1);

    }
    else if (direction == "bottom")
    {
        // 模型头顶朝 -Y
        camera->SetPosition (center[0], center[1] - distance, center[2]); // 头顶
        camera->SetViewUp (0, 0, 1); // Z向上
    }

    camera->SetFocalPoint (center);
    renderer->ResetCameraClippingRange();
    this->renderWindow()->Render();
}


vtkSmartPointer<vtkActor> VTKOcclusionSimulation::createSmoothedActor (const std::string &filePath)
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

void VTKOcclusionSimulation::centerByLowestPoint (vtkSmartPointer<vtkActor> upperActor,
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

vtkSmartPointer<vtkSTLReader> VTKOcclusionSimulation::loadStlFromQrc (const QString &qrcPath)
{
    QFile qrcFile (qrcPath);
    if (!qrcFile.open (QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open STL resource:" << qrcPath;
        return nullptr;
    }

    QString tempPath = QStandardPaths::writableLocation (QStandardPaths::TempLocation) + "/" + QFileInfo (
                           qrcPath).fileName();
    QFile tempFile (tempPath);
    if (!tempFile.open (QIODevice::WriteOnly))
    {
        qWarning() << "Failed to write temp file:" << tempPath;
        return nullptr;
    }

    tempFile.write (qrcFile.readAll());
    tempFile.close();

    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (tempPath.toUtf8().constData());
    reader->Update();
    return reader;
}

vtkSmartPointer<vtkPolyDataNormals> VTKOcclusionSimulation::smoothNormals (vtkSmartPointer<vtkSTLReader> reader)
{
    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection (reader->GetOutputPort());
    normals->SetFeatureAngle (60.0);
    normals->SplittingOff();
    normals->ConsistencyOn();
    normals->Update();
    return normals;
}

vtkSmartPointer<vtkActor> VTKOcclusionSimulation::createActor (vtkSmartPointer<vtkPolyDataNormals> normals,
        const std::array<double, 3> &color)
{
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection (normals->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);
    actor->GetProperty()->SetColor (color[0], color[1], color[2]);
    actor->GetProperty()->SetInterpolationToPhong();
    return actor;
}
