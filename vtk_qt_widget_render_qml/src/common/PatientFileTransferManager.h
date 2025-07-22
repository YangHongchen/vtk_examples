#ifndef PATIENTFILETRANSFERMANAGER_H
#define PATIENTFILETRANSFERMANAGER_H

#include <QObject>
#include "src/patient/PatientModel.h"
#include "src/patient/PatientObject.h"

/**
 * @brief 病例模型上传
 */
class PatientFileTransferManager  : public QObject {
    Q_OBJECT
    Q_PROPERTY (bool uploading READ isUploading NOTIFY uploadingStateChanged)

  public:
    // 获取单例实例对象
    static PatientFileTransferManager *instance();

    ~PatientFileTransferManager();
    bool isUploading() const { return m_uploading; }

    // QML 可调用接口
    Q_INVOKABLE bool copyFileToUploadDirectory (const QString &sourcePath, bool overwrite = false);
    Q_INVOKABLE bool moveFileToDirectory (const QString &sourcePath, const QString &targetDir, bool overwrite = false);

  signals:
    void fileUploadStarted();                             // 上传开始
    void fileUploadCompleted (const QString &targetPath); // 上传完成
    void fileUploadFailed (const QString &errorMessage);  // 上传失败
    void uploadProgressUpdated (double progress);         // 上传进度更新（0.0 ~ 1.0）
    void uploadingStateChanged (bool uploading);          // 上传状态变更

  public slots:
    void onPatientChanged (PatientObject* selectedPatient);

  private:
    explicit PatientFileTransferManager (QObject *parent = nullptr);

  private:
    static PatientFileTransferManager *s_instance;

    bool m_uploading = false;
    QString m_uploadDirectory;

    // 当前病例
    PatientModel *m_patientModel = nullptr;
    PatientObject *m_currentPatient = nullptr;
};

#endif // PATIENTFILETRANSFERMANAGER_H
