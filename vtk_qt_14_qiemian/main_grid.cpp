// #include <vtkSmartPointer.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>
// #include <vtkLineSource.h>
// #include <vtkAppendPolyData.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>
// #include <vtkProperty.h>
// #include <cmath>

// // ========== 网格函数 ==========
// struct GridActors
// {
//     vtkSmartPointer<vtkActor> minor; // 细格线
//     vtkSmartPointer<vtkActor> major; // 主格线
// };

// static GridActors CreateXYGrid (double xmin, double xmax,
//                                 double ymin, double ymax,
//                                 double planeZ = 0.0,
//                                 double minorSpacing = 1.0,
//                                 int majorEvery = 10)
// {
//     const double zMinor = planeZ - 0.02;
//     const double zMajor = planeZ - 0.01;

//     auto appendMinor = vtkSmartPointer<vtkAppendPolyData>::New();
//     auto appendMajor = vtkSmartPointer<vtkAppendPolyData>::New();

//     auto ceilTo = [] (double v, double s) { return std::ceil (v / s) * s; };
//     auto floorTo = [] (double v, double s) { return std::floor (v / s) * s; };

//     xmin = ceilTo (xmin, minorSpacing);
//     ymin = ceilTo (ymin, minorSpacing);
//     xmax = floorTo (xmax, minorSpacing);
//     ymax = floorTo (ymax, minorSpacing);

//     // 垂直线
//     for (double x = xmin; x <= xmax + 1e-9; x += minorSpacing)
//     {
//         bool isMajor = (std::llround (x) % (majorEvery) == 0);
//         auto ln = vtkSmartPointer<vtkLineSource>::New();
//         ln->SetPoint1 (x, ymin, isMajor ? zMajor : zMinor);
//         ln->SetPoint2 (x, ymax, isMajor ? zMajor : zMinor);
//         (isMajor ? appendMajor : appendMinor)->AddInputConnection (ln->GetOutputPort());
//     }

//     // 水平线
//     for (double y = ymin; y <= ymax + 1e-9; y += minorSpacing)
//     {
//         bool isMajor = (std::llround (y) % (majorEvery) == 0);
//         auto ln = vtkSmartPointer<vtkLineSource>::New();
//         ln->SetPoint1 (xmin, y, isMajor ? zMajor : zMinor);
//         ln->SetPoint2 (xmax, y, isMajor ? zMajor : zMinor);
//         (isMajor ? appendMajor : appendMinor)->AddInputConnection (ln->GetOutputPort());
//     }

//     appendMinor->Update();
//     appendMajor->Update();

//     auto mapperMinor = vtkSmartPointer<vtkPolyDataMapper>::New();
//     mapperMinor->SetInputConnection (appendMinor->GetOutputPort());
//     auto mapperMajor = vtkSmartPointer<vtkPolyDataMapper>::New();
//     mapperMajor->SetInputConnection (appendMajor->GetOutputPort());

//     auto actorMinor = vtkSmartPointer<vtkActor>::New();
//     actorMinor->SetMapper (mapperMinor);
//     actorMinor->GetProperty()->SetColor (0.8, 0.8, 0.85);
//     actorMinor->GetProperty()->SetLineWidth (1.0);

//     auto actorMajor = vtkSmartPointer<vtkActor>::New();
//     actorMajor->SetMapper (mapperMajor);
//     actorMajor->GetProperty()->SetColor (0.45, 0.45, 0.55);
//     actorMajor->GetProperty()->SetLineWidth (1.0);

//     return { actorMinor, actorMajor };
// }

// // ========== main 方法 ==========
// int main (int, char *[])
// {
//     // 1. 创建渲染器
//     auto renderer = vtkSmartPointer<vtkRenderer>::New();
//     renderer->SetBackground (1.0, 1.0, 1.0); // 白色背景

//     // 2. 创建窗口 + 交互器
//     auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//     renderWindow->AddRenderer (renderer);

//     auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//     interactor->SetRenderWindow (renderWindow);

//     // 3. 添加网格 (300 x 200 mm，位于 XY 平面 z=0)
//     auto grids = CreateXYGrid (-150, 150, -100, 100, 0.0, 1.0, 10);
//     renderer->AddActor (grids.minor);
//     renderer->AddActor (grids.major);

//     // 4. 初始化并启动
//     renderWindow->SetSize (800, 600);
//     renderWindow->Render();
//     interactor->Start();

//     return 0;
// }
