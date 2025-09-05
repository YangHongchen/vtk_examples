// #include "PlaneSliceViewer.h"

// #include <vtkPlane.h>
// #include <vtkSTLReader.h>
// #include <vtkSmoothPolyDataFilter.h>
// #include <vtkPolyDataNormals.h>
// #include <vtkCutter.h>
// #include <vtkSplineFilter.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>
// #include <vtkCamera.h>
// #include <vtkProperty.h>
// #include <vtkMath.h>
// #include <vtkInteractorStyleUser.h>
// #include <vtkObjectFactory.h>

// #include <vtkDistanceWidget.h>
// #include <vtkDistanceRepresentation3D.h>
// #include <vtkAngleWidget.h>
// #include <vtkAngleRepresentation3D.h>

// #include <algorithm>
// #include <iostream>

// // ========== 自定义交互：旋转切面 ==========
// class PlaneRotateInteractorStyle : public vtkInteractorStyleUser {
//   public:
//     static PlaneRotateInteractorStyle *New();
//     vtkTypeMacro (PlaneRotateInteractorStyle, vtkInteractorStyleUser);

//     PlaneRotateInteractorStyle()
//     {
//         Plane = nullptr;
//         RenderWindow = nullptr;
//         RenRight = nullptr;
//         Dragging = false;
//         LastY = 0;
//         AngleDeg = 0.0;
//         Sensitivity = 0.3;
//         MinAngle = 0.0;
//         MaxAngle = 180.0;
//         Center[0] = Center[1] = Center[2] = 0.0;
//     }

//     void SetPlane (vtkPlane* p) { Plane = p; }
//     void SetRenderWindow (vtkRenderWindow* rw) { RenderWindow = rw; }
//     void SetRendererRight (vtkRenderer* r) { RenRight = r; }
//     void SetCenter (double cx, double cy, double cz) { Center[0] = cx; Center[1] = cy; Center[2] = cz; }
//     void SetSensitivity (double s) { Sensitivity = s; }
//     void SetAngleLimits (double minA, double maxA) { MinAngle = minA; MaxAngle = maxA; }

//     void OnLeftButtonDown() override
//     {
//         Dragging = true;
//         int x, y; this->GetInteractor()->GetEventPosition (x, y);
//         LastY = y;
//     }

//     void OnMouseMove() override
//     {
//         if (!Dragging || !Plane || !RenderWindow || !RenRight) return;
//         int x, y; this->GetInteractor()->GetEventPosition (x, y);
//         int dy = y - LastY; LastY = y;

//         double deltaDeg = -dy * Sensitivity;
//         AngleDeg += deltaDeg;
//         if (AngleDeg < MinAngle) AngleDeg = MinAngle;
//         if (AngleDeg > MaxAngle) AngleDeg = MaxAngle;

//         double rad = vtkMath::RadiansFromDegrees (AngleDeg);
//         double nx = std::sin (rad), ny = 0.0, nz = std::cos (rad);

//         Plane->SetNormal (nx, ny, nz);
//         Plane->SetOrigin (Center);

//         auto cam = RenRight->GetActiveCamera();
//         cam->SetFocalPoint (Center);
//         cam->SetPosition (Center[0] + nx * 200, Center[1] + ny * 200, Center[2] + nz * 200);
//         cam->ParallelProjectionOn();
//         cam->SetParallelScale (30);
//         cam->SetViewUp (0, 1, 0);
//         RenRight->ResetCameraClippingRange();

//         RenderWindow->Render();
//     }

//     void OnLeftButtonUp() override { Dragging = false; }

//   private:
//     vtkPlane *Plane;
//     vtkRenderWindow *RenderWindow;
//     vtkRenderer *RenRight;
//     bool Dragging;
//     int LastY;
//     double AngleDeg, Sensitivity, MinAngle, MaxAngle;
//     double Center[3];
// };
// vtkStandardNewMacro (PlaneRotateInteractorStyle);

// // ========== PlaneSliceViewer 实现 ==========
// #include <cassert>

// PlaneSliceViewer::PlaneSliceViewer (const std::string& upperFile, const std::string& lowerFile)
//     : UpperFile (upperFile), LowerFile (lowerFile), CurrentSmoothMode (SmoothMode::Normals)
// {
//     Plane = nullptr;
//     Center[0] = Center[1] = Center[2] = 0.0;
// }

// PlaneSliceViewer::~PlaneSliceViewer()
// {
//     // ensure widgets off
//     ClearMeasurements();
// }

// void PlaneSliceViewer::SetupPipeline()
// {
//     // readers
//     auto upperReader = vtkSmartPointer<vtkSTLReader>::New();
//     upperReader->SetFileName (UpperFile.c_str());
//     upperReader->Update();

