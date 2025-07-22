#ifndef MESURE_RECORD_OBJECT_H
#define MESURE_RECORD_OBJECT_H

#include <QObject>
#include "src/mesure/MesureRecord.h"

class MesureRecordObject : public QObject
{
    Q_OBJECT
public:
    explicit MesureRecordObject(QObject *parent = nullptr);

    void from(const MesureRecord &record);

    MesureRecord toRecord() const;

    long id = 0;
    long patientId = 0;
    QString doctor;
    int status = 0;
    QString comments;
    QString reportUrl;
    QDateTime createTime;
    QDateTime updateTime;
    int deleted = 0;

};

#endif // MESURE_RECORD_OBJECT_H
