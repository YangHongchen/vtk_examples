#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QxOrm.h>
#include <QSqlDatabase>
#include <QMutex>

class DBManager
{
public:
    static DBManager& instance();  // 获取单例
    QSqlDatabase getDatabase();    // 获取数据库连接
    void configureDatabase(const QString& driver, const QString& databaseName, const QString& user = "", const QString& password = "");

private:
    explicit DBManager();
    ~DBManager();
    QSqlDatabase m_db;
    QMutex m_mutex;
};
#endif // DBMANAGER_H
