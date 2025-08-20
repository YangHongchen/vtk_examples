#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyLine.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>
#include <vtkDiskSource.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPNGReader.h>
#include <vtkImageActor.h>
#include <vtkRenderWindowInteractor.h>
#include <QWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <QApplication>
#include <QTimer>
#include <cmath>
#include <vector>
#include <tuple>  // For handling offset data
#include <vtkImageResize.h>
#include <vtkImageData.h>
#include <vtkImageMapper.h>
#include <vtkLineSource.h>

class ConcentricCirclesWidget : public QVTKOpenGLNativeWidget {
    Q_OBJECT

  public:
    ConcentricCirclesWidget (QWidget* parent = nullptr) : QVTKOpenGLNativeWidget (parent)
    {
        // 初始化渲染器和渲染窗口
        renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground (1.0, 1.0, 1.0); // 白色背景

        // 获取并设置渲染窗口
        vtkRenderWindow* renderWindow = this->renderWindow();
        renderWindow->AddRenderer (renderer);

        // 设置中心点
        center = {0.0, 0.0, 0.0};

        // 禁用鼠标交互
        vtkRenderWindowInteractor* interactor = renderWindow->GetInteractor();
        interactor->SetInteractorStyle (nullptr); // 禁用交互

        // 初始化标题和单位
        initTextActors();

        drawCenterLines();


        // 初始化Logo
        initLogo ("logo.png");

        // // 初始化定时器
        // timer = new QTimer (this);
        // connect (timer, &QTimer::timeout, this, &ConcentricCirclesWidget::updateCircles);
        // timer->start (50); // 20 FPS更新
    }

    // 外部接口：设置中心点
    void setCenter (double x, double y, double z)
    {
        center = {x, y, z};
        updateCircles();
    }

    // 外部接口：接受轨迹数据，并绘制偏移轨迹
    void setOffsetData (const std::vector<std::tuple<double, double, double>>& offsetData)
    {
        offsets = offsetData;
        updateCircles();
    }

  private slots:

