#include "MesureRecordObject.h"

MesureRecordObject::MesureRecordObject (QObject *parent) : QObject{parent}
{

}

void MesureRecordObject::from (const MesureRecord &record)
{
    id          = record.id;
    patientId   = record.patientId;
    doctor      = record.doctor;
    status      = record.status;
    comments    = record.comments;
    reportUrl   = record.reportUrl;
    createTime  = record.createTime;
    updateTime  = record.updateTime;
    deleted     = record.deleted;
}

MesureRecord MesureRecordObject::toRecord() const
{
    MesureRecord record;
    record.id = id;
    record.patientId = patientId;
    record.doctor = doctor;
    record.status = status;
    record.comments = comments;
    record.reportUrl = reportUrl;
    record.createTime = createTime;
    record.updateTime = updateTime;
    record.deleted = deleted;
    return record;

}
