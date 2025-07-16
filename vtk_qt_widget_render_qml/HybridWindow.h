#ifndef HYBRIDWINDOW_H
#define HYBRIDWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QQuickWidget>

class HybridWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit HybridWindow(QWidget *parent = nullptr);

private:
    QSplitter *m_splitter;
    QQuickWidget *m_qmlWidget;
    QWidget *m_vtkContainer;
};

#endif // HYBRIDWINDOW_H
