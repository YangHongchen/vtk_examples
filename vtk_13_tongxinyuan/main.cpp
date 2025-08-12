#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkPlot.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkChartXY.h>
#include <vtkRenderWindow.h>
#include <vtkPen.h>
#include <vtkAxis.h>
#include <vtkSplineFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPolyLine.h>
#include <vtkTextActor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkTextProperty.h>
#include <vtkImageItem.h>
#include <vtkPNGReader.h>

#include <vector>
#include <utility>

#include <vtkPNGReader.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkImageResize.h>
#include <vtkImageMapper.h>
#include <vtkChartLegend.h>
#include <qDebug>

/**
* @brief generateRightCondyleImage
*/
void generateRightCondyleImage()
{
    int winW = 800, winH = 400; // 你的窗口尺寸

    // 轨迹点 (Y, Z)，单位:mm，起点0,0，向下和向前类直线延伸
    std::vector<std::pair<double, double>> trajectory_adjusted =
    {
        {0.0, 0.0},
        {-1.0, 1.2},
        {-1.5, 2.1},
        {-2.0, 2.5},
        {-2.2, 2.7},
        {-3.0, 3.6},
        {-3.2, 3.7},
        {-3.8, 4.2},
        {-4.5, 5.0},
    };

    // 1. 转成 PolyData，X轴用Z，Y轴用Y
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines = vtkSmartPointer<vtkCellArray>::New();
    auto polyLine = vtkSmartPointer<vtkPolyLine>::New();

    polyLine->GetPointIds()->SetNumberOfIds (static_cast<vtkIdType> (trajectory_adjusted.size()));
    for (vtkIdType i = 0; i < static_cast<vtkIdType> (trajectory_adjusted.size()); ++i)
    {
        const double Y = trajectory_adjusted[static_cast<size_t> (i)].first;
        const double Z = trajectory_adjusted[static_cast<size_t> (i)].second;
        points->InsertNextPoint (Z, Y, 0.0); // 将 Z 映射到 X
        polyLine->GetPointIds()->SetId (i, i); // 索引连续依次连接成折线
    }
    lines->InsertNextCell (polyLine);

    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints (points);
    polyData->SetLines (lines);

    // 2. spline 插值平滑（指定细分数量来提高曲线平滑度）
    auto spline = vtkSmartPointer<vtkSplineFilter>::New();
    spline->SetInputData (polyData);
    spline->SetSubdivideToSpecified();
    spline->SetNumberOfSubdivisions (200);
    spline->Update();

    // 3. 转成 vtkTable (X:Z, Y:Y)
    auto table = vtkSmartPointer<vtkTable>::New();
    auto arrZ = vtkSmartPointer<vtkFloatArray>::New();
    auto arrY = vtkSmartPointer<vtkFloatArray>::New();
    arrZ->SetName ("Z (mm)");
    arrY->SetName ("Y (mm)");
    table->AddColumn (arrZ);
    table->AddColumn (arrY);

    auto pts = spline->GetOutput()->GetPoints();
    const vtkIdType npts = pts->GetNumberOfPoints();
    table->SetNumberOfRows (npts);
    for (vtkIdType i = 0; i < npts; ++i)
    {
        double p[3];
        pts->GetPoint (i, p); // p[0]=Z, p[1]=Y
        table->SetValue (i, 0, p[0]); // X <- Z
        table->SetValue (i, 1, p[1]); // Y <- Y
    }

    // 4. 绘制
    auto view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground (1.0, 1.0, 1.0);
    view->GetRenderWindow()->SetSize (winW, winH);
    view->GetRenderWindow()->SetOffScreenRendering (1);

    auto chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem (chart);
    // 曲线
    auto plotLine = chart->AddPlot (vtkChart::LINE);
    plotLine->SetInputData (table, 0, 1);
    plotLine->SetColor (255, 0, 0, 255);
    plotLine->SetWidth (2.0);

    // 坐标轴与网格（缓存指针，避免重复 GetAxis 调用）
    auto axisZ = chart->GetAxis (vtkAxis::BOTTOM);
    auto axisY = chart->GetAxis (vtkAxis::LEFT);

    // 轴标题按需为空；只保留网格与范围
    axisZ->SetTitle ("");
    axisY->SetTitle ("");

    // 网格（统一配色，避免视觉噪声）
    axisZ->SetGridVisible (true);
    axisY->SetGridVisible (true);
    axisZ->GetGridPen()->SetColor (180, 180, 180);
    axisY->GetGridPen()->SetColor (180, 180, 180);

    // 坐标轴固定范围（不受数据影响）
    axisZ->SetBehavior (vtkAxis::FIXED);
    axisZ->SetRange (-4, 16);
    axisZ->SetNumberOfTicks (11); // 步长约 2 mm

    axisY->SetBehavior (vtkAxis::FIXED);
    axisY->SetRange (-8, 2);
    axisY->SetNumberOfTicks (6); // 步长约 2 mm

    // ================== 加入图例 ================
    // 读取图像
    auto reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName ("skull_right.png");
    reader->Update();
    // 调整到 48x48（保持 1 像素 = 1 显示像素的直觉）
    auto resize = vtkSmartPointer<vtkImageResize>::New();
    resize->SetInputConnection (reader->GetOutputPort());
    resize->SetOutputDimensions (48, 48, 1); // 宽 高 深
    resize->Update();
    // 2D 显示用的图像 mapper（按像素渲染）
    auto imgMapper = vtkSmartPointer<vtkImageMapper>::New();
    imgMapper->SetInputConnection (resize->GetOutputPort());
    // 对于 8-bit 图像，设置窗宽/窗位以避免变灰（RGB/RGBA通常也能直接显示）
    imgMapper->SetColorWindow (255);
    imgMapper->SetColorLevel (127.5);
    // 2D actor：用“显示坐标系”（像素）定位到右上角
    auto logo2D = vtkSmartPointer<vtkActor2D>::New();
    logo2D->SetMapper (imgMapper);
    logo2D->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
    int margin = 20;
    int logoW = 48, logoH = 48;
    logo2D->SetPosition (winW - logoW - margin, winH - logoH - margin);
    // 添加为 2D 覆盖层
    view->GetRenderer()->AddActor2D (logo2D);

    // 渲染并保存图片
    view->GetRenderWindow()->Render();

    auto w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
    w2i->SetInput (view->GetRenderWindow());
    w2i->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName ("right_condyle_YZ_sagittal.png");
    writer->SetInputConnection (w2i->GetOutputPort());
    writer->Write();
}


