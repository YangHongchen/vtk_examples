#include "PatientController.h"
#include "src/patient/PatientDao.h"

PatientController *PatientController::s_instance = nullptr;

PatientController *PatientController::instance()
{
    if (!s_instance)
        s_instance = new PatientController();
    return s_instance;
}

PatientController::PatientController (QObject *parent) : QObject (parent),
    m_model (PatientModel::instance()),
    m_patientDao (new PatientDao)
{
    qDebug() << "PatientController 服务初始化";
}

PatientController::~PatientController()
{
    qDebug() << "~销毁PatientController";
}


void PatientController::loadPatientsConditional (const QString keyword, int page, int pageSize)
{
    if (page < 1) page = 1;
    if (pageSize < 1) pageSize = 20;
    QString _keyword = keyword.trimmed();
    auto patientPaginationResult =  m_patientDao->findPatientsCondition (_keyword, page, pageSize);
    // 更新total
    if (patientPaginationResult.total() > 0)
    {
        auto patientList = patientPaginationResult.data();
        auto patientVector = m_patientDao->sharedPtrListToVector (patientList);
        m_model->clear();
        m_model->addPatients (patientVector);
    }
    else
    {
        qWarning() << "未查询到任何的病例数据,keyword=" << keyword;
    }
}
