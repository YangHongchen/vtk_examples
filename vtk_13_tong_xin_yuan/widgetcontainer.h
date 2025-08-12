#pragma once

#include <QObject>
#include <QWidget>
#include <QWindow>
#include <QQuickWindow>
#include <QQuickItem>

class WidgetContainer : public QObject
{
    Q_OBJECT
public:
    explicit WidgetContainer(QObject *parent = nullptr);

    Q_INVOKABLE bool embedWidget(QQuickItem *containerItem);
    Q_INVOKABLE void updateGeometry(QQuickItem *containerItem);

    void setWidget(QWidget *widget);
    QWidget* widget() const;

private:
    QWidget *m_widget = nullptr;
    QWindow *m_proxyWindow = nullptr;  // 新增：作为中介的QWindow
    QWidget *m_container = nullptr;

signals:
    void widgetChanged();
};
