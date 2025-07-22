#include "PatientController.h"
#include "src/patient/PatientDao.h"

PatientController *PatientController::s_instance = nullptr;

PatientController *PatientController::instance()
{
    if (!s_instance)
        s_instance = new PatientController();
    return s_instance;
}

PatientController::PatientController (QObject *parent) : QObject (parent),
    m_model (PatientModel::instance()),
    m_patientDao (new PatientDao)
{
    qDebug() << "PatientController 服务初始化";
}

PatientController::~PatientController()
{
    qDebug() << "~销毁PatientController";
}

bool PatientController::validateForm (const QVariantMap &formData)
{
    return !formData["firstName"].toString().isEmpty()
           && !formData["lastName"].toString().isEmpty()
           && !formData["birthDay"].toString().isEmpty()
           && !formData["phone"].toString().isEmpty()
           && (formData["gender"].toInt() >= 0
               && formData["gender"].toInt() <= 2
              );
}

void PatientController::loadPatientsConditional (const QString keyword, int page, int pageSize)
{
    if (page < 1) page = 1;
    if (pageSize < 1) pageSize = 20;
    QString _keyword = keyword.trimmed();
    auto patientPaginationResult =  m_patientDao->findPatientsCondition (_keyword, page, pageSize);
    if (patientPaginationResult.total() > 0)
    {
        auto patientList = patientPaginationResult.data();
        auto patientVector = m_patientDao->sharedPtrListToVector (patientList);
        m_model->clear();
        m_model->addPatients (patientVector);
    }
    else
    {
        qWarning() << "未查询到任何的病例数据, keyword=" << keyword;
    }
}

void PatientController::selectPatient (long patientId)
{
    if (patientId < 1)
    {
        qWarning() << "无效的 patientId:" << patientId;
        return;
    }

    std::shared_ptr<Patient> patientPtr = m_patientDao->findOnePatientById (patientId);
    if (!patientPtr)
    {
        qWarning() << "选中病例数据不存在或已删除: id=" << patientId;
        return;
    }

    const Patient& patient = *patientPtr;
    qDebug() << "成功选中病例:" << patient.fullName;

    m_model->setCurrentPatient (patient);
}

bool PatientController::submitPatientFormData (const QVariantMap &formData)
{
    // 数据校验
    if (!validateForm (formData))
    {
        qDebug() << "表单数据校验失败";
        emit error (500, "表单数据校验失败");
        return false;
    }
    // 转换数据
    Patient patient;
    int id = (formData["id"].toInt());
    patient.firstName = (formData["firstName"].toString());
    patient.lastName = (formData["lastName"].toString());
    patient.fullName = patient.lastName + patient.firstName;
    patient.gender = (formData["gender"].toInt());
    patient.phone = (formData["phone"].toString());
    patient.birthday  = QDateTime::fromString (formData["birthDay"].toString(), "yyyy-MM-dd");
    patient.deleted = 0;
    QDateTime currentTime = QDateTime::currentDateTime();  // 默认当前时间
    bool ret  = false;
    if (id > 0)
    {
        patient.updateTime = currentTime;
        ret = m_patientDao->update (patient);
        if (!ret)
        {
            qWarning() << "更新病例数据，失败";
        }
        emit success();
        qWarning() << "chenggong";
    }
    else
    {
        patient.createTime      = currentTime;
        patient.updateTime      = currentTime;
        patient.lastTestingTime = currentTime;
        ret = m_patientDao->save (patient);
        if (!ret)
        {
            qWarning() << "新增病例数据，失败";
        }
        emit success();
    }

}
