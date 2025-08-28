// plane_rotate_slice_dual.cpp
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleUser.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkMath.h>

#include <cmath>
#include <algorithm>
#include <iostream>

// ========== 自定义交互：旋转切面 ==========
class PlaneRotateInteractorStyle : public vtkInteractorStyleUser {
  public:
    static PlaneRotateInteractorStyle *New();
    vtkTypeMacro (PlaneRotateInteractorStyle, vtkInteractorStyleUser);

    PlaneRotateInteractorStyle()
    {
        this->Plane = nullptr;
        this->RenderWindow = nullptr;
        this->RenRight = nullptr;
        this->Dragging = false;
        this->LastY = 0;
        this->AngleDeg = 0.0;
        this->Sensitivity = 0.3;
        this->MinAngle = 0.0;
        this->MaxAngle = 180.0;
        this->Center[0] = this->Center[1] = this->Center[2] = 0.0;
    }

    void SetPlane (vtkPlane* p) { this->Plane = p; }
    void SetRenderWindow (vtkRenderWindow* rw) { this->RenderWindow = rw; }
    void SetRendererRight (vtkRenderer* right) { this->RenRight = right; }
    void SetCenter (double cx, double cy, double cz) { this->Center[0] = cx; this->Center[1] = cy; this->Center[2] = cz; }
    void SetSensitivity (double s) { this->Sensitivity = s; }
    void SetAngleLimits (double minA, double maxA) { this->MinAngle = minA; this->MaxAngle = maxA; }

    virtual void OnLeftButtonDown() override
    {
        this->Dragging = true;
        int x, y;
        this->GetInteractor()->GetEventPosition (x, y);
        this->LastY = y;
    }

    virtual void OnMouseMove() override
    {
        if (!this->Dragging || !this->Plane || !this->RenderWindow || !this->RenRight)
            return;

        int x, y;
        this->GetInteractor()->GetEventPosition (x, y);
        int dy = y - this->LastY;
        this->LastY = y;

        double deltaDeg = -dy * this->Sensitivity;
        this->AngleDeg += deltaDeg;
        if (this->AngleDeg < this->MinAngle) this->AngleDeg = this->MinAngle;
        if (this->AngleDeg > this->MaxAngle) this->AngleDeg = this->MaxAngle;

        // 绕 Y 轴旋转
        double rad = vtkMath::RadiansFromDegrees (this->AngleDeg);
        double nx = std::sin (rad);
        double ny = 0.0;
        double nz = std::cos (rad);

        this->Plane->SetNormal (nx, ny, nz);
        this->Plane->SetOrigin (this->Center);

        // 调整右侧相机
        vtkCamera* cam = this->RenRight->GetActiveCamera();
        cam->SetFocalPoint (this->Center);
        cam->SetPosition (this->Center[0] + nx * 200, this->Center[1] + ny * 200, this->Center[2] + nz * 200);
        cam->ParallelProjectionOn();
        cam->SetParallelScale (50);
        cam->SetViewUp (0.0, 1.0, 0.0);
        this->RenRight->ResetCameraClippingRange();

        this->RenderWindow->Render();
    }

    virtual void OnLeftButtonUp() override
    {
        this->Dragging = false;
    }

  private:
    vtkPlane *Plane;
    vtkRenderWindow *RenderWindow;
    vtkRenderer *RenRight;
    bool Dragging;
    int LastY;
    double AngleDeg;
    double Sensitivity;
    double MinAngle, MaxAngle;
    double Center[3];
};

vtkStandardNewMacro (PlaneRotateInteractorStyle);

