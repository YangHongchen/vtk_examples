#include "src/patient/PatientDao.h"

std::shared_ptr<Patient> PatientDao::findOnePatientById (long id)
{
    return findById (id);
}

std::optional<Patient> PatientDao::findOneByPhone (const QString &phone)
{
    std::optional<Patient> patient;
    if (phone.isEmpty())
    {
        return std::nullopt;
    }
    qx::QxSqlQuery query;
    query.where ("phone").isEqualTo (phone);
    return findOne (query);
}

QList<std::shared_ptr<Patient>> PatientDao::findAllPatients()
{
    qDebug() << "PATIENT_DAO::findAll()";
    return findAll();
}

bool PatientDao::savePatient (Patient &patient)
{
    const long patientId = patient.id;
    bool operationResult = false;

    // 更新现有记录
    if (patientId > 0)
    {
        auto existingPatient = findOnePatientById (patientId);
        if (existingPatient)
        {
            // 确保更新前验证数据完整性
            if (validatePatient (patient))
            {
                operationResult = update (patient);
            }
        }
        // 如果记录不存在，则转为插入新记录
        else
        {
            operationResult = save (patient);
        }
    }
    // 插入新记录
    else
    {
        operationResult = save (patient);
    }

    return operationResult;
}

PaginationResult<Patient> PatientDao::findPatientsCondition (const QString keyword, int page, int pageSize)
{
    qx::QxSqlQuery query;
    if (!keyword.isEmpty() && keyword.length() > 0)
    {
        // 这里添加查询条件并绑定参数 Like 要加上通配符%
        query.where ("first_name").like ("%" + keyword + "%")
             .or_ ("last_name").like ("%" + keyword + "%")
             .or_ ("full_name").like ("%" + keyword + "%")
             .or_ ("phone").like ("%" + keyword + "%");
    }
    // createTime, updateTime 倒序
    query.orderDesc ("update_time");
    return findAllPagination (query, page, pageSize);
}

bool PatientDao::updateStlPreview (long patientId, const QString &previewUrl, int stlType)
{
    // 更新现有记录
    if (patientId < 1)
    {
        qDebug() << "patientId 为空";
        return false;
    }
    if (stlType < 1 || stlType > 4)
    {
        qDebug() << "STL 类型错误";
        return false;
    }
    auto patient = findOnePatientById (patientId);
    if (patient == nullptr)
    {
        qDebug() << "未查询到对应的病例";
        return false;
    }
    switch (stlType)
    {
    case 1: patient->maxillaStlThumbnailUrl = previewUrl; break;
    case 2: patient->mandibleStlThumbnailUrl = previewUrl; break;
    case 3: patient->upperDentitionStlThumbnailUrl = previewUrl; break;
    case 4: patient->lowerDentitionStlThumbnailUrl = previewUrl; break;
    default:
        qWarning() << "不支持的 STL 类型:" << stlType;
        return false;
    }
    return update (*patient);
}

void PatientDao::test()
{
    qDebug() << "test";
}

bool PatientDao::validatePatient (Patient &patient)
{
    if (patient.lastName.trimmed().isEmpty())
    {
        qWarning() << "患者姓氏不能为空";
        return false;
    }
    if (patient.phone.trimmed().isEmpty())
    {
        qWarning() << "手机号不能为空";
        return false;
    }
    if (patient.gender != 0 && patient.gender != 1 && patient.gender != 2)
    {
        qWarning() << "性别不能为空";
        return false;
    }
    if (!patient.birthday.isValid())
    {
        qWarning() << "出生日期无效";
        return false;
    }
    if (checkExists (patient))
    {
        qWarning() << "手机号已被其他患者使用";
        return false;
    }
    return true;  // 所有校验通过
}

bool PatientDao::checkExists (const Patient &patient)
{
    if (patient.phone.isEmpty())
    {
        return false;
    }

    // 2. 直接使用optional的布尔转换，无需显式has_value()
    if (auto existingPatient = findOneByPhone (patient.phone))
    {
        // 3. 单行返回比较结果
        return existingPatient->id != patient.id;
    }

    return false;
}