//     auto lowerReader = vtkSmartPointer<vtkSTLReader>::New();
//     lowerReader->SetFileName (LowerFile.c_str());
//     lowerReader->Update();

//     // compute scene center based on bounds of raw data
//     double b1[6], b2[6];
//     upperReader->GetOutput()->GetBounds (b1);
//     lowerReader->GetOutput()->GetBounds (b2);
//     Center[0] = 0.5 * (std::min (b1[0], b2[0]) + std::max (b1[1], b2[1]));
//     Center[1] = 0.5 * (std::min (b1[2], b2[2]) + std::max (b1[3], b2[3]));
//     Center[2] = 0.5 * (std::min (b1[4], b2[4]) + std::max (b1[5], b2[5]));

//     // ---------- choose smoothing mode ----------
//     vtkSmartPointer<vtkPolyDataMapper> upperMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     vtkSmartPointer<vtkPolyDataMapper> lowerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

//     if (CurrentSmoothMode == SmoothMode::Geometry)
//     {
//         auto upperSmooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//         upperSmooth->SetInputConnection (upperReader->GetOutputPort());
//         upperSmooth->SetNumberOfIterations (30);
//         upperSmooth->SetRelaxationFactor (0.1);
//         upperSmooth->BoundarySmoothingOn();
//         upperSmooth->Update();

//         auto lowerSmooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//         lowerSmooth->SetInputConnection (lowerReader->GetOutputPort());
//         lowerSmooth->SetNumberOfIterations (30);
//         lowerSmooth->SetRelaxationFactor (0.1);
//         lowerSmooth->BoundarySmoothingOn();
//         lowerSmooth->Update();

//         upperMapper->SetInputConnection (upperSmooth->GetOutputPort());
//         lowerMapper->SetInputConnection (lowerSmooth->GetOutputPort());
//     }
//     else if (CurrentSmoothMode == SmoothMode::Normals)
//     {
//         auto upperNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
//         upperNormals->SetInputConnection (upperReader->GetOutputPort());
//         upperNormals->SetFeatureAngle (10.0);
//         upperNormals->SplittingOff();
//         upperNormals->ConsistencyOn();
//         upperNormals->Update();

//         auto lowerNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
//         lowerNormals->SetInputConnection (lowerReader->GetOutputPort());
//         lowerNormals->SetFeatureAngle (10.0);
//         lowerNormals->SplittingOff();
//         lowerNormals->ConsistencyOn();
//         lowerNormals->Update();

//         upperMapper->SetInputConnection (upperNormals->GetOutputPort());
//         lowerMapper->SetInputConnection (lowerNormals->GetOutputPort());
//     }
//     else   // None
//     {
//         upperMapper->SetInputConnection (upperReader->GetOutputPort());
//         lowerMapper->SetInputConnection (lowerReader->GetOutputPort());
//     }

//     // ---------- cut plane ----------
//     Plane = vtkSmartPointer<vtkPlane>::New();
//     Plane->SetOrigin (Center);
//     Plane->SetNormal (0, 0, 1);

//     auto upperCutter = vtkSmartPointer<vtkCutter>::New();
//     upperCutter->SetCutFunction (Plane);
//     // for cutter input we use raw geometry (no normals only) so use reader or smoothed geometry depending on mode:
//     if (CurrentSmoothMode == SmoothMode::Geometry)
//     {
//         // if geometry smoothing used, cut the smoothed mesh
//         // recreate the smooth filters to get ports (we already ran above Update; but create new pipes for connecting)
//         auto upperSmooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//         upperSmooth->SetInputConnection (upperReader->GetOutputPort());
//         upperSmooth->SetNumberOfIterations (30);
//         upperSmooth->SetRelaxationFactor (0.1);
//         upperSmooth->BoundarySmoothingOn();
//         upperCutter->SetInputConnection (upperSmooth->GetOutputPort());
//     }
//     else if (CurrentSmoothMode == SmoothMode::Normals)
//     {
//         // normals doesn't change geometry so cutter from reader is ok
//         upperCutter->SetInputConnection (upperReader->GetOutputPort());
//     }
//     else
//     {
//         upperCutter->SetInputConnection (upperReader->GetOutputPort());
//     }

//     auto lowerCutter = vtkSmartPointer<vtkCutter>::New();
//     lowerCutter->SetCutFunction (Plane);
//     if (CurrentSmoothMode == SmoothMode::Geometry)
//     {
//         auto lowerSmooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//         lowerSmooth->SetInputConnection (lowerReader->GetOutputPort());
//         lowerSmooth->SetNumberOfIterations (30);
//         lowerSmooth->SetRelaxationFactor (0.1);
//         lowerSmooth->BoundarySmoothingOn();
//         lowerCutter->SetInputConnection (lowerSmooth->GetOutputPort());
//     }
//     else
//     {
//         lowerCutter->SetInputConnection (lowerReader->GetOutputPort());
//     }

