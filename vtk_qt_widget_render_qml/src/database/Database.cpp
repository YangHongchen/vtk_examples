#include "Database.h"

#include <QxOrm_Impl.h>
#include "src/database/DBManager.h"
#include "src/user/User.h"
#include "src/common/EncryptUtils.h"
#include "src/user/UserDao.h"

void Database::init()
{
    // 避免多线程竞争
    static QMutex mutex;
    QMutexLocker locker (&mutex);
    try
    {
        // 获取数据库链接
        QSqlDatabase db = DBManager::instance().getDatabase();
        QSqlError daoError;


        // 初始化 User 表  ==========
        daoError = qx::dao::create_table<User> (&db);
        if (daoError.type() != QSqlError::NoError)
            qCritical() << "创建 User 表失败:" << daoError.text();
        else
        {
            qDebug() << "User 表创建成功";
            User user;
            user.username = "admin";
            user.password = EncryptUtils::passwordEncrypt ("123456");
            UserDao userDao;
            bool initUserSaveRet = userDao.save (user);
            if (initUserSaveRet)
            {
                qDebug() << "初始化管理员用户成功，用户名:admin, 密码：123456";
            }
        }


    }
    catch (const std::exception& e)
    {
        qCritical() << "数据库表更新时发生异常：" << e.what();
    }
}
