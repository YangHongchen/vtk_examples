#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>

class LoginController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
public:
    explicit LoginController(QObject* parent = nullptr);

    QString userName() const;
    void setUserName(const QString& name);

signals:
    void userNameChanged();

private:
    QString m_userName;
};
#endif // LOGINCONTROLLER_H
