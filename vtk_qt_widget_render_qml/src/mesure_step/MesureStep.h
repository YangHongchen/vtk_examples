#ifndef MESURE_STEP_H
#define MESURE_STEP_H
#include "src/database/export.h"

class MESURE_STEP_DLL_EXPORT MesureStep {
  public:
    MesureStep(): id (0) {}
  public:
    long id;
    long patientId;         // 病例Id
    long mesuireId;   // 检测记录id

    QString stepName;       // 测量步骤名称
    QString schematicUrl;   // 示意图
    int status = 0;         // 状态
    int priority = 0;       // 权重（检测顺序）

    QDateTime createTime;   // 创建日期
    QDateTime updateTime;   // 创建日期
    int deleted = 0;        // 软删除标识：0未删除，1已删除
};

QX_REGISTER_HPP_MESURE_STEP (MesureStep, qx::trait::no_base_class_defined, 1)

#endif // MESURE_STEP_H
