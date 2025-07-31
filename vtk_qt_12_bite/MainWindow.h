#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QMainWindow>
#include "Mesure3DWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow (QWidget *parent = nullptr);

  signals:
  private:

    Mesure3DWidget *m_vtkWidget = nullptr;;


};

#endif // MAINWINDOW_H
