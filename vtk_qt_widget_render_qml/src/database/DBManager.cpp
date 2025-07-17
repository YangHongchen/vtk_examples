#include "DBManager.h"

DBManager::DBManager()
{
    // 默认使用 SQLite
    configureDatabase("QSQLITE", "Users.db", "root", "123456");
}

DBManager& DBManager::instance() {
    static DBManager instance;
    return instance;
}

DBManager::~DBManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void DBManager::configureDatabase(const QString& driver, const QString& databaseName, const QString& user, const QString& password) {
    QMutexLocker locker(&m_mutex);
    m_db = QSqlDatabase::addDatabase(driver);
    m_db.setDatabaseName(databaseName);
    if (!user.isEmpty()) {
        m_db.setUserName(user);
        qDebug() << "设置数据库user:" << user;
    }
    if (!password.isEmpty()) {
        m_db.setPassword(password);
        qDebug() << "设置数据库password:" << password;
    }
    if (!m_db.open()) {
        qCritical() << "数据库打开失败:" << m_db.lastError().text();
    } else {
        qDebug() << "数据库连接成功:" << databaseName;
    }
}

QSqlDatabase DBManager::getDatabase() {
    QMutexLocker locker(&m_mutex);
    return m_db;
}
