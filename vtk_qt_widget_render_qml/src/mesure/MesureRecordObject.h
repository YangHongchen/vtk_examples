#ifndef MESURE_RECORD_OBJECT_H
#define MESURE_RECORD_OBJECT_H

#include <QObject>
#include <QDateTime>
#include "src/mesure/MesureRecord.h"
#include "src/mesure/MesureRecordStatusEnum.h"
#include <QMap>



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
    // 格式化后的时间字符串（可供 QML Text 等控件绑定）
    Q_PROPERTY (QString formattedCreateTime READ formattedCreateTime NOTIFY createTimeChanged)
    Q_PROPERTY (QString formattedUpdateTime READ formattedUpdateTime NOTIFY updateTimeChanged)

    // 状态：中文
    Q_PROPERTY (QString statusCN READ statusCN  NOTIFY statusChanged)
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

    // 格式化时间输出
    QString formattedCreateTime() const
    {
        return m_createTime.toLocalTime().toString ("yyyy-MM-dd HH:mm:ss");
    }

    QString formattedUpdateTime() const
    {
        return m_updateTime.toLocalTime().toString ("yyyy-MM-dd HH:mm:ss");
    }

    QString statusCN()
    {

        // 创建枚举值到中文描述的映射
        QMap<MesureRecordStatusEnum, QString> statusToChineseMap =
        {
            {MesureRecordStatusEnum::Pending, "待处理"},
            {MesureRecordStatusEnum::Testing, "检测中"},
            {MesureRecordStatusEnum::Completed, "已完成"},
            {MesureRecordStatusEnum::Cancelled, "已取消"}
        };

        MesureRecordStatusEnum enumStatus;

        // 将status转换为中文
        switch (m_status)
        {
        case 0:
            enumStatus = MesureRecordStatusEnum::Pending;
            break;
        case 1:
            enumStatus = MesureRecordStatusEnum::Testing;
            break;
        case 2:
            enumStatus = MesureRecordStatusEnum::Completed;
            break;
        case 3:
            enumStatus = MesureRecordStatusEnum::Cancelled;
            break;
        default:
            enumStatus = MesureRecordStatusEnum::Pending;
            break;
        }
        return statusToChineseMap[enumStatus];
    }

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
