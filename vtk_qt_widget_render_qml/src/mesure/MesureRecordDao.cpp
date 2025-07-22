#include "src/mesure/MesureRecordDao.h"
#include "src/mesure/MesureRecordStatusEnum.h"

PaginationResult<MesureRecord> MesureRecordDao::findPatientsCondition (long patientId, int page, int pageSize)
{
    qx::QxSqlQuery query;
    if (patientId > 0)
    {
        query.where ("patient_id").isEqualTo (QVariant::fromValue (patientId))
             .and_ ("status").isGreaterThan (static_cast<int> (MesureRecordStatusEnum::Pending))
             .and_ ("deleted").isNotEqualTo ("1");
    }
    else
    {
        qCritical() << "未传入病例id，查询的检测结果不予展示";
        return PaginationResult<MesureRecord>();
    }
    return findAllPagination (query, page, pageSize);
}

std::optional<MesureRecord> MesureRecordDao::findOneActiveTesting (long patientId)
{
    qx::QxSqlQuery query;
    if (patientId > 0)
    {
        query.where ("patient_id").isEqualTo (QVariant::fromValue (patientId))
             .and_ ("status").isLessThanOrEqualTo (static_cast<int> (MesureRecordStatusEnum::Testing))
             .and_ ("deleted").isNotEqualTo ("1");
    }
    std::optional<MesureRecord> testing = findOne (query);
    return testing;
}

bool MesureRecordDao::initTesting (long patientId)
{
    if (patientId < 1)
    {
        qDebug() << "初始化检测记录失败，病例id小于1";
        return false;
    }
    MesureRecord record;
    record.patientId = patientId;
    record.status = static_cast<int> (MesureRecordStatusEnum::Pending);
    record.doctor = "admin";
    record.createTime = QDateTime::currentDateTime();
    record.updateTime = QDateTime::currentDateTime();
    return save (record);
}

/**
 * @brief MesureRecordDao::updateStatus
 * @param testingId
 * @param status
 * @return
 */
bool MesureRecordDao::updateStatus (long testingId, int status)
{
    return false;
}
