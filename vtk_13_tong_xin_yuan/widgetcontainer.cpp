#include "WidgetContainer.h"
#include <QDebug>

WidgetContainer::WidgetContainer(QObject *parent) : QObject(parent) {}

bool WidgetContainer::embedWidget(QQuickItem *containerItem)
{
    qDebug() << "embedWidget called";

    if (!m_widget || !containerItem || !containerItem->window()) {
        qWarning() << "Invalid parameters for embedWidget";
        return false;
    }

    // 清理旧资源
    qDebug() << "embedWidget 清理旧资源";
    if (m_container) {
        m_container->deleteLater();
        m_container = nullptr;
    }
    if (m_proxyWindow) {
        m_proxyWindow->destroy();
        m_proxyWindow = nullptr;
    }

    qDebug() << "embedWidget 创建代理QWindow";
    // 创建代理QWindow
    m_proxyWindow = new QWindow(containerItem->window());
    m_proxyWindow->setFlags(Qt::FramelessWindowHint);

    // 正确调用（注意参数顺序）
    m_container = QWidget::createWindowContainer(
        m_proxyWindow,  // QWindow* 参数（第一参数）
        nullptr,        // QWidget* parent（第二参数）
        Qt::FramelessWindowHint
        );

    if (!m_container) {
        qWarning() << "Failed to create window container";
        return false;
    }

    qDebug() << "embedWidget 设置Widget和容器的父子关系";
    // 设置Widget和容器的父子关系
    QWidget *nativeParent = QWidget::find(containerItem->window()->winId());
    if (nativeParent) {
        m_container->setParent(nativeParent);
    }

    // 将Widget嵌入到代理窗口
    m_widget->setParent(m_container);
    m_widget->show();

    updateGeometry(containerItem);

    qDebug() << "embedWidget updateGeometry";
    return true;
}

void WidgetContainer::updateGeometry(QQuickItem *containerItem)
{
    if (!m_container || !containerItem) return;

    const QRectF rect = containerItem->mapRectToScene(
        QRectF(0, 0, containerItem->width(), containerItem->height()));

    // 必须添加1px偏移解决渲染问题
    m_container->setGeometry(rect.toRect().adjusted(1,  1, -1, -1));
}

void WidgetContainer::setWidget(QWidget *widget)
{
    if (m_widget == widget)
        return;

    m_widget = widget;
    emit widgetChanged();  // 如果有对应的信号
}

QWidget *WidgetContainer::widget() const
{
    return m_widget;
}