/**
* @brief generateRightCondyleImage
*/
void generateLeftCondyleImage()
{
    int winW = 800, winH = 400; // 你的窗口尺寸

    // 轨迹点 (Y, Z)，单位:mm，起点0,0，向下和向前类直线延伸
    std::vector<std::pair<double, double>> trajectory_adjusted =
    {
        {0.0, 0.0},
        {-1.0, 1.2},
        {-1.5, 2.1},
        {-2.0, 2.5},
        {-2.2, 2.7},
        {-3.0, 3.6},
        {-3.2, 3.7},
        {-3.8, 4.2},
        {-4.5, 5.0},
    };

    // 1. 转成 PolyData，X轴用Z，Y轴用Y
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines = vtkSmartPointer<vtkCellArray>::New();
    auto polyLine = vtkSmartPointer<vtkPolyLine>::New();

    polyLine->GetPointIds()->SetNumberOfIds (static_cast<vtkIdType> (trajectory_adjusted.size()));
    for (vtkIdType i = 0; i < static_cast<vtkIdType> (trajectory_adjusted.size()); ++i)
    {
        const double Y = trajectory_adjusted[static_cast<size_t> (i)].first;
        const double Z = trajectory_adjusted[static_cast<size_t> (i)].second;
        points->InsertNextPoint (Z, Y, 0.0); // 将 Z 映射到 X
        polyLine->GetPointIds()->SetId (i, i); // 索引连续依次连接成折线
    }
    lines->InsertNextCell (polyLine);

    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints (points);
    polyData->SetLines (lines);

    // 2. spline 插值平滑（指定细分数量来提高曲线平滑度）
    auto spline = vtkSmartPointer<vtkSplineFilter>::New();
    spline->SetInputData (polyData);
    spline->SetSubdivideToSpecified();
    spline->SetNumberOfSubdivisions (200);
    spline->Update();

    // 3. 转成 vtkTable (X:Z, Y:Y)
    auto table = vtkSmartPointer<vtkTable>::New();
    auto arrZ = vtkSmartPointer<vtkFloatArray>::New();
    auto arrY = vtkSmartPointer<vtkFloatArray>::New();
    arrZ->SetName ("Z (mm)");
    arrY->SetName ("Y (mm)");
    table->AddColumn (arrZ);
    table->AddColumn (arrY);

    auto pts = spline->GetOutput()->GetPoints();
    const vtkIdType npts = pts->GetNumberOfPoints();
    table->SetNumberOfRows (npts);
    for (vtkIdType i = 0; i < npts; ++i)
    {
        double p[3];
        pts->GetPoint (i, p); // p[0]=Z, p[1]=Y
        table->SetValue (i, 0, p[0]); // X <- Z
        table->SetValue (i, 1, p[1]); // Y <- Y
    }

    // 4. 绘制
    auto view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground (1.0, 1.0, 1.0);
    view->GetRenderWindow()->SetSize (winW, winH);
    view->GetRenderWindow()->SetOffScreenRendering (1);

    auto chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem (chart);
    // 曲线
    auto plotLine = chart->AddPlot (vtkChart::LINE);
    plotLine->SetInputData (table, 0, 1);
    plotLine->SetColor (0, 0, 255, 255);
    plotLine->SetWidth (2.0);

    // 坐标轴与网格（缓存指针，避免重复 GetAxis 调用）
    auto axisZ = chart->GetAxis (vtkAxis::BOTTOM);
    auto axisY = chart->GetAxis (vtkAxis::LEFT);

    // 轴标题按需为空；只保留网格与范围
    axisZ->SetTitle ("");
    axisY->SetTitle ("");

    // 网格（统一配色，避免视觉噪声）
    axisZ->SetGridVisible (true);
    axisY->SetGridVisible (true);
    axisZ->GetGridPen()->SetColor (180, 180, 180);
    axisY->GetGridPen()->SetColor (180, 180, 180);

    // 坐标轴固定范围（不受数据影响）
    axisZ->SetBehavior (vtkAxis::FIXED);
    axisZ->SetRange (16, -4);
    axisZ->SetNumberOfTicks (11); // 步长约 2 mm

    axisY->SetBehavior (vtkAxis::FIXED);
    axisY->SetRange (-8, 2);
    axisY->SetNumberOfTicks (6); // 步长约 2 mm

    // ================== 加入图例 ================
    auto reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName ("skull_left.png");
    reader->Update();

    auto resize = vtkSmartPointer<vtkImageResize>::New();
    resize->SetInputConnection (reader->GetOutputPort());
    resize->SetOutputDimensions (48, 48, 1); // 宽 高 深
    resize->Update();

    auto imgMapper = vtkSmartPointer<vtkImageMapper>::New();
    imgMapper->SetInputConnection (resize->GetOutputPort());
    imgMapper->SetColorWindow (255);
    imgMapper->SetColorLevel (127.5);

    auto logo2D = vtkSmartPointer<vtkActor2D>::New();
    logo2D->SetMapper (imgMapper);
    logo2D->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
    int margin = 20;
    int logoW = 48, logoH = 48;
    logo2D->SetPosition (margin * 2, winH - logoH - margin);

    // 添加为 2D 覆盖层
    view->GetRenderer()->AddActor2D (logo2D);

    // ================ 渲染并保存图片 ================
    view->GetRenderWindow()->Render();

    auto w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
    w2i->SetInput (view->GetRenderWindow());
    w2i->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName ("left_condyle_YZ_sagittal.png");
    writer->SetInputConnection (w2i->GetOutputPort());
    writer->Write();
}

