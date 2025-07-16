#ifndef BACKEND_H
#define BACKEND_H
#include <QObject>
#include <QDebug>

class Backend: public QObject
{
public:
    explicit Backend(QObject *parent = nullptr) : QObject(parent) {}
public slots:
    void doSomething() {
        qDebug() << "C++处理业务逻辑";
        emit resultReady("操作完成");
    }

signals:
    void resultReady(const QString &message);
};

#endif // BACKEND_H
