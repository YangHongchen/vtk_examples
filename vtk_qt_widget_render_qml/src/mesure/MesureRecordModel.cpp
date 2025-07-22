#include "src/mesure/MesureRecordModel.h"

MesureRecordModel *MesureRecordModel::s_instance = nullptr;

MesureRecordModel::MesureRecordModel (QObject *parent) : QAbstractListModel (parent)
{
    m_currentRecord = new MesureRecordObject (this);
}

MesureRecordModel *MesureRecordModel::instance()
{
    if (!s_instance)
    {
        s_instance = new MesureRecordModel();
    }
    return s_instance;
}

int MesureRecordModel::rowCount (const QModelIndex &parent) const
{
    Q_UNUSED (parent)
    return m_visibleRecords.size();
}

QVariant MesureRecordModel::data (const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_visibleRecords.size())
        return QVariant();

    const MesureRecord &r = m_visibleRecords[index.row()];

    switch (role)
    {
    case IdRole: return QVariant::fromValue <qint64> (r.id);
    case PatientIdRole: return QVariant::fromValue <qint64> (r.patientId);
    case DoctorRole: return r.doctor;
    case StatusRole: return r.status;
    case CommentsRole: return r.comments;
    case ReportUrlRole: return r.reportUrl;
    case CreateTimeRole: return r.createTime;
    case UpdateTimeRole: return r.updateTime;
    case DeletedRole: return r.deleted;
    default: return QVariant();
    }
}

QHash<int, QByteArray> MesureRecordModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole]           = "id";
    roles[PatientIdRole]    = "patientId";
    roles[DoctorRole]       = "doctor";
    roles[StatusRole]       = "status";
    roles[CommentsRole]     = "comments";
    roles[ReportUrlRole]    = "reportUrl";
    roles[CreateTimeRole]   = "createTime";
    roles[UpdateTimeRole]   = "updateTime";
    roles[DeletedRole]      = "deleted";
    return roles;
}

void MesureRecordModel::addRecord (const MesureRecord &record)
{
    m_allRecords.append (record);
    updateVisible();
    emit dataUpdated();
}


void MesureRecordModel::setRecords (const QVector<MesureRecord> &records)
{
    beginResetModel();
    m_allRecords = records;
    updateVisible();
    endResetModel();
    emit dataUpdated();
}


void MesureRecordModel::clear()
{
    beginResetModel();
    m_allRecords.clear();
    m_visibleRecords.clear();
    endResetModel();
    emit dataUpdated();
}

MesureRecordObject *MesureRecordModel::currentRecord() const
{
    return m_currentRecord;
}

void MesureRecordModel::setCurrentRecord (const MesureRecord &record)
{
    if (m_currentRecord)
    {
        m_currentRecord->from (record);
        emit currentRecordChanged();
    }
}


MesureRecord MesureRecordModel::get (int index) const
{
    if (index >= 0 && index < m_visibleRecords.size())
        return m_visibleRecords[index];
    return MesureRecord{};
}


// 分页实现 =====================
int MesureRecordModel::totalCount() const
{
    return m_allRecords.size();
}

int MesureRecordModel::pageSize() const
{
    return m_pageSize;
}

int MesureRecordModel::currentPage() const
{
    return m_currentPage;
}

int MesureRecordModel::pageCount() const
{
    return (totalCount() + m_pageSize - 1) / m_pageSize;
}

void MesureRecordModel::nextPage()
{
    if (m_currentPage + 1 < pageCount())
    {
        m_currentPage++;
        updateVisible();
        emit paginationChanged();
    }
}

void MesureRecordModel::previousPage()
{
    if (m_currentPage > 0)
    {
        m_currentPage--;
        updateVisible();
        emit paginationChanged();
    }
}

void MesureRecordModel::setPage (int page)
{
    if (page >= 0 && page < pageCount())
    {
        m_currentPage = page;
        updateVisible();
        emit paginationChanged();
    }
}

void MesureRecordModel::setPageSize (int size)
{
    if (size > 0)
    {
        m_pageSize = size;
        m_currentPage = 0;
        updateVisible();
        emit paginationChanged();
    }
}

void MesureRecordModel::updateVisible()
{
    beginResetModel();

    const int start = m_currentPage * m_pageSize;
    const int end = qMin (start + m_pageSize, m_allRecords.size());
    m_visibleRecords = m_allRecords.mid (start, end - start);

    endResetModel();
}
