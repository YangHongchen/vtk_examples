#ifndef PATIENTCONTROLLER_H
#define PATIENTCONTROLLER_H

#include <QObject>
#include "PatientModel.h"
#include "Patient.h"
#include "src/patient/PatientDao.h"

class PatientController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PatientModel* model READ model CONSTANT)
    Q_PROPERTY(Patient* currentPatient READ currentPatient NOTIFY currentPatientChanged)

public:
    Q_INVOKABLE void loadPatients();
    Q_INVOKABLE void selectPatient(int row);

public:
    static PatientController* instance();
    PatientModel* model() const { return m_model; }
    Patient* currentPatient() const { return m_currentPatient; }

signals:
    void currentPatientChanged();

private:
    explicit PatientController(QObject *parent = nullptr);

private:
    static PatientController* s_instance;
    PatientModel* m_model;
    Patient* m_currentPatient;
    PatientDao m_patientDao;


};

#endif // PATIENTCONTROLLER_H
