// EventBus.h
#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <QObject>
#include <QVariant>
#include <QMutex>
#include <QHash>
#include <QList>
#include <QJSValue>
#include <QJSEngine>

class EventBus : public QObject {
    Q_OBJECT
  public:
    static EventBus &instance();

    Q_INVOKABLE void publish (const QString& eventName, const QVariant& data); // QML/C++ 通用
    Q_INVOKABLE void subscribeSlot (const QString& eventName, QObject* receiver,
                                    const QString& method); // C++槽函数订阅
    Q_INVOKABLE void unsubscribe (const QString& eventName, QObject* receiver);
    Q_INVOKABLE void unsubscribeAll (QObject* receiver);
    Q_INVOKABLE void subscribeJS (const QString& eventName, const QJSValue& callback); // QML回调订阅
    Q_INVOKABLE void unsubscribeJS (const QString& eventName, const QJSValue& callback);

  signals:
    void eventEmitted (const QString& eventName, const QVariant& data); // 可在 QML 端使用 connect

  private:
    explicit EventBus (QObject *parent = nullptr);
    ~EventBus() override;

    struct QmlCallback
    {
        QJSValue callback;
        QJSEngine *engine;
    };

    QMutex m_mutex;
    QHash<QString, QList<QObject *>> m_slotSubscribers;
    QHash<QString, QList<QmlCallback>> m_jsSubscribers;
};

#endif // EVENTBUS_H