// ========== main ==========
int main (int argc, char* argv[])
{
    std::string upperFile = "c:/stl/1.stl";
    std::string lowerFile = "c:/stl/2.stl";

    // 读取 STL
    auto upperReader = vtkSmartPointer<vtkSTLReader>::New();
    upperReader->SetFileName (upperFile.c_str());
    upperReader->Update();

    auto lowerReader = vtkSmartPointer<vtkSTLReader>::New();
    lowerReader->SetFileName (lowerFile.c_str());
    lowerReader->Update();

    // 计算整体中心
    double b1[6], b2[6];
    upperReader->GetOutput()->GetBounds (b1);
    lowerReader->GetOutput()->GetBounds (b2);
    double centerX = 0.5 * (std::min (b1[0], b2[0]) + std::max (b1[1], b2[1]));
    double centerY = 0.5 * (std::min (b1[2], b2[2]) + std::max (b1[3], b2[3]));
    double centerZ = 0.5 * (std::min (b1[4], b2[4]) + std::max (b1[5], b2[5]));

    // 定义切平面
    auto plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin (centerX, centerY, centerZ);
    plane->SetNormal (0.0, 0.0, 1.0);

    // cutter 上颌
    auto upperCutter = vtkSmartPointer<vtkCutter>::New();
    upperCutter->SetCutFunction (plane);
    upperCutter->SetInputConnection (upperReader->GetOutputPort());

    // cutter 下颌
    auto lowerCutter = vtkSmartPointer<vtkCutter>::New();
    lowerCutter->SetCutFunction (plane);
    lowerCutter->SetInputConnection (lowerReader->GetOutputPort());

    // 上颌模型
    auto upperMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    upperMapper->SetInputConnection (upperReader->GetOutputPort());
    auto upperActor = vtkSmartPointer<vtkActor>::New();
    upperActor->SetMapper (upperMapper);
    upperActor->GetProperty()->SetOpacity (0.35);
    upperActor->GetProperty()->SetColor (0.8, 0.8, 0.8);

    // 下颌模型
    auto lowerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    lowerMapper->SetInputConnection (lowerReader->GetOutputPort());
    auto lowerActor = vtkSmartPointer<vtkActor>::New();
    lowerActor->SetMapper (lowerMapper);
    lowerActor->GetProperty()->SetOpacity (0.35);
    lowerActor->GetProperty()->SetColor (0.6, 0.8, 1.0);

    // 切线 - 上颌
    auto upperCutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    upperCutMapper->SetInputConnection (upperCutter->GetOutputPort());
    auto upperCutActor = vtkSmartPointer<vtkActor>::New();
    upperCutActor->SetMapper (upperCutMapper);
    upperCutActor->GetProperty()->SetColor (1, 0, 0);
    upperCutActor->GetProperty()->SetLineWidth (3);

    // 切线 - 下颌
    auto lowerCutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    lowerCutMapper->SetInputConnection (lowerCutter->GetOutputPort());
    auto lowerCutActor = vtkSmartPointer<vtkActor>::New();
    lowerCutActor->SetMapper (lowerCutMapper);
    lowerCutActor->GetProperty()->SetColor (0, 0.6, 0);
    lowerCutActor->GetProperty()->SetLineWidth (3);

    // 左侧渲染 (3D)
    auto renLeft = vtkSmartPointer<vtkRenderer>::New();
    renLeft->SetViewport (0.0, 0.0, 0.66, 1.0);
    renLeft->SetBackground (0.15, 0.15, 0.15);
    renLeft->AddActor (upperActor);
    renLeft->AddActor (lowerActor);
    renLeft->AddActor (upperCutActor);
    renLeft->AddActor (lowerCutActor);

    // 右侧渲染 (2D 切线投影)
    auto renRight = vtkSmartPointer<vtkRenderer>::New();
    renRight->SetViewport (0.66, 0.0, 1.0, 1.0);
    renRight->SetBackground (1, 1, 1);
    renRight->AddActor (upperCutActor);
    renRight->AddActor (lowerCutActor);
    renRight->GetActiveCamera()->ParallelProjectionOn();

    // 渲染窗口
    auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetSize (1000, 600);
    renWin->AddRenderer (renLeft);
    renWin->AddRenderer (renRight);

    auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow (renWin);

    // 交互样式
    auto style = vtkSmartPointer<PlaneRotateInteractorStyle>::New();
    style->SetPlane (plane);
    style->SetRenderWindow (renWin);
    style->SetRendererRight (renRight);
    style->SetCenter (centerX, centerY, centerZ);
    style->SetSensitivity (0.4);
    style->SetAngleLimits (0, 180);
    iren->SetInteractorStyle (style);

    // 启动
    renLeft->ResetCamera();
    renWin->Render();
    iren->Start();

    return EXIT_SUCCESS;
}
