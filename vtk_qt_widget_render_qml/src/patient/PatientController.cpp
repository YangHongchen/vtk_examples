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
    m_currentPatient (nullptr),
    m_patientDao (new PatientDao)
{
}

PatientController::~PatientController()
{
    qDebug() << "~销毁PatientController";
}

void PatientController::loadPatients()
{
    qDebug() << "<<< loadPatients";
    auto list = m_patientDao->findAll();  // 你的数据库查询
    if (!list.isEmpty())
    {
        auto vector = m_patientDao->sharedPtrListToVector (list);
        m_model->addPatients (vector);
        m_currentPatient = &vector.first();  // 默认选择第一个
        emit currentPatientChanged();
    }
}

void PatientController::selectPatient (int row)
{

}
