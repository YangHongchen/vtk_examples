// // plane_rotate_slice.cpp
// #include <vtkSmartPointer.h>
// #include <vtkSTLReader.h>
// #include <vtkPlane.h>
// #include <vtkCutter.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>
// #include <vtkInteractorStyleUser.h>
// #include <vtkCamera.h>
// #include <vtkProperty.h>
// #include <vtkObjectFactory.h>
// #include <vtkMath.h>

// #include <cmath>
// #include <algorithm>
// #include <iostream>

// #include <QDebug>

// class PlaneRotateInteractorStyle : public vtkInteractorStyleUser {
//   public:
//     static PlaneRotateInteractorStyle *New();
//     vtkTypeMacro (PlaneRotateInteractorStyle, vtkInteractorStyleUser);

//     PlaneRotateInteractorStyle()
//     {
//         this->Plane = nullptr;
//         this->Cutter = nullptr;
//         this->RenderWindow = nullptr;
//         this->RenLeft = nullptr;
//         this->RenRight = nullptr;
//         this->Dragging = false;
//         this->LastY = 0;
//         this->AngleDeg = 0.0;
//         this->Sensitivity = 0.3; // degree per pixel, 可调
//         this->MinAngle = 0.0;
//         this->MaxAngle = 180.0;
//         this->Center[0] = this->Center[1] = this->Center[2] = 0.0;
//     }

//     void SetPlane (vtkPlane* p) { this->Plane = p; }
//     void SetCutter (vtkCutter* c) { this->Cutter = c; }
//     void SetRenderWindow (vtkRenderWindow* rw) { this->RenderWindow = rw; }
//     void SetRenderers (vtkRenderer* left, vtkRenderer* right) { this->RenLeft = left; this->RenRight = right; }
//     void SetCenter (double cx, double cy, double cz) { this->Center[0] = cx; this->Center[1] = cy; this->Center[2] = cz; }
//     void SetSensitivity (double s) { this->Sensitivity = s; }
//     void SetAngleLimits (double minA, double maxA) { this->MinAngle = minA; this->MaxAngle = maxA; }

//     // 鼠标左键按下：开始拖拽
//     virtual void OnLeftButtonDown() override
//     {
//         this->Dragging = true;
//         int x, y;
//         this->GetInteractor()->GetEventPosition (x, y);
//         this->LastY = y;
//         // 不调用父类，避免默认的相机交互
//     }

//     // 鼠标移动：在拖拽状态下更新角度（基于竖直移动）
//     virtual void OnMouseMove() override
//     {
//         if (!this->Dragging || !this->Plane || !this->Cutter || !this->RenderWindow || !this->RenRight)
//         {
//             return;
//         }

//         int x, y;
//         this->GetInteractor()->GetEventPosition (x, y);
//         int dy = y - this->LastY;
//         this->LastY = y;

//         // 将像素偏移映射到角度增量
//         double deltaDeg = -dy * this->Sensitivity; // 向上拖动（dy<0） -> 增加角度
//         this->AngleDeg += deltaDeg;
//         if (this->AngleDeg < this->MinAngle) this->AngleDeg = this->MinAngle;
//         if (this->AngleDeg > this->MaxAngle) this->AngleDeg = this->MaxAngle;

//         // 计算新的法向量（原法向为 (0,0,1)，绕 Y 轴旋转 AngleDeg）
//         double rad = vtkMath::RadiansFromDegrees (this->AngleDeg);
//         double nx = std::sin (rad);
//         double ny = 0.0;
//         double nz = std::cos (rad);

//         this->Plane->SetNormal (nx, ny, nz);
//         this->Plane->SetOrigin (this->Center);

//         // 更新 cutter 并刷新 mapper
//         this->Cutter->Update();

