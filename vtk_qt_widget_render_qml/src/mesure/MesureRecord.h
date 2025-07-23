#ifndef MESURE_RECORD_H
#define MESURE_RECORD_H

#include "src/database/export.h"
#include <QxOrm.h>
#include <QMap>

class MESURE_RECORD_H MesureRecord {
  public:
    MesureRecord(): id (0) {}
    virtual ~MesureRecord() { }

    long id;
    long patientId;         // 病例Id
    QString doctor;         // 操作医生
    int status = 0;         // 状态
    QString comments;       // 描述
    QString reportUrl;      // 报告链接

    QDateTime createTime;   // 创建日期
    QDateTime updateTime;   // 创建日期
    int deleted = 0;        // 软删除标识：0未删除，1已删除
};

QX_REGISTER_HPP_MESURE_RECORD (MesureRecord, qx::trait::no_base_class_defined, 1)

#endif // MESURE_RECORD_H
