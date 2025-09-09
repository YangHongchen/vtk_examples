#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <cmath>

// 根据目标模型计算源模型点颜色
void ColorByContact (vtkSmartPointer<vtkPolyData> source, vtkSmartPointer<vtkPolyData> target,
                     double tol = 0.2)
{
    auto colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents (3);
    colors->SetName ("ContactColors");

    auto dist = vtkSmartPointer<vtkImplicitPolyDataDistance>::New();
    dist->SetInput (target);

    for (vtkIdType i = 0; i < source->GetNumberOfPoints(); ++i)
    {
        double p[3];
        source->GetPoint (i, p);
        double d = dist->EvaluateFunction (p);

        unsigned char color[3];
        if (d < -tol)        { color[0] = 255; color[1] = 0;   color[2] = 0;   } // 红色：穿插
        else if (std::fabs (d) <= tol) { color[0] = 0;   color[1] = 255; color[2] = 0; } // 绿色：接触
        else                  { color[0] = 0;   color[1] = 0;   color[2] = 255; } // 蓝色：未接触

        colors->InsertNextTypedTuple (color);
    }

    source->GetPointData()->SetScalars (colors);
}

int main (int argc, char* argv[])
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    // ---------------------------
    // 1. 读取上下颌 STL
    // ---------------------------
    auto upperReader = vtkSmartPointer<vtkSTLReader>::New();
    upperReader->SetFileName ("c:/stl/1.stl");
    upperReader->Update();
    auto upper = upperReader->GetOutput();

    auto lowerReader = vtkSmartPointer<vtkSTLReader>::New();
    lowerReader->SetFileName ("c:/stl/2.stl");
    lowerReader->Update();
    auto lower = lowerReader->GetOutput();

    // ---------------------------
    // 2. 计算碰撞面颜色
    // ---------------------------
    ColorByContact (lower, upper, 0.2); // 下颌点到上颌
    ColorByContact (upper, lower, 0.2); // 上颌点到下颌

    // ---------------------------
    // 3. 创建Mapper和Actor
    // ---------------------------
    auto upperMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    upperMapper->SetInputData (upper);
    auto upperActor = vtkSmartPointer<vtkActor>::New();
    upperActor->SetMapper (upperMapper);
    upperActor->GetProperty()->SetOpacity (1.0);
    upperActor->SetPosition (0, 0, 30); // 上颌向上移动

    // 创建下颌的变换（旋转180度以显示内侧面）
    auto lowerTransform = vtkSmartPointer<vtkTransform>::New();
    lowerTransform->RotateY (180); // 绕Y轴旋转180度

    auto lowerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    lowerMapper->SetInputData (lower);
    auto lowerActor = vtkSmartPointer<vtkActor>::New();
    lowerActor->SetMapper (lowerMapper);
    lowerActor->GetProperty()->SetOpacity (1.0);
    lowerActor->SetPosition (0, 0, -30); // 下颌向下移动
    lowerActor->SetUserTransform (lowerTransform); // 应用旋转变换

    // ---------------------------
    // 4. 添加标签说明
    // ---------------------------
    auto upperText = vtkSmartPointer<vtkTextActor>::New();
    upperText->SetInput ("上颌 (外侧)");
    upperText->GetTextProperty()->SetColor (colors->GetColor3d ("Black").GetData());
    upperText->GetTextProperty()->SetFontSize (20);
    upperText->SetPosition (10, 10);

    auto lowerText = vtkSmartPointer<vtkTextActor>::New();
    lowerText->SetInput ("下颌 (内侧)");
    lowerText->GetTextProperty()->SetColor (colors->GetColor3d ("Black").GetData());
    lowerText->GetTextProperty()->SetFontSize (20);
    lowerText->SetPosition (10, 40);

    // ---------------------------
    // 5. 渲染设置
    // ---------------------------
    auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetSize (800, 800);
    renWin->SetWindowName ("上下颌咬合分析 - 内侧视图");

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground (colors->GetColor3d ("White").GetData());
    renderer->AddActor (upperActor);
    renderer->AddActor (lowerActor);
    renderer->AddActor2D (upperText);
    renderer->AddActor2D (lowerText);

    renWin->AddRenderer (renderer);

    auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow (renWin);

    // ---------------------------
    // 6. 相机设置
    // ---------------------------
    auto camera = renderer->GetActiveCamera();
    camera->SetPosition (0, -150, 0); // 从前方观察
    camera->SetFocalPoint (0, 0, 0);
    camera->SetViewUp (0, 0, 1);
    camera->ParallelProjectionOn();
    renderer->ResetCameraClippingRange();

    // ---------------------------
    // 7. 交互设置
    // ---------------------------
    auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren->SetInteractorStyle (style);

    // ---------------------------
    // 8. 渲染
    // ---------------------------
    renWin->Render();
    iren->Start();

    return 0;
}