//         // 调整右侧正交相机，使切面完整可见
//         double cbounds[6];
//         this->Cutter->GetOutput()->GetBounds (cbounds);
//         // 如果 cutter 没有输出（切不到），使用模型中心与适当尺度
//         double dx = cbounds[1] - cbounds[0];
//         double dyb = cbounds[3] - cbounds[2];
//         if (dx <= 0.0) dx = 1.0;
//         if (dyb <= 0.0) dyb = 1.0;
//         double maxSize = std::max (dx, dyb);

//         vtkCamera* cam = this->RenRight->GetActiveCamera();
//         double cx = 0.5 * (cbounds[0] + cbounds[1]);
//         double cy = 0.5 * (cbounds[2] + cbounds[3]);
//         double cz = 0.5 * (cbounds[4] + cbounds[5]);

//         // 若 bounds 不合理（例如全 0），回退到 plane center
//         if (!std::isfinite (cx) || !std::isfinite (cy) || !std::isfinite (cz))
//         {
//             cx = this->Center[0]; cy = this->Center[1]; cz = this->Center[2];
//             maxSize = 100.0; // 默认尺度
//         }

//         // Right camera 看向 plane 的法线方向位置
//         double camDist = std::max (maxSize, 1.0) * 1.5; // 距离尺度（可调）
//         cam->SetFocalPoint (cx, cy, cz);
//         cam->SetPosition (cx + nx * camDist, cy + ny * camDist, cz + nz * camDist);
//         cam->ParallelProjectionOn();
//         cam->SetParallelScale (20); // 使切面完整可见（0.5 为半高，这里乘一点 padding）
//         cam->SetViewUp (0.0, 1.0, 0.0);
//         this->RenRight->ResetCameraClippingRange();

//         // 最终渲染
//         this->RenderWindow->Render();
//     }

//     // 鼠标左键释放：结束拖拽
//     virtual void OnLeftButtonUp() override
//     {
//         this->Dragging = false;
//     }

//   protected:
//     ~PlaneRotateInteractorStyle() override = default;

//   private:
//     vtkPlane *Plane;
//     vtkCutter *Cutter;
//     vtkRenderWindow *RenderWindow;
//     vtkRenderer *RenLeft;
//     vtkRenderer *RenRight;

//     bool Dragging;
//     int LastY;
//     double AngleDeg;
//     double Sensitivity;
//     double MinAngle, MaxAngle;
//     double Center[3];
// };

// vtkStandardNewMacro (PlaneRotateInteractorStyle);

// // ----------------------- main -----------------------
// int main (int argc, char* argv[])
// {


//     std::string stlFile = "c:/stl/1.stl";

//     // 读取 STL
//     auto reader = vtkSmartPointer<vtkSTLReader>::New();
//     reader->SetFileName (stlFile.c_str());
//     reader->Update();

//     // 计算模型中心
//     double bounds[6];
//     reader->GetOutput()->GetBounds (bounds);
//     double centerX = 0.5 * (bounds[0] + bounds[1]);
//     double centerY = 0.5 * (bounds[2] + bounds[3]);
//     double centerZ = 0.5 * (bounds[4] + bounds[5]);
//     std::cout << "Model bounds: "
//               << bounds[0] << "," << bounds[1] << " | "
//               << bounds[2] << "," << bounds[3] << " | "
//               << bounds[4] << "," << bounds[5] << std::endl;

//     // 切平面：初始在模型中心，法线指向 +Z
//     auto plane = vtkSmartPointer<vtkPlane>::New();
//     plane->SetOrigin (centerX, centerY, centerZ);
//     plane->SetNormal (0.0, 0.0, 1.0);

//     // Cutter 提取切线
//     auto cutter = vtkSmartPointer<vtkCutter>::New();
//     cutter->SetCutFunction (plane);
//     cutter->SetInputConnection (reader->GetOutputPort());
//     cutter->Update();

//     // 模型 actor（半透明）
//     auto stlMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     stlMapper->SetInputConnection (reader->GetOutputPort());