    // 更新圆圈和轨迹
    void updateCircles()
    {
        static double angleOffset = 0.0;
        angleOffset += 0.02;  // 旋转速度

        // 创建轨迹数据
        vtkNew<vtkPolyData> polyData;
        vtkNew<vtkPoints> points;
        vtkNew<vtkCellArray> lines;

        // 插入原点（实心二维圆圈）
        vtkSmartPointer<vtkDiskSource> diskSource = vtkSmartPointer<vtkDiskSource>::New();
        diskSource->SetInnerRadius (0.0); // 内半径
        diskSource->SetOuterRadius (0.1); // 外半径，设置小半径代表实心点（例如1像素）
        diskSource->SetRadialResolution (10);
        diskSource->SetCircumferentialResolution (10);
        diskSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> diskMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        diskMapper->SetInputConnection (diskSource->GetOutputPort());

        vtkSmartPointer<vtkActor> diskActor = vtkSmartPointer<vtkActor>::New();
        diskActor->SetMapper (diskMapper);
        diskActor->GetProperty()->SetColor (0.8, 0.8, 0.8); // 灰色
        renderer->AddActor (diskActor);

        // 创建5个同心圆，每个间距1mm
        const int numCircles = 5;
        const double spacing = 1.0;  // 1mm 间距
        const int segments = 100;    // 每圆100段

        // 绘制同心圆
        for (int circleIdx = 0; circleIdx < numCircles; ++circleIdx)
        {
            double radius = (circleIdx + 1) * spacing;  // 半径从1mm开始递增

            vtkIdType prevPointId = -1;
            std::vector<vtkIdType> circlePoints;

            // 生成圆上的点
            for (int j = 0; j <= segments; ++j)
            {
                double angle = 2.0 * M_PI * j / segments + angleOffset;
                double x = center[0] + radius * cos (angle);
                double y = center[1] + radius * sin (angle);
                double z = center[2];

                vtkIdType currentPointId = points->InsertNextPoint (x, y, z);
                circlePoints.push_back (currentPointId);

                // 连接前后点
                if (prevPointId != -1)
                {
                    vtkNew<vtkPolyLine> line;
                    line->GetPointIds()->SetNumberOfIds (2);
                    line->GetPointIds()->SetId (0, prevPointId);
                    line->GetPointIds()->SetId (1, currentPointId);
                    lines->InsertNextCell (line);
                }
                prevPointId = currentPointId;
            }

            // 闭合圆
            if (!circlePoints.empty())
            {
                vtkNew<vtkPolyLine> closingLine;
                closingLine->GetPointIds()->SetNumberOfIds (2);
                closingLine->GetPointIds()->SetId (0, circlePoints.back());
                closingLine->GetPointIds()->SetId (1, circlePoints.front());
                lines->InsertNextCell (closingLine);
            }
        }

        // 使用平滑曲线连接轨迹点
        vtkNew<vtkCardinalSpline> spline;
        vtkNew<vtkSplineFilter> splineFilter;
        splineFilter->SetInputData (polyData);
        splineFilter->SetSpline (spline);
        splineFilter->Update();

        polyData->SetPoints (points);
        polyData->SetLines (lines);

        // 创建并配置mapper和actor
        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputData (polyData);

        vtkNew<vtkActor> actor;
        actor->SetMapper (mapper);
        actor->GetProperty()->SetColor (0.8, 0.8, 0.8); // 蓝色
        actor->GetProperty()->SetLineWidth (2.0); // 2像素线宽

        renderer->AddActor (actor);

        // 添加轨迹的绘制
        if (!offsets.empty())
        {
            vtkNew<vtkPolyData> trajectoryPolyData;
            vtkNew<vtkPoints> trajectoryPoints;
            vtkNew<vtkCellArray> trajectoryLines;

            vtkIdType prevOffsetPointId = -1;

            // 遍历偏移数据并绘制轨迹
            for (const auto& offset : offsets)
            {
                double offsetX = std::get<0> (offset);
                double offsetY = std::get<1> (offset);
                double offsetZ = std::get<2> (offset);

                double x = center[0] + offsetX;
                double y = center[1] + offsetY;
                double z = center[2] + offsetZ;

                vtkIdType currentPointId = trajectoryPoints->InsertNextPoint (x, y, z);

                if (prevOffsetPointId != -1)
                {
                    vtkNew<vtkPolyLine> line;
                    line->GetPointIds()->SetNumberOfIds (2);
                    line->GetPointIds()->SetId (0, prevOffsetPointId);
                    line->GetPointIds()->SetId (1, currentPointId);
                    trajectoryLines->InsertNextCell (line);
                }

                prevOffsetPointId = currentPointId;
            }

            trajectoryPolyData->SetPoints (trajectoryPoints);
            trajectoryPolyData->SetLines (trajectoryLines);

            vtkNew<vtkPolyDataMapper> trajectoryMapper;
            trajectoryMapper->SetInputData (trajectoryPolyData);

            vtkNew<vtkActor> trajectoryActor;
            trajectoryActor->SetMapper (trajectoryMapper);
            trajectoryActor->GetProperty()->SetColor (1.0, 0.0, 0.0); // 红色轨迹
            trajectoryActor->GetProperty()->SetLineWidth (2.0);

            renderer->AddActor (trajectoryActor);
        }

        renderer->ResetCamera();
        this->renderWindow()->Render();
    }


    // 初始化文本：标题和单位
    void initTextActors()
    {
        // 标题
        vtkSmartPointer<vtkTextActor> titleActor = vtkSmartPointer<vtkTextActor>::New();
        titleActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
        titleActor->SetPosition (center[0] + 0.1, center[1] + 0.85); // 调整位置相对圆心
        titleActor->GetTextProperty()->SetFontSize (20); // 设置字体大小
        titleActor->GetTextProperty()->SetColor (0.0, 0.0, 0.0); // 设置字体颜色为黑色
        titleActor->SetInput ("Left Condyle"); // 设置标题文本
        renderer->AddActor (titleActor);

        // 单位
        vtkSmartPointer<vtkTextActor> unitActor = vtkSmartPointer<vtkTextActor>::New();
        unitActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
        unitActor->SetPosition (center[0] + 0.85, center[1]  + 0.15); // 调整位置相对圆心
        unitActor->GetTextProperty()->SetFontSize (14); // 设置字体大小
        unitActor->GetTextProperty()->SetColor (0.0, 0.0, 0.0); // 设置字体颜色为黑色
        unitActor->SetInput ("1mm"); // 设置单位
        renderer->AddActor (unitActor);

        renderer->Modified();
        this->renderWindow()->Render();  // 立即刷新窗口
    }