/**
* @brief generateRightCondyleImage
*/
void generateRightBennettImage()
{
    int winW = 400, winH = 600; // 你的窗口尺寸

    // 轨迹点 (Y, Z)，单位:mm，起点0,0，向下和向前类直线延伸
    std::vector<std::pair<double, double>> trajectory_adjusted =
    {
        {0.0, 0.0},
        {-1.2, 0.1},
        {-2.1, 0.2},
        {-2.5, 0.24},
        {-2.7, 0.3},
        {-3.6, 0.35},
        {-3.7, 0.4},
        {-4.2, 0.5},
        {-5.0, 0.62},
        {-6.0, 0.63},
        {-7.0, 0.67},
        {-8.0, 0.69}
    };

    // 1. 转成 PolyData，X轴用Z，Y轴用Y
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines = vtkSmartPointer<vtkCellArray>::New();
    auto polyLine = vtkSmartPointer<vtkPolyLine>::New();

    polyLine->GetPointIds()->SetNumberOfIds (static_cast<vtkIdType> (trajectory_adjusted.size()));
    for (vtkIdType i = 0; i < static_cast<vtkIdType> (trajectory_adjusted.size()); ++i)
    {
        const double Y = trajectory_adjusted[static_cast<size_t> (i)].first;
        const double Z = trajectory_adjusted[static_cast<size_t> (i)].second;
        points->InsertNextPoint (Z, Y, 0.0); // 将 Z 映射到 X
        polyLine->GetPointIds()->SetId (i, i); // 索引连续依次连接成折线
    }
    lines->InsertNextCell (polyLine);

    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints (points);
    polyData->SetLines (lines);

    // 2. spline 插值平滑（指定细分数量来提高曲线平滑度）
    auto spline = vtkSmartPointer<vtkSplineFilter>::New();
    spline->SetInputData (polyData);
    spline->SetSubdivideToSpecified();
    spline->SetNumberOfSubdivisions (200);
    spline->Update();

    // 3. 转成 vtkTable (X:Z, Y:Y)
    auto table = vtkSmartPointer<vtkTable>::New();
    auto arrZ = vtkSmartPointer<vtkFloatArray>::New();
    auto arrY = vtkSmartPointer<vtkFloatArray>::New();
    arrZ->SetName ("Z (mm)");
    arrY->SetName ("Y (mm)");
    table->AddColumn (arrZ);
    table->AddColumn (arrY);

    auto pts = spline->GetOutput()->GetPoints();
    const vtkIdType npts = pts->GetNumberOfPoints();
    table->SetNumberOfRows (npts);
    for (vtkIdType i = 0; i < npts; ++i)
    {
        double p[3];
        pts->GetPoint (i, p); // p[0]=Z, p[1]=Y
        table->SetValue (i, 0, p[0]); // X <- Z
        table->SetValue (i, 1, p[1]); // Y <- Y
    }

    // 4. 绘制
    auto view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground (1.0, 1.0, 1.0);
    view->GetRenderWindow()->SetSize (winW, winH);
    view->GetRenderWindow()->SetOffScreenRendering (1);

    auto chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem (chart);
    // 曲线
    auto plotLine = chart->AddPlot (vtkChart::LINE);
    plotLine->SetInputData (table, 0, 1);
    plotLine->SetColor (255, 0, 0, 255);
    plotLine->SetWidth (2.0);

    // 坐标轴与网格（缓存指针，避免重复 GetAxis 调用）
    auto axisZ = chart->GetAxis (vtkAxis::BOTTOM);
    auto axisY = chart->GetAxis (vtkAxis::LEFT);

    // 轴标题按需为空；只保留网格与范围
    axisZ->SetTitle ("");
    axisY->SetTitle ("");

    // 网格（统一配色，避免视觉噪声）
    axisZ->SetGridVisible (true);
    axisY->SetGridVisible (true);
    axisZ->GetGridPen()->SetColor (180, 180, 180);
    axisY->GetGridPen()->SetColor (180, 180, 180);

    // 坐标轴固定范围（不受数据影响）
    axisZ->SetBehavior (vtkAxis::FIXED);
    axisZ->SetRange (-2, 5);
    axisZ->SetNumberOfTicks (8); // 步长约 2 mm

    axisY->SetBehavior (vtkAxis::FIXED);
    axisY->SetRange (-11, 1);
    axisY->SetNumberOfTicks (13); // 步长约 2 mm

    // ================== 加入图例 ================
    // 读取图像
    auto reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName ("skull_right.png");
    reader->Update();
    // 调整到 48x48（保持 1 像素 = 1 显示像素的直觉）
    auto resize = vtkSmartPointer<vtkImageResize>::New();
    resize->SetInputConnection (reader->GetOutputPort());
    resize->SetOutputDimensions (48, 48, 1); // 宽 高 深
    resize->Update();
    // 2D 显示用的图像 mapper（按像素渲染）
    auto imgMapper = vtkSmartPointer<vtkImageMapper>::New();
    imgMapper->SetInputConnection (resize->GetOutputPort());
    // 对于 8-bit 图像，设置窗宽/窗位以避免变灰（RGB/RGBA通常也能直接显示）
    imgMapper->SetColorWindow (255);
    imgMapper->SetColorLevel (127.5);
    // 2D actor：用“显示坐标系”（像素）定位到右上角
    auto logo2D = vtkSmartPointer<vtkActor2D>::New();
    logo2D->SetMapper (imgMapper);
    logo2D->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
    int margin = 20;
    int logoW = 48, logoH = 48;
    logo2D->SetPosition (winW - logoW - margin, winH - logoH - margin);
    // 添加为 2D 覆盖层
    view->GetRenderer()->AddActor2D (logo2D);

    // 渲染并保存图片
    view->GetRenderWindow()->Render();

    auto w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
    w2i->SetInput (view->GetRenderWindow());
    w2i->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName ("right_bennett_YZ_hor.png");
    writer->SetInputConnection (w2i->GetOutputPort());
    writer->Write();
}

