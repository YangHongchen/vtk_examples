#ifndef MESURE_STEP_DAO_H
#define MESURE_STEP_DAO_H

#include "src/mesure_step/MesureStep.h"
#include "src/database/BaseDao.h"


class MesureStepDao: BaseDao<MesureStep> {

  public:
    explicit MesureStepDao() = default;

    // 查询分布列表
    QList<std::shared_ptr<MesureStep>> findAllStepByMesureId (long mesureId);

    // 初始化测量步骤
    bool initSteps (long mesureId, long patientId = 0);

    // 查询所属测试的步骤是否存在(待检测)
    bool checkStepExits (long mesureId);

    // 查询当前激活的步骤
    std::optional<MesureStep> findOneByMesureIdActive (long mesureId);

    // 当前步骤（状态:完成），进入下一步（状态:测量中）
    bool updateStepNext (long mesureId, int currentStep);

    // 更新状态
    bool updateStatus (long mesureId, int currentStepIndex, int status);

    // 通过检测记录id和步骤id，获取检测步骤数据
    std::optional<MesureStep> getStepByMesureIdAndPriority (long mesureId, int priority);

    // 检测是否测量完成或取消
    int checkMesureStepComplete (long mesureId);

    // 查询检查记录步骤数量
    long countByMesureId (long mesureId);

};

#endif // MESURE_STEP_DAO_H
