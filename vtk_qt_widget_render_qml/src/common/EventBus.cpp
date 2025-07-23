// EventBus.cpp
#include "EventBus.h"
#include <QDebug>
#include <QJSEngine>
#include <QJSValueList>
#include <QMetaObject>

EventBus &EventBus::instance()
{
    static EventBus instance;
    return instance;
}

EventBus::EventBus (QObject* parent) : QObject (parent) {}

EventBus::~EventBus() {}

void EventBus::publish (const QString& eventName, const QVariant& data)
{
    qDebug() << "[EventBus] 发布事件:" << eventName << " 数据:" << data;
    if (eventName.isEmpty())
    {
        qWarning() << "[EventBus]事件名为空，已忽略发布";
        return;
    }
    // 派发信号供 C++ 和 QML connect() 使用
    emit eventEmitted (eventName, data);
    // 分发给 subscribeJS 注册的 QML 回调
    QMutexLocker locker (&m_mutex);
    const auto jsList = m_jsSubscribers.value (eventName);
    for (const auto& cb : jsList)
    {
        if (!cb.callback.isCallable() || !cb.engine)
        {
            qWarning() << "[EventBus] 无效回调，跳过事件：" << eventName;
            continue;
        }
        QJSValue converted = cb.engine->toScriptValue (data);
        if (converted.isUndefined() || converted.isNull())
        {
            qWarning() << "[EventBus] payload 转换失败，事件：" << eventName << " 原始数据：" << data;
            continue;
        }
        QJSValueList args;
        args << QJSValue (eventName);
        args << converted;

        cb.callback.call (args);
    }
}


void EventBus::subscribeSlot (const QString& eventName, QObject* receiver, const QString& method)
{
    QMutexLocker locker (&m_mutex);
    m_slotSubscribers[eventName].append (receiver);

    // 如果没有连接信号，则连接一次
    static bool connected = false;
    if (!connected)
    {
        connect (this, &EventBus::eventEmitted, this, [this, method] (const QString & name, const QVariant & payload)
        {
            QMutexLocker locker (&m_mutex);
            const auto receivers = m_slotSubscribers.value (name);
            for (QObject * receiver : receivers)
            {
                if (receiver)
                {
                    // 调用方法名，参数必须匹配 (QString, QVariant)
                    bool success = QMetaObject::invokeMethod (receiver, method.toUtf8().constData(),
                                   Qt::DirectConnection,
                                   Q_ARG (QString, name),
                                   Q_ARG (QVariant, payload));
                    if (!success)
                    {
                        qWarning() << "[EventBus] Failed to invoke" << method << "on" << receiver;
                    }
                }
            }
        });
        connected = true;
    }
}
void EventBus::unsubscribe (const QString& eventName, QObject* receiver)
{
    QMutexLocker locker (&m_mutex);
    disconnect (this, nullptr, receiver, nullptr);
    m_slotSubscribers[eventName].removeAll (receiver);
}

void EventBus::unsubscribeAll (QObject* receiver)
{
    QMutexLocker locker (&m_mutex);
    disconnect (this, nullptr, receiver, nullptr);
    for (auto& list : m_slotSubscribers)
        list.removeAll (receiver);
}

void EventBus::subscribeJS (const QString& eventName, const QJSValue& callback)
{
    if (!callback.isCallable())
        return;

    QMutexLocker locker (&m_mutex);
    m_jsSubscribers[eventName].append ({ callback, qjsEngine (this) });
}

void EventBus::unsubscribeJS (const QString& eventName, const QJSValue& callback)
{
    QMutexLocker locker (&m_mutex);
    auto& list = m_jsSubscribers[eventName];
    list.removeIf ([&] (const QmlCallback & cb)
    {
        return cb.callback.strictlyEquals (callback);
    });
}
