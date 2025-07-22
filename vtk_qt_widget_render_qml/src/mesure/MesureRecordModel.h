#ifndef MESURERECORDMODEL_H
#define MESURERECORDMODEL_H

#include <QAbstractListModel>
#include "src/mesure/MesureRecord.h"
#include "src/mesure/MesureRecordObject.h"

class MesureRecordModel: public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY (int totalCount READ totalCount NOTIFY paginationChanged)
    Q_PROPERTY (int pageSize READ pageSize NOTIFY paginationChanged)
    Q_PROPERTY (int currentPage READ currentPage NOTIFY paginationChanged)
    Q_PROPERTY (int pageCount READ pageCount NOTIFY paginationChanged)

    Q_PROPERTY (MesureRecordObject* currentRecord READ currentRecord NOTIFY currentRecordChanged)

  public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        PatientIdRole,
        DoctorRole,
        StatusRole,
        CommentsRole,
        ReportUrlRole,
        CreateTimeRole,
        UpdateTimeRole,
        DeletedRole
    };

    static MesureRecordModel *instance();

    int rowCount (const QModelIndex &parent = QModelIndex()) const override;
    QVariant data (const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;


    void addRecord (const MesureRecord &record);
    void setRecords (const QVector<MesureRecord> &records);
    void clear();

    MesureRecordObject *currentRecord() const;
    void setCurrentRecord (const MesureRecord &record);

    Q_INVOKABLE MesureRecord get (int index) const;

    // 分页
    Q_INVOKABLE int totalCount() const;
    Q_INVOKABLE int pageSize() const;
    Q_INVOKABLE int currentPage() const;
    Q_INVOKABLE int pageCount() const;
    Q_INVOKABLE void nextPage();
    Q_INVOKABLE void previousPage();
    Q_INVOKABLE void setPage (int page);
    Q_INVOKABLE void setPageSize (int size);

  signals:
    void dataUpdated();
    void paginationChanged();
    void currentRecordChanged();

  private:
    explicit MesureRecordModel (QObject *parent = nullptr);
    void updateVisible();

    static MesureRecordModel *m_instance;
    QVector<MesureRecord> m_allRecords;
    QVector<MesureRecord> m_visibleRecords;

    int m_pageSize = 20;
    int m_currentPage = 0;

    MesureRecordObject *m_currentRecord = nullptr;

};

#endif // MESURERECORDMODEL_H
