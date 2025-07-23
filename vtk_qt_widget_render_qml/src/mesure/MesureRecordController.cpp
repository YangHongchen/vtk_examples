#include "src/mesure/MesureRecordController.h"

MesureRecordController *MesureRecordController::s_instance = nullptr;

MesureRecordController *MesureRecordController::instance()
{
    if (!s_instance)
    {
        s_instance = new MesureRecordController();
    }
    return s_instance;
}

MesureRecordController::MesureRecordController (QObject *parent): QObject{parent}
{
    m_mesureRecordDao = new MesureRecordDao;
    m_model =  MesureRecordModel::instance();
    m_patientModel = PatientModel::instance();

    // 订阅PatientModel 病例变化，同步到本类
    connect (m_patientModel, &PatientModel::currentPatientChanged, this, &MesureRecordController::onCurrentPatientChanged);

}

void MesureRecordController::onCurrentPatientChanged (PatientObject *patentObj)
{
    QString currentPatientName = (patentObj == nullptr) ? patentObj->fullName() : "空";
    m_currentPatientId = (patentObj == nullptr) ? patentObj->id() : 0;
    qDebug() << "MesureRecordController:: 监听到病例更新" << currentPatientName ;
    m_currentPatient = patentObj;
}

MesureRecordController::~MesureRecordController()
{
    qDebug() << "~释放 MesureRecordController";
    if (m_model)
    {
        m_model->deleteLater();
    }
    if (m_mesureRecordDao)
    {
        delete m_mesureRecordDao;
        m_mesureRecordDao = nullptr;
    }
    if (m_patientModel)
    {
        m_patientModel = nullptr;
    }
    if (m_currentPatient)
    {
        m_currentPatient = nullptr;
    }
}

void MesureRecordController::loadMesureRecords (long patientId, int page, int pageSize)
{
    auto paginationResult = m_mesureRecordDao->findPatientsCondition (patientId, page, pageSize);
    int total = paginationResult.total();
    int pageCount = paginationResult.pageCount();
    QList<std::shared_ptr<MesureRecord>> dataList = paginationResult.data();

    qDebug() << "分页结果，TOTAL:" << total;
    qDebug() << "分页结果，PAGE_COUNT:" << pageCount;
    qDebug() << "分页结果，PAGE_ROWS：" << dataList.size();
}

