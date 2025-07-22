#include "PatientController.h"
#include "src/patient/PatientDao.h"
#include "src/common/VtkStlPreviewGenerator.h"
#include <QThread>

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

bool PatientController::validateForm (const QVariantMap &formData)
{
    return !formData["firstName"].toString().isEmpty()
           && !formData["lastName"].toString().isEmpty()
           && !formData["birthDay"].toString().isEmpty()
           && !formData["phone"].toString().isEmpty()
           && (formData["gender"].toInt() >= 0
               && formData["gender"].toInt() <= 2
              );
}



void PatientController::loadPatientsConditional (const QString keyword, int page, int pageSize)
{
    if (page < 1) page = 1;
    if (pageSize < 1) pageSize = 20;
    QString _keyword = keyword.trimmed();
    auto patientPaginationResult =  m_patientDao->findPatientsCondition (_keyword, page, pageSize);
    if (patientPaginationResult.total() > 0)
    {
        auto patientList = patientPaginationResult.data();
        auto patientVector = m_patientDao->sharedPtrListToVector (patientList);
        m_model->clear();
        m_model->addPatients (patientVector);
    }
    else
    {
        qWarning() << "未查询到任何的病例数据, keyword=" << keyword;
    }
}

void PatientController::selectPatient (long patientId, bool forceUpdate)
{
    if (patientId < 1)
    {
        qWarning() << "无效的 patientId:" << patientId;
        return;
    }
    std::shared_ptr<Patient> patientPtr = m_patientDao->findOnePatientById (patientId);
    if (!patientPtr)
    {
        qWarning() << "选中病例数据不存在或已删除: id=" << patientId;
        return;
    }
    const Patient& patient = *patientPtr;
    qDebug() << "成功选中病例:" << patient.fullName;
    m_model->setCurrentPatient (patient, true);
}

bool PatientController::submitPatientFormData (const QVariantMap &formData)
{
    if (!validateForm (formData))
    {
        emit error (500, "表单数据校验失败");
        return false;
    }

    Patient patient;
    patient.id        = formData.value ("id").toInt();
    patient.firstName = formData.value ("firstName").toString();
    patient.lastName  = formData.value ("lastName").toString();
    patient.fullName  = patient.lastName + patient.firstName;
    patient.gender    = formData.value ("gender").toInt();
    patient.phone     = formData.value ("phone").toString();
    patient.birthday  = QDateTime::fromString (formData.value ("birthDay").toString(), "yyyy-MM-dd");
    patient.deleted   = 0;

    const QDateTime currentTime = QDateTime::currentDateTime();
    patient.updateTime = currentTime;

    bool ret = false;
    if (patient.id > 0)
    {
        ret = m_patientDao->update (patient);
        if (!ret)
        {
            emit error (500, "更新病例失败");
            return false;
        }
    }
    else
    {
        patient.createTime      = currentTime;
        patient.lastTestingTime = currentTime;
        ret = m_patientDao->save (patient);
        if (!ret)
        {
            emit error (500, "新增病例失败");
            return false;
        }
    }
    emit success();
    return true;
}

bool PatientController::updatePatientStl (const QString &stlModelUrl, int stlType)
{
    PatientObject* patient = m_model->currentPatient();
    if (!patient || patient->id() < 1)
    {
        emit error (500, "当前病例不存在或未选中");
        return false;
    }
    std::shared_ptr<Patient> patientPtr =  m_patientDao->findOnePatientById (patient->id());
    if (patientPtr == nullptr)
    {
        emit error (500, "当前病例不存在或未选中");
        return false;
    }
    // 直接修改指针指向的对象
    switch (stlType)
    {
    case 1: patientPtr->maxillaStlUrl = stlModelUrl; break;
    case 2: patientPtr->mandibleStlUrl = stlModelUrl; break;
    case 3: patientPtr->upperDentitionStlUrl = stlModelUrl; break;
    case 4: patientPtr->lowerDentitionStlUrl = stlModelUrl; break;
    default:
        emit error (500, "模型类型错误");
        return false;
    }
    if (m_patientDao->update (*patientPtr))
    {
        //
        startStlThumbnailGeneration (stlModelUrl, stlType);

        // TODO: 监听生成缩略图成功后，再更新
        qDebug() << "开始生成缩略图";
        return true;
    }
    emit error (500, "模型文件更新失败");
    return false;
}

void PatientController::startStlThumbnailGeneration (const QString &stlPath, int stlType)
{
    QThread* workerThread = new QThread (this);
    VtkStlPreviewGenerator* generator = new VtkStlPreviewGenerator();
    generator->moveToThread (workerThread);
    // 基于上传类型，当前病例生成缩略图存放路径
    connect (workerThread, &QThread::started, [generator, stlPath, stlType]()
    {
        generator->startGenerate (stlPath, 1024, stlType);
    });
    connect (generator, &VtkStlPreviewGenerator::thumbnailReady, this, [workerThread,
             generator ] (const QString & imagePath, int stlType)
    {
        qDebug() << "生成的缩略图路径：" << imagePath << ", 模型类型：stlType=" << stlType;
        workerThread->quit();
    });
    connect (workerThread, &QThread::finished, generator, &QObject::deleteLater);
    connect (workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

