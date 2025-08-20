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
#include <QWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <QApplication>
#include <QTimer>
#include <cmath>
#include <vector>

class ConcentricCirclesWidget : public QVTKOpenGLNativeWidget {
    Q_OBJECT

  public:
    ConcentricCirclesWidget (QWidget* parent = nullptr)
        : QVTKOpenGLNativeWidget (parent)
    {
        // 初始化渲染器和渲染窗口
        renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground (1.0, 1.0, 1.0); // 白色背景

        // 获取并设置渲染窗口
        vtkRenderWindow* renderWindow = this->renderWindow();
        renderWindow->AddRenderer (renderer);

        // 设置中心点
        center = {0.0, 0.0, 0.0};

        // 初始化定时器
        timer = new QTimer (this);
        connect (timer, &QTimer::timeout, this, &ConcentricCirclesWidget::updateCircles);
        timer->start (50); // 20 FPS更新

        // 初始绘制
        updateCircles();
    }

  private slots:
    void updateCircles()
    {
        static double angleOffset = 0.0;
        angleOffset += 0.02; // 旋转速度

        // 清除现有的轨迹
        renderer->RemoveAllViewProps();

        // 创建轨迹数据
        vtkNew<vtkPolyData> polyData;
        vtkNew<vtkPoints> points;
        vtkNew<vtkCellArray> lines;

        // 创建5个同心圆，每个间距1mm
        const int numCircles = 5;
        const double spacing = 1.0; // 1mm间距
        const int segments = 100;   // 每圆100段

        for (int circleIdx = 0; circleIdx < numCircles; ++circleIdx)
        {
            double radius = (circleIdx + 1) * spacing; // 半径从1mm开始递增

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

        polyData->SetPoints (points);
        polyData->SetLines (lines);

        // 创建并配置mapper和actor
        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputData (polyData);

        vtkNew<vtkActor> actor;
        actor->SetMapper (mapper);
        actor->GetProperty()->SetColor (0.0, 0.0, 1.0); // 蓝色
        actor->GetProperty()->SetLineWidth (2.0);      // 2像素线宽

        renderer->AddActor (actor);
        renderer->ResetCamera();
        this->renderWindow()->Render();
    }

  private:
    vtkSmartPointer<vtkRenderer> renderer;
    QTimer *timer;
    std::vector<double> center; // 中心点坐标
};

#include "main.moc"  // 用于Qt的元对象编译器

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);

    ConcentricCirclesWidget widget;
    widget.resize (800,  600);
    widget.show();

    return app.exec();
}