//     // ---------- spline filter for cut curve smoothing ----------
//     auto upperSpline = vtkSmartPointer<vtkSplineFilter>::New();
//     upperSpline->SetInputConnection (upperCutter->GetOutputPort());
//     upperSpline->SetSubdivideToLength();
//     upperSpline->SetLength (1.0);

//     auto lowerSpline = vtkSmartPointer<vtkSplineFilter>::New();
//     lowerSpline->SetInputConnection (lowerCutter->GetOutputPort());
//     lowerSpline->SetSubdivideToLength();
//     lowerSpline->SetLength (1.0);

//     // ---------- actors ----------
//     auto upperActor = vtkSmartPointer<vtkActor>::New();
//     upperActor->SetMapper (upperMapper);
//     upperActor->GetProperty()->SetColor (0.8, 0.8, 0.8);
//     upperActor->GetProperty()->SetOpacity (0.75);

//     auto lowerActor = vtkSmartPointer<vtkActor>::New();
//     lowerActor->SetMapper (lowerMapper);
//     lowerActor->GetProperty()->SetColor (0.6, 0.8, 1.0);
//     lowerActor->GetProperty()->SetOpacity (0.75);

//     auto upperCutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     upperCutMapper->SetInputConnection (upperSpline->GetOutputPort());
//     auto upperCutActor = vtkSmartPointer<vtkActor>::New();
//     upperCutActor->SetMapper (upperCutMapper);
//     upperCutActor->GetProperty()->SetColor (1, 0, 0);
//     upperCutActor->GetProperty()->SetLineWidth (2);
//     upperCutActor->GetProperty()->LightingOff(); // avoid black lines

//     auto lowerCutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     lowerCutMapper->SetInputConnection (lowerSpline->GetOutputPort());
//     auto lowerCutActor = vtkSmartPointer<vtkActor>::New();
//     lowerCutActor->SetMapper (lowerCutMapper);
//     lowerCutActor->GetProperty()->SetColor (0, 0.6, 0);
//     lowerCutActor->GetProperty()->SetLineWidth (2);
//     lowerCutActor->GetProperty()->LightingOff();

//     // ---------- renderers ----------
//     RenLeft = vtkSmartPointer<vtkRenderer>::New();
//     RenLeft->SetViewport (0.0, 0.0, 0.66, 1.0);
//     RenLeft->SetBackground (0.15, 0.15, 0.15);
//     RenLeft->AddActor (upperActor);
//     RenLeft->AddActor (lowerActor);
//     RenLeft->AddActor (upperCutActor);
//     RenLeft->AddActor (lowerCutActor);

//     RenRight = vtkSmartPointer<vtkRenderer>::New();
//     RenRight->SetViewport (0.66, 0.0, 1.0, 1.0);
//     RenRight->SetBackground (1.0, 1.0, 1.0);
//     RenRight->AddActor (upperCutActor);
//     RenRight->AddActor (lowerCutActor);
//     RenRight->GetActiveCamera()->ParallelProjectionOn();

//     // ---------- render window ----------
//     RenWin = vtkSmartPointer<vtkRenderWindow>::New();
//     RenWin->SetSize (1000, 600);
//     RenWin->AddRenderer (RenLeft);
//     RenWin->AddRenderer (RenRight);

//     // interactor
//     Iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//     Iren->SetRenderWindow (RenWin);

//     // interactor style (for rotating the plane)
//     auto style = vtkSmartPointer<PlaneRotateInteractorStyle>::New();
//     style->SetPlane (Plane);
//     style->SetRenderWindow (RenWin);
//     style->SetRendererRight (RenRight);
//     style->SetCenter (Center[0], Center[1], Center[2]);
//     style->SetSensitivity (0.6);
//     style->SetAngleLimits (0, 180);
//     Iren->SetInteractorStyle (style);

//     // set initial left camera to focus center
//     RenLeft->ResetCamera();
//     auto cam = RenLeft->GetActiveCamera();
//     cam->SetFocalPoint (Center);
//     RenLeft->ResetCameraClippingRange();
// }

// void PlaneSliceViewer::Run()
// {
//     SetupPipeline();
//     RenLeft->ResetCamera();
//     RenWin->Render();
//     Iren->Start(); // blocking
// }