/**
* @brief generateRightCondyleImage
*/
void generateRightISSImage()
{
    int winW = 400, winH = 600; // 你的窗口尺寸

    // 轨迹点 (Y, Z)，单位:mm，起点0,0，向下和向前类直线延伸
    std::vector<std::pair<double, double>> trajectory_adjusted =
    {
        {0.0, 0.0},
        {-1.2, 0.1},
        {-2.1, 0.2},
        {-2.5, 0.24},
        {-2.7, 0.3},
        {-3.6, 0.35},
        {-3.7, 0.4},
        {-4.2, 0.5},
        {-5.0, 0.62},
        {-6.0, 0.63},
        {-7.0, 0.67},
        {-8.0, 0.69}
    };

    // 1. 转成 PolyData，X轴用Z，Y轴用Y
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines = vtkSmartPointer<vtkCellArray>::New();
    auto polyLine = vtkSmartPointer<vtkPolyLine>::New();

    polyLine->GetPointIds()->SetNumberOfIds (static_cast<vtkIdType> (trajectory_adjusted.size()));
    for (vtkIdType i = 0; i < static_cast<vtkIdType> (trajectory_adjusted.size()); ++i)
    {
        const double Y = trajectory_adjusted[static_cast<size_t> (i)].first;
        const double Z = trajectory_adjusted[static_cast<size_t> (i)].second;
        points->InsertNextPoint (Z, Y, 0.0); // 将 Z 映射到 X
        polyLine->GetPointIds()->SetId (i, i); // 索引连续依次连接成折线
    }
    lines->InsertNextCell (polyLine);

    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints (points);
    polyData->SetLines (lines);

    // 2. spline 插值平滑（指定细分数量来提高曲线平滑度）
    auto spline = vtkSmartPointer<vtkSplineFilter>::New();
    spline->SetInputData (polyData);
    spline->SetSubdivideToSpecified();
    spline->SetNumberOfSubdivisions (200);
    spline->Update();

    // 3. 转成 vtkTable (X:Z, Y:Y)
    auto table = vtkSmartPointer<vtkTable>::New();
    auto arrZ = vtkSmartPointer<vtkFloatArray>::New();
    auto arrY = vtkSmartPointer<vtkFloatArray>::New();
    arrZ->SetName ("Z (mm)");
    arrY->SetName ("Y (mm)");
    table->AddColumn (arrZ);
    table->AddColumn (arrY);

    auto pts = spline->GetOutput()->GetPoints();
    const vtkIdType npts = pts->GetNumberOfPoints();
    table->SetNumberOfRows (npts);
    for (vtkIdType i = 0; i < npts; ++i)
    {
        double p[3];
        pts->GetPoint (i, p); // p[0]=Z, p[1]=Y
        table->SetValue (i, 0, p[0]); // X <- Z
        table->SetValue (i, 1, p[1]); // Y <- Y
    }

    // 4. 绘制
    auto view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground (1.0, 1.0, 1.0);
    view->GetRenderWindow()->SetSize (winW, winH);
    view->GetRenderWindow()->SetOffScreenRendering (1);

    auto chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem (chart);
    // 曲线
    auto plotLine = chart->AddPlot (vtkChart::LINE);
    plotLine->SetInputData (table, 0, 1);
    plotLine->SetColor (255, 0, 0, 255);
    plotLine->SetWidth (2.0);

    // 坐标轴与网格（缓存指针，避免重复 GetAxis 调用）
    auto axisZ = chart->GetAxis (vtkAxis::BOTTOM);
    auto axisY = chart->GetAxis (vtkAxis::LEFT);

    // 轴标题按需为空；只保留网格与范围
    axisZ->SetTitle ("");
    axisY->SetTitle ("");

    // 网格（统一配色，避免视觉噪声）
    axisZ->SetGridVisible (true);
    axisY->SetGridVisible (true);
    axisZ->GetGridPen()->SetColor (180, 180, 180);
    axisY->GetGridPen()->SetColor (180, 180, 180);

    // 坐标轴固定范围（不受数据影响）
    axisZ->SetBehavior (vtkAxis::FIXED);
    axisZ->SetRange (-1, 2);
    axisZ->SetNumberOfTicks (4); // 步长约 2 mm

    axisY->SetBehavior (vtkAxis::FIXED);
    axisY->SetRange (-3.5, 0.5);
    axisY->SetNumberOfTicks (9); // 步长约 2 mm

    // ================== 加入图例 ================
    // 读取图像
    auto reader = vtkSmartPointer<vtkPNGReader>::New();
    reader->SetFileName ("skull_right.png");
    reader->Update();
    // 调整到 48x48（保持 1 像素 = 1 显示像素的直觉）
    auto resize = vtkSmartPointer<vtkImageResize>::New();
    resize->SetInputConnection (reader->GetOutputPort());
    resize->SetOutputDimensions (48, 48, 1); // 宽 高 深
    resize->Update();
    // 2D 显示用的图像 mapper（按像素渲染）
    auto imgMapper = vtkSmartPointer<vtkImageMapper>::New();
    imgMapper->SetInputConnection (resize->GetOutputPort());
    // 对于 8-bit 图像，设置窗宽/窗位以避免变灰（RGB/RGBA通常也能直接显示）
    imgMapper->SetColorWindow (255);
    imgMapper->SetColorLevel (127.5);
    // 2D actor：用“显示坐标系”（像素）定位到右上角
    auto logo2D = vtkSmartPointer<vtkActor2D>::New();
    logo2D->SetMapper (imgMapper);
    logo2D->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
    int margin = 20;
    int logoW = 48, logoH = 48;
    logo2D->SetPosition (winW - logoW - margin, winH - logoH - margin);
    // 添加为 2D 覆盖层
    view->GetRenderer()->AddActor2D (logo2D);

    // 渲染并保存图片
    view->GetRenderWindow()->Render();

    auto w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
    w2i->SetInput (view->GetRenderWindow());
    w2i->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName ("right_iss_YZ_hor.png");
    writer->SetInputConnection (w2i->GetOutputPort());
    writer->Write();
}

int main()
{
    // generateRightCondyleImage();
    // generateLeftCondyleImage();

    // generateRightBennettImage();
    generateRightISSImage();

    return 0;
}
