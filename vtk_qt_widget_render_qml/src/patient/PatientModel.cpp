#include "src/patient/PatientModel.h"

PatientModel* PatientModel::m_instance = nullptr;

PatientModel::PatientModel(QObject *parent) : QAbstractListModel(parent)
{

}

// 返回当前页数据行数
PatientModel *PatientModel::instance()
{
    if (!m_instance) {
        m_instance = new PatientModel(); // 或者考虑 QScopedPointer/QSharedPointer
    }
    return m_instance;
}

int PatientModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_visiblePatients.size();
}

// 返回某一行某个 role 对应的数据
QVariant PatientModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_visiblePatients.size())
        return QVariant();
    const Patient &patient = m_visiblePatients.at(index.row());
    switch (role) {
        case IdRole: return QVariant::fromValue<qint64>(patient.id);
        case FirstNameRole: return patient.firstName;
        case LastNameRole: return patient.lastName;
        case FullNameRole: return patient.fullName;
        case GenderRole: return patient.gender;
        case CodeRole: return patient.code;
        case BirthdayRole: return patient.birthday;
        case TechnicianRole: return patient.technician;
        case EmailRole: return patient.email;
        case PhoneRole: return patient.phone;
        case CountryRole: return patient.country;
        case ProvinceRole: return patient.province;
        case CityRole: return patient.city;
        case StreetRole: return patient.street;
        case AddressRole: return patient.address;
        case LabelsRole: return patient.labels;
        case CommentsRole: return patient.comments;
        case AvatarRole: return patient.avatar;
        case CreateTimeRole: return patient.createTime;
        case UpdateTimeRole: return patient.updateTime;
        case LastTestingTimeRole: return patient.lastTestingTime;
        case DeletedRole: return patient.deleted;
        case StatusRole: return patient.status;
        case MaxillaStlUrlRole: return patient.maxillaStlUrl;
        case MaxillaStlThumbnailUrlRole: return patient.maxillaStlThumbnailUrl;
        case MandibleStlUrlRole: return patient.mandibleStlUrl;
        case MandibleStlThumbnailUrlRole: return patient.mandibleStlThumbnailUrl;
        case UpperDentitionStlUrlRole: return patient.upperDentitionStlUrl;
        case UpperDentitionStlThumbnailUrlRole: return patient.upperDentitionStlThumbnailUrl;
        case LowerDentitionStlUrlRole: return patient.lowerDentitionStlUrl;
        case LowerDentitionStlThumbnailUrlRole: return patient.lowerDentitionStlThumbnailUrl;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> PatientModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[FirstNameRole] = "firstName";
    roles[LastNameRole] = "lastName";
    roles[FullNameRole] = "fullName";
    roles[GenderRole] = "gender";
    roles[CodeRole] = "code";
    roles[BirthdayRole] = "birthday";
    roles[TechnicianRole] = "technician";
    roles[EmailRole] = "email";
    roles[PhoneRole] = "phone";
    roles[CountryRole] = "country";
    roles[ProvinceRole] = "province";
    roles[CityRole] = "city";
    roles[StreetRole] = "street";
    roles[AddressRole] = "address";
    roles[LabelsRole] = "labels";
    roles[CommentsRole] = "comments";
    roles[AvatarRole] = "avatar";
    roles[CreateTimeRole] = "createTime";
    roles[UpdateTimeRole] = "updateTime";
    roles[LastTestingTimeRole] = "lastTestingTime";
    roles[DeletedRole] = "deleted";
    roles[StatusRole] = "status";
    roles[MaxillaStlUrlRole] = "maxillaStlUrl";
    roles[MaxillaStlThumbnailUrlRole] = "maxillaStlThumbnailUrl";
    roles[MandibleStlUrlRole] = "mandibleStlUrl";
    roles[MandibleStlThumbnailUrlRole] = "mandibleStlThumbnailUrl";
    roles[UpperDentitionStlUrlRole] = "upperDentitionStlUrl";
    roles[UpperDentitionStlThumbnailUrlRole] = "upperDentitionStlThumbnailUrl";
    roles[LowerDentitionStlUrlRole] = "lowerDentitionStlUrl";
    roles[LowerDentitionStlThumbnailUrlRole] = "lowerDentitionStlThumbnailUrl";
    return roles;
}

// 添加一个病人
void PatientModel::addPatient(const Patient &patient)
{
    beginResetModel();
    m_allPatients.append(patient);
    updateVisiblePatients();
    endResetModel();
    emit dataUpdated();
}

// 批量添加
void PatientModel::addPatients(const QVector<Patient> &patients)
{
    beginResetModel();
    m_allPatients.append(patients);
    updateVisiblePatients();
    endResetModel();
    emit dataUpdated();
}

// 清空所有数据
void PatientModel::clear()
{
    beginResetModel();
    m_allPatients.clear();
    m_visiblePatients.clear();
    endResetModel();
    emit dataUpdated();
}

// 分页逻辑
void PatientModel::updateVisiblePatients()
{
    m_visiblePatients.clear();
    int start = m_currentPage * m_pageSize;
    int end = qMin(start + m_pageSize, m_allPatients.size());
    if (start < m_allPatients.size()) {
        for (int i = start; i < end; ++i) {
            m_visiblePatients.append(m_allPatients[i]);
        }
    }
    emit paginationChanged();
}

// 获取分页信息
int PatientModel::totalCount() const
{
    return m_allPatients.size();
}

int PatientModel::pageSize() const
{
    return m_pageSize;
}

int PatientModel::currentPage() const
{
    return m_currentPage;
}

int PatientModel::pageCount() const
{
    return (m_allPatients.size() + m_pageSize - 1) / m_pageSize;
}

void PatientModel::nextPage()
{
    if (m_currentPage < pageCount() - 1) {
        ++m_currentPage;
        beginResetModel();
        updateVisiblePatients();
        endResetModel();
    }
}

void PatientModel::previousPage()
{
    if (m_currentPage > 0) {
        --m_currentPage;
        beginResetModel();
        updateVisiblePatients();
        endResetModel();
    }
}

void PatientModel::setPage(int page)
{
    if (page >= 0 && page < pageCount()) {
        m_currentPage = page;
        beginResetModel();
        updateVisiblePatients();
        endResetModel();
    }
}

void PatientModel::setPageSize(int size)
{
    if (size > 0 && size != m_pageSize) {
        m_pageSize = size;
        m_currentPage = 0;
        beginResetModel();
        updateVisiblePatients();
        endResetModel();
    }
}

// 获取原始 Patient 对象
Patient PatientModel::get(int index) const
{
    if (index >= 0 && index < m_visiblePatients.size()) {
        return m_visiblePatients.at(index);
    }
    return Patient{};
}
