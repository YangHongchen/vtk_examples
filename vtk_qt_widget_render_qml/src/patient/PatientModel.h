#ifndef PATIENTMODEL_H
#define PATIENTMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>
#include <QUrl>
#include <QPointer>

#include "src/patient/Patient.h"
#include "src/patient/PatientObject.h"

class PatientModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY (int totalCount READ totalCount NOTIFY paginationChanged)
    Q_PROPERTY (int pageSize READ pageSize NOTIFY paginationChanged)
    Q_PROPERTY (int currentPage READ currentPage NOTIFY paginationChanged)
    Q_PROPERTY (int pageCount READ pageCount NOTIFY paginationChanged)

    // 当前选中病例
    Q_PROPERTY (PatientObject* currentPatient READ currentPatient NOTIFY currentPatientChanged)
    Q_PROPERTY (long currentPatientId READ currentPatientId NOTIFY currentPatientIdChanged)

  public:

    // 核心角色定义（按医疗数据类别分组）
    enum PatientRoles
    {
        IdRole = Qt::UserRole + 1,
        FirstNameRole,
        LastNameRole,
        FullNameRole,
        GenderRole,
        CodeRole,
        BirthdayRole,
        TechnicianRole,
        EmailRole,
        PhoneRole,
        CountryRole,
        ProvinceRole,
        CityRole,
        StreetRole,
        AddressRole,
        LabelsRole,
        CommentsRole,
        AvatarRole,
        CreateTimeRole,
        UpdateTimeRole,
        LastTestingTimeRole,
        DeletedRole,
        StatusRole,
        MaxillaStlUrlRole,
        MaxillaStlThumbnailUrlRole,
        MandibleStlUrlRole,
        MandibleStlThumbnailUrlRole,
        UpperDentitionStlUrlRole,
        UpperDentitionStlThumbnailUrlRole,
        LowerDentitionStlUrlRole,
        LowerDentitionStlThumbnailUrlRole
    };

    // ✅ 单例访问器
    static PatientModel *instance();

    // 分页数据
    int rowCount (const QModelIndex &parent = QModelIndex()) const override;
    QVariant data (const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 列表操作
    void addPatient (const Patient &patient);
    void addPatients (const QVector<Patient> &patients);
    void clear();

    // 当前病例
    PatientObject *currentPatient() const { return m_currentPatientObject; }
    void setCurrentPatient (const Patient &patient);

    long currentPatientId() const
    {
        return m_currentPatientId;
    }

  public:

    // 分页功能
    Q_INVOKABLE int totalCount() const;
    Q_INVOKABLE int pageSize() const;
    Q_INVOKABLE int currentPage() const;
    Q_INVOKABLE int pageCount() const;

    Q_INVOKABLE void nextPage();
    Q_INVOKABLE void previousPage();
    Q_INVOKABLE void setPage (int page);
    Q_INVOKABLE void setPageSize (int size);

    Q_INVOKABLE Patient get (int index) const;

  signals:
    void dataUpdated();
    void stlModelsLoaded (int patientId);
    void paginationChanged();
    void currentPatientChanged (PatientObject* obj);
    void currentPatientIdChanged (long patientId);

  private:
    explicit PatientModel (QObject *parent = nullptr);
    QVector<Patient> m_allPatients;
    QVector<Patient> m_visiblePatients;

    int m_pageSize = 20;
    int m_currentPage = 0;

    void updateVisiblePatients();

    static PatientModel *m_instance;

    // 当前病例
    PatientObject *m_currentPatientObject = nullptr ;
    long m_currentPatientId = 0;

};

#endif // PATIENTMODEL_H
