#ifndef PATIENTCONTROLLER_H
#define PATIENTCONTROLLER_H

#include <QObject>

#include "src/patient/PatientModel.h"
#include "src/patient/PatientDao.h"

class PatientController : public QObject {
    Q_OBJECT
    Q_PROPERTY (PatientModel* model READ model CONSTANT)

  public:

    // 病例列表(查询)
    Q_INVOKABLE void loadPatientsConditional (const QString keyword = "", int page = 1, int pageSize = 10);

  public:
    static PatientController *instance();

    PatientModel *model() const
    {
        return m_model;
    }

  signals:
    void currentPatientChanged();

  private:
    explicit PatientController (QObject *parent = nullptr);
    ~PatientController();

  private:
    static PatientController *s_instance;

    PatientModel *m_model = nullptr;
    PatientDao *m_patientDao = nullptr;

};

#endif // PATIENTCONTROLLER_H