//     auto stlActor = vtkSmartPointer<vtkActor>::New();
//     stlActor->SetMapper (stlMapper);
//     stlActor->GetProperty()->SetOpacity (0.35);

//     // 切线在左侧的 actor
//     auto cutMapperLeft = vtkSmartPointer<vtkPolyDataMapper>::New();
//     cutMapperLeft->SetInputConnection (cutter->GetOutputPort());

//     auto cutActorLeft = vtkSmartPointer<vtkActor>::New();
//     cutActorLeft->SetMapper (cutMapperLeft);
//     cutActorLeft->GetProperty()->SetColor (1.0, 0.0, 0.0);
//     cutActorLeft->GetProperty()->SetLineWidth (3.0);

//     // 切线在右侧（2D）的 actor（独立 mapper）
//     auto cutMapperRight = vtkSmartPointer<vtkPolyDataMapper>::New();
//     cutMapperRight->SetInputConnection (cutter->GetOutputPort());

//     auto cutActorRight = vtkSmartPointer<vtkActor>::New();
//     cutActorRight->SetMapper (cutMapperRight);
//     cutActorRight->GetProperty()->SetColor (0.0, 0.6, 0.0);
//     cutActorRight->GetProperty()->SetLineWidth (2.0);

//     // 左侧 renderer (3D)
//     auto renLeft = vtkSmartPointer<vtkRenderer>::New();
//     renLeft->AddActor (stlActor);
//     renLeft->AddActor (cutActorLeft);
//     renLeft->SetBackground (0.12, 0.12, 0.12);
//     renLeft->SetViewport (0.0, 0.0, 0.66, 1.0);

//     // 右侧 renderer (2D)
//     auto renRight = vtkSmartPointer<vtkRenderer>::New();
//     renRight->AddActor (cutActorRight);
//     renRight->SetBackground (1.0, 1.0, 1.0);
//     renRight->SetViewport (0.66, 0.0, 1.0, 1.0);

//     // 右侧相机初始设置（正交）
//     renRight->GetActiveCamera()->ParallelProjectionOn();

//     // 渲染窗口与交互器
//     auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
//     renWin->AddRenderer (renLeft);
//     renWin->AddRenderer (renRight);
//     renWin->SetSize (1000, 600);

//     auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//     iren->SetRenderWindow (renWin);

//     // 自定义交互器（只处理切面角度）
//     auto style = vtkSmartPointer<PlaneRotateInteractorStyle>::New();
//     style->SetPlane (plane);
//     style->SetCutter (cutter);
//     style->SetRenderWindow (renWin);
//     style->SetRenderers (renLeft, renRight);
//     style->SetCenter (centerX, centerY, centerZ);
//     style->SetSensitivity (0.45);      // 可调：灵敏度（度 / 像素）
//     style->SetAngleLimits (0.0, 180.0); // 限制角度范围
//     iren->SetInteractorStyle (style);

//     // 首次渲染（确保 cutter 有输出）
//     renWin->Render();

//     // 也为右侧相机做一次合适的初始缩放
//     {
//         double cbounds[6];
//         cutter->GetOutput()->GetBounds (cbounds);
//         double dx = cbounds[1] - cbounds[0];
//         double dy = cbounds[3] - cbounds[2];
//         if (dx <= 0.0) dx = bounds[1] - bounds[0];
//         if (dy <= 0.0) dy = bounds[3] - bounds[2];
//         double maxSize = std::min (dx, dy);
//         vtkCamera* cam = renRight->GetActiveCamera();
//         cam->SetFocalPoint (centerX, centerY, centerZ);
//         cam->SetPosition (centerX, centerY, centerZ + std::max (maxSize, 1.0) * 1.5);
//         cam->ParallelProjectionOn();
//         // qDebug() << "maxSize:" << maxSize;
//         cam->SetParallelScale (20);
//         renRight->ResetCameraClippingRange();
//     }

//     // 启动交互
//     iren->Initialize();
//     iren->Start();

//     return EXIT_SUCCESS;
// }
