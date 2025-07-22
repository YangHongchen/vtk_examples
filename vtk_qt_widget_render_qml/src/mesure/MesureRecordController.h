#ifndef MESURE_RECORD_CONTROLLER_H
#define MESURE_RECORD_CONTROLLER_H

#include <QObject>

class MesureRecordController : public QObject {
    Q_OBJECT
  public:
    explicit MesureRecordController (QObject *parent = nullptr);

  signals:
};

#endif // MESURE_RECORD_CONTROLLER_H
