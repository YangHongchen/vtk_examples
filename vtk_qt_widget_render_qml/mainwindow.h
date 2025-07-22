#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QQuickWidget>
#include <QHBoxLayout>
#include "src/patient/PatientController.h"
#include "src/patient/PatientModel.h"
#include "src/common/PatientFileTransferManager.h"
#include "src/mesure/MesureRecordController.h"
#include "src/mesure/MesureRecordModel.h"

class VtkRenderWidget;  // 前置声明自定义Widget

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow (QWidget *parent = nullptr);

  private slots:
    void handlePageChange (int index);

  private:
    void addQmlPage (const QString &qmlPath);

    void setupUI();
    void refreshUI();
    void setupCpps();

  private:
    QQuickWidget *m_navWidget;
    QStackedWidget *m_contentStack;
    VtkRenderWidget *m_vtkWidget = nullptr;

    // 资源类
    PatientController *m_patientController = nullptr;
    PatientModel *m_patientModel = nullptr;
    PatientFileTransferManager *m_patientFileTransferManager = nullptr;
    MesureRecordController *m_mesureRecordController = nullptr;
    MesureRecordModel *m_mesureRecordModel = nullptr;

};


#endif // MAINWINDOW_H
