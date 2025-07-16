#include "VtkWidgetWindow.h"
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <QVBoxLayout>
#include <vtkProperty.h>

VtkWidgetWindow::VtkWidgetWindow(QWidget *parent) : QMainWindow(parent) {
    init();
}

VtkWidgetWindow::~VtkWidgetWindow()
{
    // delete vtkWidget;
}

void VtkWidgetWindow::init()
{
    vtkWidget = new QVTKOpenGLNativeWidget(this);
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkWidget->setRenderWindow(renderWindow);

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(50.0);
    sphere->SetThetaResolution(30);
    sphere->SetPhiResolution(30);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 1.0); // 黄色

    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.1);

    setCentralWidget(vtkWidget);
}
