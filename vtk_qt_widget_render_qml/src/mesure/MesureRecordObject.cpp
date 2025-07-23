#include "src/mesure/MesureRecordObject.h"

MesureRecordObject::MesureRecordObject (QObject *parent)
    : QObject (parent)
{
}

MesureRecordObject::MesureRecordObject (const MesureRecord &record, QObject *parent)
    : QObject (parent)
{
    from (record);
}

long MesureRecordObject::id() const { return m_id; }
long MesureRecordObject::patientId() const { return m_patientId; }
QString MesureRecordObject::doctor() const { return m_doctor; }
int MesureRecordObject::status() const { return m_status; }
QString MesureRecordObject::comments() const { return m_comments; }
QString MesureRecordObject::reportUrl() const { return m_reportUrl; }
QDateTime MesureRecordObject::createTime() const { return m_createTime; }
QDateTime MesureRecordObject::updateTime() const { return m_updateTime; }
int MesureRecordObject::deleted() const { return m_deleted; }

void MesureRecordObject::setId (long id)
{
    if (m_id == id) return;
    m_id = id;
    emit idChanged();
}

void MesureRecordObject::setPatientId (long patientId)
{
    if (m_patientId == patientId) return;
    m_patientId = patientId;
    emit patientIdChanged();
}

void MesureRecordObject::setDoctor (const QString &doctor)
{
    if (m_doctor == doctor) return;
    m_doctor = doctor;
    emit doctorChanged();
}

void MesureRecordObject::setStatus (int status)
{
    if (m_status == status) return;
    m_status = status;
    emit statusChanged();
}

void MesureRecordObject::setComments (const QString &comments)
{
    if (m_comments == comments) return;
    m_comments = comments;
    emit commentsChanged();
}

void MesureRecordObject::setReportUrl (const QString &reportUrl)
{
    if (m_reportUrl == reportUrl) return;
    m_reportUrl = reportUrl;
    emit reportUrlChanged();
}

void MesureRecordObject::setCreateTime (const QDateTime &createTime)
{
    if (m_createTime == createTime) return;
    m_createTime = createTime;
    emit createTimeChanged();
}

void MesureRecordObject::setUpdateTime (const QDateTime &updateTime)
{
    if (m_updateTime == updateTime) return;
    m_updateTime = updateTime;
    emit updateTimeChanged();
}

void MesureRecordObject::setDeleted (int deleted)
{
    if (m_deleted == deleted) return;
    m_deleted = deleted;
    emit deletedChanged();
}

void MesureRecordObject::from (const MesureRecord &record)
{
    setId (record.id);
    setPatientId (record.patientId);
    setDoctor (record.doctor);
    setStatus (record.status);
    setComments (record.comments);
    setReportUrl (record.reportUrl);
    setCreateTime (record.createTime);
    setUpdateTime (record.updateTime);
    setDeleted (record.deleted);
}

MesureRecord MesureRecordObject::toRecord() const
{
    MesureRecord record;
    record.id = m_id;
    record.patientId = m_patientId;
    record.doctor = m_doctor;
    record.status = m_status;
    record.comments = m_comments;
    record.reportUrl = m_reportUrl;
    record.createTime = m_createTime;
    record.updateTime = m_updateTime;
    record.deleted = m_deleted;
    return record;
}
