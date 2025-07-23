#ifndef MESURE_RECORD_CONTROLLER_H
#define MESURE_RECORD_CONTROLLER_H

#include <QObject>
#include "src/mesure/MesureRecordDao.h"
#include "src/mesure/MesureRecordModel.h"
#include "src/patient/PatientModel.h"
#include "src/patient/PatientObject.h"

class MesureRecordController : public QObject {
    Q_OBJECT

  public:
    // 获取静态实例
    static MesureRecordController *instance();

    // 析构函数
    ~MesureRecordController();

  public:

    /**
     * @brief loadMesureRecords
     */
    Q_INVOKABLE void loadMesureRecords (int page, int pageSize);

  private:
    // 单例隐藏构造函数
    explicit MesureRecordController (QObject *parent = nullptr);

  signals:

  public slots:
    // 同步选中病例更新
    void onCurrentPatientChanged (PatientObject *patentObj);

  private:
    static MesureRecordController *s_instance;

    // 检测Dao
    MesureRecordDao *m_mesureRecordDao = nullptr;
    // 检测数据模型
    MesureRecordModel *m_model = nullptr;

    // 选中病例
    PatientModel *m_patientModel = nullptr;
    PatientObject *m_currentPatient = nullptr;
    long m_currentPatientId = 0;
};

#endif // MESURE_RECORD_CONTROLLER_H
