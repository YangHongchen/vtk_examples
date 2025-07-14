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
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkCollisionDetectionFilter.h>
#include <vtkCommand.h>
#include <limits>
#include <iostream>

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

vtkSmartPointer<vtkActor> createMarkerSphere (const double pos[3], double radius = 0.3,
        const double color[3] = nullptr)
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
        actor->GetProperty()->SetColor (1.0, 0.0, 0.0);
    actor->GetProperty()->SetOpacity (1.0);

    return actor;
}

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

        if (p[2] > maxZ)
        {
            maxZ = p[2];
            std::copy (p, p + 3, toothTip);
        }
        if (p[0] < minX)
        {
            minX = p[0];
            std::copy (p, p + 3, condyleLeft);
        }
        if (p[0] > maxX)
        {
            maxX = p[0];
            std::copy (p, p + 3, condyleRight);
        }
    }
}

// ----------- 定时器回调动画类 -----------
class OcclusionCallback : public vtkCommand {
  public:
    static OcclusionCallback *New() { return new OcclusionCallback; }

    vtkActor *lowerActor = nullptr;
    vtkTransform *transform = nullptr;
    vtkCollisionDetectionFilter *collisionFilter = nullptr;
    bool active = false;
    double step = 0.2;

    void Execute (vtkObject* caller, unsigned long eventId, void*) override
    {
        if (!active || eventId != vtkCommand::TimerEvent) return;

        transform->Translate (0, 0, step);
        lowerActor->Modified();

        collisionFilter->Update();
        if (collisionFilter->GetNumberOfContacts() > 0)
        {
            std::cout << "碰撞检测：上下颌接触！动画停止。\n";
            active = false;
        }
    }

    void Toggle()
    {
        active = !active;
        std::cout << (active ? "开始咬合动画\n" : "暂停咬合动画\n");
    }
};

// ---------- 主函数 ----------
int main()
{
    auto lowerReader = vtkSmartPointer<vtkSTLReader>::New();
    lowerReader->SetFileName ("c:/stl/2.stl");
    lowerReader->Update();

    auto upperActor = createSmoothedActor ("c:/stl/1.stl", 1.0, 0.7, 0.6);
    auto lowerActor = createSmoothedActor ("c:/stl/2.stl", 0.6, 0.8, 1.0);

    double tip[3], left[3], right[3];
    detectKeyPoints (lowerReader, tip, left, right);

    upperActor->SetPosition (-tip[0], -tip[1], -tip[2]);
    lowerActor->SetPosition (-tip[0], -tip[1], -tip[2]);

    for (int i = 0; i < 3; ++i)
    {
        left[i] -= tip[i];
        right[i] -= tip[i];
        tip[i] = 0.0;
    }

    auto toothMarker = createMarkerSphere (tip, 0.3, (const double[]) {1, 0, 0});
    auto condyleLMarker = createMarkerSphere (left, 0.3, (const double[]) {0, 1, 0});
    auto condyleRMarker = createMarkerSphere (right, 0.3, (const double[]) {0, 0.7, 1});

    // 动画 transform
    auto transform = vtkSmartPointer<vtkTransform>::New();
    lowerActor->SetUserTransform (transform);

    // 碰撞检测
    auto collisionFilter = vtkSmartPointer<vtkCollisionDetectionFilter>::New();
    collisionFilter->SetInputConnection (0,
                                         dynamic_cast<vtkPolyDataMapper *> (upperActor->GetMapper())->GetInputConnection (0, 0));
    collisionFilter->SetInputConnection (1,
                                         dynamic_cast<vtkPolyDataMapper *> (lowerActor->GetMapper())->GetInputConnection (0, 0));
    collisionFilter->SetMatrix (0, upperActor->GetMatrix());
    collisionFilter->SetMatrix (1, transform->GetMatrix());
    collisionFilter->SetBoxTolerance (0.01);
    collisionFilter->SetCellTolerance (0.0);
    collisionFilter->SetNumberOfCellsPerNode (2);
    collisionFilter->SetCollisionModeToAllContacts();
    collisionFilter->GenerateScalarsOn();

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

    // 设置动画回调
    auto callback = vtkSmartPointer<OcclusionCallback>::New();
    callback->lowerActor = lowerActor;
    callback->transform = transform;
    callback->collisionFilter = collisionFilter;

    interactor->AddObserver (vtkCommand::TimerEvent, callback);
    interactor->CreateRepeatingTimer (30);

    // 按空格开始 / 停止动画
    auto keyCallback = vtkSmartPointer<KeyPressCallback>::New();
    keyCallback->animationCallback = callback;
    interactor->AddObserver (vtkCommand::KeyPressEvent, keyCallback);

    renderWindow->Render();
    interactor->Start();

    return 0;
}
