#ifndef MESURE_WIDGET_H
#define MESURE_WIDGET_H

#include <QWidget>
#include <QWidget>
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include "src/patient/PatientModel.h"
#include "src/patient/PatientObject.h"

class MesureWidget : public QWidget {
    Q_OBJECT
  public:
    explicit MesureWidget (QWidget *parent = nullptr);
    ~MesureWidget();

    // 公开接口
    Q_INVOKABLE void startMeasurement();
    Q_INVOKABLE void setMeasurementMode (int mode);
    Q_INVOKABLE void saveMeasurement (const QString& filePath);

  signals:
    void measurementCompleted (double result);
    void errorOccurred (const QString& message);

  private slots:
    void handleQmlEvent (const QVariant& data);
    void onCurrentPatientChanged (PatientObject *patientObject);

  private:
    void setupVTK();
    void setupQML();
    void setupConnections();

    // 加载测量步骤
    void setupSteps();

    // 加载控制面板
    void setupControl();

    // 显示当前选中病例信息组件（悬浮rectangle）
    void setupPatientWidget();

    // VTK 相关
    QVTKOpenGLNativeWidget *m_vtkWidget;
    vtkSmartPointer<vtkRenderer> m_renderer;

    // QML 相关
    QQuickWidget *m_qmlControl;
    bool m_measurementActive = false;

    // 测量步骤组件
    QQuickWidget *m_stepsOverlay;

    // 当前病例组件
    QQuickWidget *m_patientOverlay;

    // 选中病例
    PatientModel *m_patientModel = nullptr;
    PatientObject *m_patient = nullptr;


    // 根据病例变化，更新病例组件的数据
    void updateQmlPatientData();


};

#endif // MESURE_WIDGET_H
