#pragma once
#include <string>

#include <vtkSmartPointer.h>

class vtkPlane;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkDistanceWidget;
class vtkAngleWidget;

class PlaneSliceViewer {
  public:
    enum class SmoothMode { None, Geometry, Normals };

    PlaneSliceViewer (const std::string& upperFile, const std::string& lowerFile);
    ~PlaneSliceViewer();

    // 初始化并启动交互（阻塞，调用后进入交互循环）
    void Run();

    // 渲染控制 API (可由外部 UI 调用)
    void ZoomIn();
    void ZoomOut();
    void ResetView();
    void SetViewDirection (const std::string& dir); // "front","back","left","right","top","bottom"

    // 平滑控制
    void SetSmoothMode (SmoothMode mode);
    SmoothMode GetSmoothMode() const;

    // 测量工具
    void StartMeasureDistance();
    void StartMeasureAngle();
    void ClearMeasurements();

  private:
    void SetupPipeline();      // 构建/（重）构建渲染管线（根据 SmoothMode）
    void SetupInteractor();
    void UpdateCameraAfterCenterChange();

  private:
    std::string UpperFile;
    std::string LowerFile;

    vtkSmartPointer<vtkPlane> Plane;
    vtkSmartPointer<vtkRenderer> RenLeft;
    vtkSmartPointer<vtkRenderer> RenRight;
    vtkSmartPointer<vtkRenderWindow> RenWin;
    vtkSmartPointer<vtkRenderWindowInteractor> Iren;

    // measurement widgets
    vtkSmartPointer<vtkDistanceWidget> DistanceWidget;
    vtkSmartPointer<vtkAngleWidget> AngleWidget;

    // mode
    SmoothMode CurrentSmoothMode;

    // center point of scene
    double Center[3];
};
