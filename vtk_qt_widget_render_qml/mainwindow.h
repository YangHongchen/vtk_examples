#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QQuickWidget>
#include <QHBoxLayout>


class VtkRenderWidget;  // 前置声明自定义Widget


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handlePageChange(int index);

private:
    void addQmlPage(const QString &qmlPath);

private:
    QQuickWidget *m_navWidget;
    QStackedWidget *m_contentStack;
    VtkRenderWidget *m_vtkWidget = nullptr;  // 自定义Widget指针
};


#endif // MAINWINDOW_H
