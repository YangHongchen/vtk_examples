#include "CustomInteractorStyle.h"
#include "VTKOcclusionSimulation.h"
#include <QFile>
#include <QIODevice>
#include <QStandardPaths>
#include <QFileInfo>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSet.h>
#include <vtkTransform.h>
#include <vtkCollisionDetectionFilter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSphereSource.h>
#include <QTimer>


VTKOcclusionSimulation::VTKOcclusionSimulation (QWidget *parent) : QVTKOpenGLNativeWidget{parent}
{
    // 1. 加载 STL 模型（通过 QRC 转临时文件）
    m_upperReader = loadStlFromQrc (":/assets/stl/upper_jaw.stl");
    m_lowerReader = loadStlFromQrc (":/assets/stl/lower_jaw.stl");

    // 2. 法线平滑
    auto upperNormals = smoothNormals (m_upperReader);
    auto lowerNormals = smoothNormals (m_lowerReader);

    // 3. Mapper 和 Actor
    m_upperActor = createActor (upperNormals, {1.0, 0.7, 0.6}); // 淡粉
    m_lowerActor = createActor (lowerNormals, {1.0, 1.0, 1.0}); // 浅蓝

    // 4. 居中（牙尖）
    centerByLowestPoint (m_upperActor, m_lowerActor, m_lowerReader);

    // 5. 渲染器设置
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor (m_upperActor);
    renderer->AddActor (m_lowerActor);
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

    setCameraView ("front");

    // 自动定位左髁突，右髁突，牙尖位坐标
    detectKeyPoints (m_lowerReader, tip, leftCondyle, rightCondyle);


    m_lowerTransform = vtkSmartPointer<vtkTransform>::New();
    m_lowerTransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    m_lowerTransformFilter->SetInputData (m_lowerReader->GetOutput());
    m_lowerTransformFilter->SetTransform (m_lowerTransform);
    m_lowerTransformFilter->Update();

    // Mapper 和 Actor
    m_lowerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_lowerMapper->SetInputConnection (m_lowerTransformFilter->GetOutputPort());
    m_lowerActor->SetMapper (m_lowerMapper);


    QTimer* timer = new QTimer (this);
    connect (timer, &QTimer::timeout, this, &VTKOcclusionSimulation::animateLowerJaw);
    timer->start (30);

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

void VTKOcclusionSimulation::markPoints()
{
    qDebug() << "标记左髁突";
    // 定位牙尖和髁突（未平移前）
    // double tip[3], left[3], right[3];


    // std::cout << "牙尖坐标: " << tip[0] << ", " << tip[1] << ", " << tip[2] << std::endl;
    // std::cout << "左髁突坐标: " << left[0] << ", " << left[1] << ", " << left[2] << std::endl;
    // std::cout << "右髁突坐标: " << right[0] << ", " << right[1] << ", " << right[2] << std::endl;

    // 平移上下颌使牙尖居中
    m_upperActor->SetPosition (-tip[0], -tip[1], -tip[2]);
    m_lowerActor->SetPosition (-tip[0], -tip[1], -tip[2]);
    // 平移关键点坐标，使标记球位置正确
    for (int i = 0; i < 3; ++i)
    {
        leftCondyle[i] -= tip[i];
        rightCondyle[i] -= tip[i];
        tip[i] = 0.0; // 牙尖现在为原点
    }

    // 创建标记球（小一点）
    const double red[3] = {1, 0, 0};
    const double green[3] = {0, 1, 0};
    const double blue[3] = {0, 0.7, 1};

    auto toothMarker = createMarkerSphere (tip, 0.3, red);
    auto condyleLMarker = createMarkerSphere (leftCondyle, 0.3, green);
    auto condyleRMarker = createMarkerSphere (rightCondyle, 0.3, blue);

    // 渲染设置
    auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor (toothMarker);
    renderer->AddActor (condyleLMarker);
    renderer->AddActor (condyleRMarker);

    detectKeyPoints (m_lowerReader, leftCondyle, rightCondyle, tip);

    // 刷新
    // this->renderWindow()->Render();

    setCameraView ("front");
}

void VTKOcclusionSimulation::animateLowerJaw()
{
    // 先重置变换
    m_lowerTransform->Identity();
    // 移动旋转中心到原点（平移负的关节中心）
    m_lowerTransform->Translate (-m_jointCenter[0], -m_jointCenter[1], -m_jointCenter[2]);
    // 绕关节轴旋转 angle 度
    m_lowerTransform->RotateWXYZ (angle, m_axis[0], m_axis[1], m_axis[2]);
    // 再平移回来
    m_lowerTransform->Translate (m_jointCenter[0], m_jointCenter[1], m_jointCenter[2]);
    m_lowerTransformFilter->Update();
    // 控制咬合角度增减
    if (closing)
        angle +=  0.1; // 每帧增加1度
    else
        angle -=  0.1;
    if (angle >= 5.0) closing = false;
    else if (angle <= 0.0) closing = true;

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

void VTKOcclusionSimulation::detectKeyPoints (vtkSmartPointer<vtkSTLReader> reader, double toothTip[],
        double condyleLeft[], double condyleRight[])
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

    m_jointCenter[0] = (leftCondyle[0] + rightCondyle[0]) / 2.0;
    m_jointCenter[1] = (leftCondyle[1] + rightCondyle[1]) / 2.0;
    m_jointCenter[2] = (leftCondyle[2] + rightCondyle[2]) / 2.0;

    double axis[3] = { rightCondyle[0] - leftCondyle[0],
                       rightCondyle[1] - leftCondyle[1],
                       rightCondyle[2] - leftCondyle[2]
                     };
    double len = std::sqrt (axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
    m_axis[0] = axis[0] / len;
    m_axis[1] = axis[1] / len;
    m_axis[2] = axis[2] / len;
}

vtkSmartPointer<vtkActor> VTKOcclusionSimulation::createMarkerSphere (const double pos[], double radius,
        const double color[])
{
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetCenter (pos);
    sphere->SetRadius (radius);
    sphere->SetPhiResolution (15);
    sphere->SetThetaResolution (15);

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