    // 绘制基于中心点的横线和竖线
    void drawCenterLines()
    {
        // 绘制横线
        vtkSmartPointer<vtkLineSource> horizontalLine = vtkSmartPointer<vtkLineSource>::New();
        horizontalLine->SetPoint1 (center[0] - 1.0 * 6, center[1], center[2]); // 左端
        horizontalLine->SetPoint2 (center[0] + 1.0 * 6, center[1], center[2]);  // 右端
        horizontalLine->Update();

        vtkSmartPointer<vtkPolyDataMapper> horizontalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        horizontalMapper->SetInputData (horizontalLine->GetOutput());

        vtkSmartPointer<vtkActor> horizontalActor = vtkSmartPointer<vtkActor>::New();
        horizontalActor->SetMapper (horizontalMapper);
        horizontalActor->GetProperty()->SetColor (0.0, 0.0, 0.0); // 黑色
        renderer->AddActor (horizontalActor);

        // 绘制竖线
        vtkSmartPointer<vtkLineSource> verticalLine = vtkSmartPointer<vtkLineSource>::New();
        verticalLine->SetPoint1 (center[0], center[1] - 1.0 * 6, center[2]); // 下端
        verticalLine->SetPoint2 (center[0], center[1] + 1.0 * 6, center[2]); // 上端
        verticalLine->Update();

        vtkSmartPointer<vtkPolyDataMapper> verticalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        verticalMapper->SetInputData (verticalLine->GetOutput());

        vtkSmartPointer<vtkActor> verticalActor = vtkSmartPointer<vtkActor>::New();
        verticalActor->SetMapper (verticalMapper);
        verticalActor->GetProperty()->SetColor (0.0, 0.0, 0.0); // 黑色
        renderer->AddActor (verticalActor);
    }

    // 初始化Logo
    // 初始化Logo
    void initLogo (QString logoFile)
    {
        QImage qimg (logoFile);
        if (qimg.isNull())
        {
            qWarning() << "Logo load failed:" << logoFile;
            return;
        }

        // qimg = qimg.mirrored (false, true); // 镜像翻转
        auto vtkImg = qimageToVtk (qimg);

        auto resize = vtkSmartPointer<vtkImageResize>::New();
        resize->SetInputData (vtkImg);
        resize->SetOutputDimensions (96, 96, 1); // 设置Logo大小
        resize->Update();

        vtkSmartPointer<vtkActor2D> logoActor;
        auto imgMapper = vtkSmartPointer<vtkImageMapper>::New();
        imgMapper->SetInputConnection (resize->GetOutputPort());
        imgMapper->SetColorWindow (255);
        imgMapper->SetColorLevel (127.5);

        logoActor = vtkSmartPointer<vtkActor2D>::New();
        logoActor->SetMapper (imgMapper);

        // 设置Logo位置为相对圆心位置，使用[0, 1]标准化坐标
        double logoOffsetX = 0.1;  // 相对于圆心的X偏移量
        double logoOffsetY = 0.15;  // 相对于圆心的Y偏移量

        logoActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();

        // 计算位置：相对于圆心偏移
        double posX = center[0] + logoOffsetX;  // X坐标偏移
        double posY = center[1] + logoOffsetY;  // Y坐标偏移

        // 确保位置在[0, 1]范围内
        posX = std::min (std::max (posX, 0.0), 1.0);
        posY = std::min (std::max (posY, 0.0), 1.0);

        logoActor->SetPosition (posX, posY); // 设置标准化坐标

        renderer->AddActor2D (logoActor);
    }


    // QImage 转换为 VTK ImageData
    vtkSmartPointer<vtkImageData> qimageToVtk (const QImage& qimg)
    {
        QImage img = qimg.convertToFormat (QImage::Format_RGBA8888);
        auto vtkImg = vtkSmartPointer<vtkImageData>::New();
        vtkImg->SetExtent (0, img.width() - 1, 0, img.height() - 1, 0, 0);
        vtkImg->AllocateScalars (VTK_UNSIGNED_CHAR, 4);

        for (int y = 0; y < img.height(); ++y)
        {
            memcpy (vtkImg->GetScalarPointer (0, y, 0), img.scanLine (y), img.bytesPerLine());
        }
        return vtkImg;
    }

  private:
    vtkSmartPointer<vtkRenderer> renderer;
    QTimer* timer;
    std::vector<double> center;  // 中心点坐标
    std::vector<std::tuple<double, double, double>> offsets;  // 偏移轨迹数据
    vtkIdType prevOffsetPointId = -1;  // 用于绘制偏移轨迹时的上一个点ID
};

#include "main.moc"  // 用于Qt的元对象编译器

int main (int argc, char* argv[])
{
    // 强制使用 OpenGL 渲染后端 (必须在QApplication之前设置)
    qputenv ("QSG_RHI_BACKEND", "opengl");

    QApplication app (argc, argv);

    ConcentricCirclesWidget widget;
    widget.resize (800, 600);
    widget.show();

    // 示例：设置偏移数据
    std::vector<std::tuple<double, double, double>> offsetData =
    {
        {1.0, 0.0, 0.0},
        {0.5, 0.5, 0.0},
        {0.0, 1.0, 0.0},
        {-0.5, 0.5, 0.0},
        {-1.0, 0.0, 0.0}
    };
    widget.setOffsetData (offsetData);

    return app.exec();
}
