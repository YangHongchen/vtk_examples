// #ifndef OCCLUSIONWIDGET_H
// #define OCCLUSIONWIDGET_H

// #include <QVTKOpenGLNativeWidget.h>
// #include <QJsonObject>
// #include <QJsonArray>
// #include <vtkSmartPointer.h>
// #include <vtkRenderer.h>
// #include <vtkSTLReader.h>
// #include <vtkActor.h>
// #include <vtkPolyDataMapper.h>

// class OcclusionWidget : public QVTKOpenGLNativeWidget {
//     Q_OBJECT

//   public:
//     explicit OcclusionWidget (QWidget *parent = nullptr);
//     void loadUpperModel (const QString &filePath);
//     void loadLowerModel (const QString &filePath);
//     void updateFromData (const QJsonObject &data);

//     void applyLowerJawTransform (double offsetMm);

//     void markInitialTip();

//   private:
//     vtkSmartPointer<vtkRenderer> m_renderer;
//     vtkSmartPointer<vtkActor> m_upperActor;
//     vtkSmartPointer<vtkActor> m_lowerActor;

//     // 动态参考点和法向
//     double m_tipPoint[3] = {0};                 // 实时牙尖点（毫米）
//     double m_tipNormal[3] = {0};                // 实时牙尖法向（单位向量）
//     double m_initialLowerTipPosition[3] = {0};  // 初始化下颌牙尖点

//     void resetCamera();
//     void applyLowerJawTransform();
// };

// #endif // OCCLUSIONWIDGET_H
