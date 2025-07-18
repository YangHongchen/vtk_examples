#include "PatientController.h"

PatientController::PatientController(QObject *parent):QObject(parent)
{
    qDebug()<< "Patient Controller Called.";
}

void PatientController::setName(const QString &name)
{

    m_name = name;
    emit nameChanged(m_name);

}
