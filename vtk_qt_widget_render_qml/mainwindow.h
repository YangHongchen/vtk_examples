#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QQuickWidget>
#include <QHBoxLayout>

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
};


#endif // MAINWINDOW_H
