#ifndef MESURE_RECORD_CONTROLLER_H
#define MESURE_RECORD_CONTROLLER_H

#include <QObject>

class MesureRecordController : public QObject {
    Q_OBJECT

  public:
    static MesureRecordController *instance();
    // 析构函数
    ~MesureRecordController();
  private:
    explicit MesureRecordController (QObject *parent = nullptr);


  private:
    static MesureRecordController *s_instance;

};

#endif // MESURE_RECORD_CONTROLLER_H
