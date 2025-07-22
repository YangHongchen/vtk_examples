#include "src/common/PatientFileTransferManager.h"
#include <QDir>
#include "src/config/ConfigManager.h"
#include "src/common/FileUtils.h"

// instance
PatientFileTransferManager *PatientFileTransferManager::s_instance = nullptr;

PatientFileTransferManager *PatientFileTransferManager::instance()
{
    if (!s_instance)
        s_instance = new PatientFileTransferManager();
    return s_instance;
}

PatientFileTransferManager::PatientFileTransferManager (QObject *parent)
{
    m_patientModel = PatientModel::instance();

    // 同步监听和更新当前病例
    connect (m_patientModel, &PatientModel::currentPatientChanged, this,
             &PatientFileTransferManager::onPatientChanged);
}

PatientFileTransferManager::~PatientFileTransferManager()
{
    m_patientModel = nullptr;
    m_currentPatient = nullptr;
}

bool PatientFileTransferManager::copyFileToUploadDirectory (const QString &sourcePath, bool overwrite)
{
    qDebug() << "copyFileToUploadDirectory, sourcePath=" << sourcePath << ", overwrite=" << overwrite;

    auto fail = [this] (const QString & reason)
    {
        emit fileUploadFailed (reason);
        m_uploading = false;
        return false;
    };
    qDebug() << 111;

    if (!QFile::exists (sourcePath))
        return fail ("源文件不存在，无法上传");
    qDebug() << 222;
    if (!m_currentPatient || m_currentPatient->id() <= 0)
        return fail ("当前病例未设置或 ID 无效，无法上传文件");
    qDebug() << 333;
    const QString phone = m_currentPatient->phone().trimmed();
    if (phone.isEmpty())
        return fail ("当前病例电话号码为空，无法上传文件");
    qDebug() << 444;
    QString appConfigPath = ConfigManager::instance()->getConfig ("Facebow/AppConfigLocation");
    if (appConfigPath.isEmpty())
    {
        ConfigManager::instance()->reloadAppConfig();
        appConfigPath = ConfigManager::instance()->getConfig ("Facebow/AppConfigLocation");
    }
    qDebug() << 555;
    if (appConfigPath.isEmpty())
        return fail ("AppConfig路径未设置，无法上传文件");
    qDebug() << 666;

    const QString targetDirPath = appConfigPath + "/" + phone;
    QDir targetDir (targetDirPath);
    if (!targetDir.exists() && !targetDir.mkpath ("."))
        return fail ("创建目标目录失败: " + targetDirPath);
    qDebug() << 777;

    const QString originalFileName = QFileInfo (sourcePath).fileName();
    QString targetPath = FileUtils::generateRandomFileName (targetDirPath + "/" + originalFileName);

    if (QFile::exists (targetPath) && !overwrite)
        return fail ("目标文件已存在，设置 overwrite=true 可覆盖: " + targetPath);
    qDebug() << 888;
    QFile sourceFile (sourcePath);
    if (!sourceFile.copy (targetPath))
        return fail ("文件复制失败: " + sourceFile.errorString());

    if (!QFile::exists (targetPath))
        return fail ("复制后未找到目标文件，可能写入失败");
    qDebug() << 999;
    qDebug() << "文件上传完成，路径：" << targetPath;

    emit fileUploadCompleted (targetPath);
    m_uploading = false;
    return true;
}

bool PatientFileTransferManager::moveFileToDirectory (const QString &sourcePath, const QString &targetDir,
        bool overwrite)
{
    if (copyFileToUploadDirectory (sourcePath, overwrite))
    {
        QFile::remove (sourcePath);
        return true;
    }
    return false;
}

void PatientFileTransferManager::onPatientChanged (PatientObject *newPatient)
{
    m_currentPatient = newPatient;
}
