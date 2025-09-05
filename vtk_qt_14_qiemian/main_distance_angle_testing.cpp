// // main.cpp
// #include <QApplication>
// #include <QSurfaceFormat>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QWidget>
// #include <QVTKOpenGLNativeWidget.h>

// #include <vtkGenericOpenGLRenderWindow.h>
// #include <vtkRenderer.h>
// #include <vtkSTLReader.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>

// #include <vtkDistanceWidget.h>
// #include <vtkDistanceRepresentation2D.h>
// #include <vtkAngleWidget.h>
// #include <vtkAngleRepresentation2D.h>

// class MainWindow : public QWidget {
//     Q_OBJECT

// public:
//     MainWindow (QWidget* parent = nullptr)
//         : QWidget (parent)
//     {
//         QVBoxLayout* layout = new QVBoxLayout (this);

//         // ====== VTK Widget ======
//         vtkWidget = new QVTKOpenGLNativeWidget (this);
//         layout->addWidget (vtkWidget, 1);

//         auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//         vtkWidget->setRenderWindow (renderWindow);

//         renderer = vtkSmartPointer<vtkRenderer>::New();
//         vtkWidget->renderWindow()->AddRenderer (renderer);

//         interactor = vtkWidget->renderWindow()->GetInteractor();

//         // ====== 按钮 ======
//         QPushButton* btnDistance = new QPushButton ("开始测距");
//         QPushButton* btnAngle    = new QPushButton ("开始测角");
//         QPushButton* btnClear    = new QPushButton ("清除测量");

//         layout->addWidget (btnDistance);
//         layout->addWidget (btnAngle);
//         layout->addWidget (btnClear);

//         connect (btnDistance, &QPushButton::clicked, this, &MainWindow::startDistanceMeasure);
//         connect (btnAngle,    &QPushButton::clicked, this, &MainWindow::startAngleMeasure);
//         connect (btnClear,    &QPushButton::clicked, this, &MainWindow::clearMeasure);

//         // ====== 加载 STL ======
//         auto reader = vtkSmartPointer<vtkSTLReader>::New();
//         reader->SetFileName ("c:/stl/1.stl");  // TODO: 替换成你的 STL 文件路径
//         reader->Update();

//         auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//         mapper->SetInputConnection (reader->GetOutputPort());

//         auto actor = vtkSmartPointer<vtkActor>::New();
//         actor->SetMapper (mapper);

//         renderer->AddActor (actor);
//         renderer->ResetCamera();
//     }

// public slots:
//     void startDistanceMeasure()
//     {
//         clearMeasure();

//         distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
//         distanceWidget->SetInteractor (interactor);

//         auto rep = vtkSmartPointer<vtkDistanceRepresentation2D>::New();
//         distanceWidget->SetRepresentation (rep);

//         distanceWidget->On();
//     }

//     void startAngleMeasure()
//     {
//         clearMeasure();

//         angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
//         angleWidget->SetInteractor (interactor);

//         auto rep = vtkSmartPointer<vtkAngleRepresentation2D>::New();
//         angleWidget->SetRepresentation (rep);

//         angleWidget->On();
//     }

//     void clearMeasure()
//     {
//         if (distanceWidget)
//         {
//             distanceWidget->Off();
//             distanceWidget = nullptr;
//         }
//         if (angleWidget)
//         {
//             angleWidget->Off();
//             angleWidget = nullptr;
//         }
//         vtkWidget->renderWindow()->Render();
//     }

// private:
//     QVTKOpenGLNativeWidget *vtkWidget;
//     vtkSmartPointer<vtkRenderer> renderer;
//     vtkRenderWindowInteractor *interactor = nullptr;

//     vtkSmartPointer<vtkDistanceWidget> distanceWidget;
//     vtkSmartPointer<vtkAngleWidget> angleWidget;
// };

// int main (int argc, char* argv[])
// {
//     QApplication app (argc, argv);
//     QSurfaceFormat::setDefaultFormat (QVTKOpenGLNativeWidget::defaultFormat());

//     MainWindow window;
//     window.resize (1000, 800);
//     window.show();

//     return app.exec();
// }

// #include "main.moc"
