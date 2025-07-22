#ifndef MESURE_RECORD_DAO_H
#define MESURE_RECORD_DAO_H

#include "src/database/BaseDao.h"
#include "src/mesure/MesureRecord.h"

class MesureRecordDao: public BaseDao<MesureRecord> {
  public:
    explicit MesureRecordDao() = default ;

    // 分页查询检测结果数据
    PaginationResult<MesureRecord> findPatientsCondition (long patientId, int page = 1, int pageSize = 10);

    // 查询一个正在检测的记录
    std::optional<MesureRecord> findOneActiveTesting (long patientId);

    // 初始化检测记录
    bool initTesting (long patientId);

    // 更新检测记录状态
    bool updateStatus (long testingId, int status);
};

#endif // MESURE_RECORD_DAO_H
