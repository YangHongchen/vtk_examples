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

void PatientController::selectPatient (long patientId, bool forceUpdate)
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
    m_model->setCurrentPatient (patient, true);
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

bool PatientController::updatePatientStl (const QString stlModelUrl, int stlType)
{
    auto currentPatient = m_model->currentPatient();
    if (currentPatient == nullptr)
    {
        emit error (500, "当前病例不存在或未选中");
        return false;
    }
    long patientId = currentPatient->id();
    if (patientId < 1)
    {
        emit error (500, "病例主键缺失");
        return false;
    }

    std::shared_ptr<Patient> patientPtr = m_patientDao->findById (patientId);
    if (patientPtr == nullptr)
    {
        emit error (500, "病例数据不存在");
        return false;
    }
    Patient patient = *patientPtr;
    if (stlType == 1) // 更新上颌模型
    {
        patient.maxillaStlUrl = stlModelUrl;
        // patient.maxillaStlThumbnailUrl = stlThumbnailUrl;
    }
    else if (stlType == 2) // 更新下颌模型
    {
        patient.mandibleStlUrl = stlModelUrl;
        // patient.mandibleStlThumbnailUrl = stlThumbnailUrl;
    }
    else if (stlType == 3)  // 更新上颌牙弓模型
    {
        patient.upperDentitionStlUrl = stlModelUrl;
        // patient.upperDentitionStlThumbnailUrl = stlThumbnailUrl;
    }
    else if (stlType == 4) // 更新下颌牙弓模型
    {
        patient.lowerDentitionStlUrl = stlModelUrl;
        // patient.lowerDentitionStlThumbnailUrl = stlThumbnailUrl;
    }
    else
    {
        emit error (500, "模型类型错误");
        return false;
    }
    // 更新到数据库
    bool ret = m_patientDao->update (patient);
    if (ret)
    {
        // 触发当前病例数据: (强制刷新)
        selectPatient (patient.id, true);
        return true;
    }
    emit error (500, "更新模型文件失败");
    return false;
}
