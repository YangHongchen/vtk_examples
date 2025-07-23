#ifndef MESURE_RECORD_OBJECT_H
#define MESURE_RECORD_OBJECT_H

#include <QObject>
#include <QDateTime>
#include "src/mesure/MesureRecord.h"

class MesureRecordObject : public QObject {
    Q_OBJECT

    Q_PROPERTY (long id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY (long patientId READ patientId WRITE setPatientId NOTIFY patientIdChanged)
    Q_PROPERTY (QString doctor READ doctor WRITE setDoctor NOTIFY doctorChanged)
    Q_PROPERTY (int status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY (QString comments READ comments WRITE setComments NOTIFY commentsChanged)
    Q_PROPERTY (QString reportUrl READ reportUrl WRITE setReportUrl NOTIFY reportUrlChanged)
    Q_PROPERTY (QDateTime createTime READ createTime WRITE setCreateTime NOTIFY createTimeChanged)
    Q_PROPERTY (QDateTime updateTime READ updateTime WRITE setUpdateTime NOTIFY updateTimeChanged)
    Q_PROPERTY (int deleted READ deleted WRITE setDeleted NOTIFY deletedChanged)

  public:
    explicit MesureRecordObject (QObject *parent = nullptr);
    explicit MesureRecordObject (const MesureRecord &record, QObject *parent = nullptr);

    // Getters
    long id() const;
    long patientId() const;
    QString doctor() const;
    int status() const;
    QString comments() const;
    QString reportUrl() const;
    QDateTime createTime() const;
    QDateTime updateTime() const;
    int deleted() const;

    // Setters
    void setId (long id);
    void setPatientId (long patientId);
    void setDoctor (const QString &doctor);
    void setStatus (int status);
    void setComments (const QString &comments);
    void setReportUrl (const QString &reportUrl);
    void setCreateTime (const QDateTime &createTime);
    void setUpdateTime (const QDateTime &updateTime);
    void setDeleted (int deleted);

    Q_INVOKABLE void from (const MesureRecord &record);
    Q_INVOKABLE MesureRecord toRecord() const;

  signals:
    void idChanged();
    void patientIdChanged();
    void doctorChanged();
    void statusChanged();
    void commentsChanged();
    void reportUrlChanged();
    void createTimeChanged();
    void updateTimeChanged();
    void deletedChanged();

  private:
    long m_id = 0;
    long m_patientId = 0;
    QString m_doctor;
    int m_status = 0;
    QString m_comments;
    QString m_reportUrl;
    QDateTime m_createTime;
    QDateTime m_updateTime;
    int m_deleted = 0;
};

#endif // MESURE_RECORD_OBJECT_H
