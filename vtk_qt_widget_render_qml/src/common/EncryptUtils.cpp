#include "EncryptUtils.h"
#include <QCryptographicHash>

QString EncryptUtils::passwordEncrypt(const QString passwordRaw)
{
    QString passwordEncrypted = "";
    passwordEncrypted = QString(QCryptographicHash::hash(QString(passwordRaw).toUtf8(), QCryptographicHash::Sha256).toHex());
    return passwordEncrypted;
}