// // --------- camera controls ----------
// void PlaneSliceViewer::ZoomIn()
// {
//     if (!RenLeft) return;
//     RenLeft->GetActiveCamera()->Zoom (1.2);
//     RenWin->Render();
// }

// void PlaneSliceViewer::ZoomOut()
// {
//     if (!RenLeft) return;
//     RenLeft->GetActiveCamera()->Zoom (0.8);
//     RenWin->Render();
// }

// void PlaneSliceViewer::ResetView()
// {
//     if (!RenLeft) return;
//     RenLeft->ResetCamera();
//     RenLeft->ResetCameraClippingRange();
//     RenWin->Render();
// }

// void PlaneSliceViewer::SetViewDirection (const std::string& dir)
// {
//     if (!RenLeft) return;
//     auto cam = RenLeft->GetActiveCamera();
//     double fp[3];
//     cam->GetFocalPoint (fp);
//     double distance = cam->GetDistance();
//     if (distance <= 0) distance = 200.0;

//     if (dir == "front") cam->SetPosition (fp[0], fp[1], fp[2] + distance);
//     else if (dir == "back") cam->SetPosition (fp[0], fp[1], fp[2] - distance);
//     else if (dir == "left") cam->SetPosition (fp[0] - distance, fp[1], fp[2]);
//     else if (dir == "right") cam->SetPosition (fp[0] + distance, fp[1], fp[2]);
//     else if (dir == "top") cam->SetPosition (fp[0], fp[1] + distance, fp[2]);
//     else if (dir == "bottom") cam->SetPosition (fp[0], fp[1] - distance, fp[2]);

//     cam->SetViewUp (0, 1, 0);
//     RenLeft->ResetCameraClippingRange();
//     RenWin->Render();
// }

// // --------- smoothing ----------
// void PlaneSliceViewer::SetSmoothMode (SmoothMode mode)
// {
//     CurrentSmoothMode = mode;
//     // Rebuild pipeline to reflect new smoothing mode
//     // Stop existing interactor if running, then rebuild
//     if (Iren && Iren->GetRenderWindow())
//     {
//         // It's difficult to stop a running interactor cleanly from here if blocking.
//         // For simple usage (call SetSmoothMode before Run), we rebuild here.
//     }
// }

// PlaneSliceViewer::SmoothMode PlaneSliceViewer::GetSmoothMode() const { return CurrentSmoothMode; }

// // --------- measurement tools ----------
// void PlaneSliceViewer::StartMeasureDistance()
// {
//     if (!Iren) return;
//     ClearMeasurements();

//     DistanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
//     DistanceWidget->SetInteractor (Iren);

//     auto rep = vtkSmartPointer<vtkDistanceRepresentation3D>::New();
//     rep->SetLabelFormat ("%.2f mm");
//     rep->SetLabelScale (5.0, 5.0, 5.0);

//     rep->GetLineProperty()->SetLineWidth (4.0);
//     rep->GetLineProperty()->SetColor (1.0, 0.0, 0.0);

//     DistanceWidget->SetRepresentation (rep);
//     DistanceWidget->CreateDefaultRepresentation();
//     DistanceWidget->On();
// }

// void PlaneSliceViewer::StartMeasureAngle()
// {
//     if (!Iren) return;
//     ClearMeasurements();

//     AngleWidget = vtkSmartPointer<vtkAngleWidget>::New();
//     AngleWidget->SetInteractor (Iren);

//     auto rep = vtkSmartPointer<vtkAngleRepresentation3D>::New();
//     rep->SetLabelFormat ("%.2f °");
//     // rep->SetLabelScale (5.0, 5.0, 5.0);

//     // set ray/arc widths/colors
//     rep->GetRay1()->GetProperty()->SetLineWidth (3.0);
//     rep->GetRay1()->GetProperty()->SetColor (0.0, 1.0, 0.0);

//     rep->GetRay2()->GetProperty()->SetLineWidth (3.0);
//     rep->GetRay2()->GetProperty()->SetColor (0.0, 1.0, 0.0);

//     rep->GetArc()->GetProperty()->SetLineWidth (2.5);
//     rep->GetArc()->GetProperty()->SetColor (1.0, 0.0, 0.0);

//     AngleWidget->SetRepresentation (rep);
//     AngleWidget->CreateDefaultRepresentation();
//     AngleWidget->On();
// }

// void PlaneSliceViewer::ClearMeasurements()
// {
//     if (DistanceWidget)
//     {
//         DistanceWidget->Off();
//         DistanceWidget = nullptr;
//     }
//     if (AngleWidget)
//     {
//         AngleWidget->Off();
//         AngleWidget = nullptr;
//     }
//     if (RenWin) RenWin->Render();
// }
