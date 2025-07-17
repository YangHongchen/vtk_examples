#ifndef PATIENTCONTROLLER_H
#define PATIENTCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QDate>

class PatientController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)


public:

    explicit PatientController(QObject *parent = nullptr);



public:

    Q_INVOKABLE void setName(const QString &name) {
        m_name = name;
        emit nameChanged(m_name);
    }

signals:
    void nameChanged(const QString &name);

protected:
    QString getName() const {
        return m_name;
    }

signals:
    void dataChanged();  // 统一数据变化信号

private:
    QString m_name = "星辰";

};

#endif // PATIENTCONTROLLER_H
