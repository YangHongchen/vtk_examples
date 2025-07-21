#ifndef PATIENTCONTROLLER_H
#define PATIENTCONTROLLER_H

#include <QObject>
#include <QVariantMap>

#include "src/patient/PatientModel.h"
#include "src/patient/PatientDao.h"

class PatientController : public QObject {
    Q_OBJECT
    Q_PROPERTY (PatientModel* model READ model CONSTANT)

  public:

    // 病例列表(查询)
    Q_INVOKABLE void loadPatientsConditional (const QString keyword = "", int page = 1, int pageSize = 10);
    // 切换病例
    Q_INVOKABLE void selectPatient (long patientId);
    // 提交病例编辑
    Q_INVOKABLE bool submitPatientFormData (const QVariantMap &formData);

  public:
    static PatientController *instance();

    PatientModel *model() const
    {
        return m_model;
    }

  signals:
    void currentPatientChanged();
    void error (int code, const QString &message);
    void success();

  private:
    explicit PatientController (QObject *parent = nullptr);
    ~PatientController();

    // 校验病例表单提交的数据
    bool validateForm (const QVariantMap &formData);

  private:
    static PatientController *s_instance;

    PatientModel *m_model = nullptr;
    PatientDao *m_patientDao = nullptr;

};

#endif // PATIENTCONTROLLER_H
