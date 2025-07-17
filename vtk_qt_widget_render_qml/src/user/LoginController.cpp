#include "LoginController.h"

LoginController::LoginController(QObject *parent): QObject(parent) {}

QString LoginController::userName() const { return m_userName; }

void LoginController::setUserName(const QString& name) {
    if (m_userName != name) {
        m_userName = name;
        emit userNameChanged();
    }
}
