#ifndef VTKJAWRENDERERWIDGET_H
#define VTKJAWRENDERERWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>

// VTK 9.3.0 必需头文件
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

class QTimer;

class VtkJawRendererWidget : public QVTKOpenGLNativeWidget {
    Q_OBJECT
    Q_PROPERTY (float biteProgress READ biteProgress NOTIFY biteProgressChanged)

  public:
    explicit VtkJawRendererWidget (QWidget* parent = nullptr);
    ~VtkJawRendererWidget();

    // 模型操作
    Q_INVOKABLE bool loadUpperJaw (const QString& filePath);
    Q_INVOKABLE bool loadLowerJaw (const QString& filePath);

    // 运动控制
    Q_INVOKABLE void startBiteSimulation();
    Q_INVOKABLE void stopSimulation();
    Q_INVOKABLE void resetPosition();
    Q_INVOKABLE void setJawPosition (float x, float y, float z, float rotation);

    // 校准功能
    Q_INVOKABLE void autoAlignJaws();

    // 属性访问
    float biteProgress() const { return m_biteProgress; }

  signals:
    void simulationStarted();
    void simulationFinished();
    void biteProgressChanged (float progress);
    void errorOccurred (const QString& message);

  private slots:
    void updateBiteAnimation();

  private:
    void initializeRenderSystem();
    void calculateBiteAlignment();

    // VTK对象
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkActor> m_upperJawActor;
    vtkSmartPointer<vtkActor> m_lowerJawActor;
    vtkSmartPointer<vtkTransform> m_lowerJawTransform;

    // 动画控制
    QTimer *m_animationTimer;
    float m_biteProgress = 0.0f;
    float m_currentRotation = 0.0f;
};

#endif // VTKJAWRENDERERWIDGET_H
