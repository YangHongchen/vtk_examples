#include "UserDao.h"
#include "src/common/EncryptUtils.h"

User UserDao::findOneByUsernameAndPassword (const QString username, const QString password)
{
    User user;
    // 通过用户名 查询数据库
    qx::QxSqlQuery query;
    query.where ("username").isEqualTo (username);
    PaginationResult<User> userFetchResult = findAllPagination (query);
    if (userFetchResult.total() > 0)
    {
        qDebug() << "查询到用户";
        QList<std::shared_ptr<User>> userListData = userFetchResult.data();
        if (!userListData.isEmpty())
        {
            auto userPtr = userListData.first();
            qDebug() << "查询到用户(id):" << userPtr->id;
            qDebug() << "查询到用户(firstMame):" << userPtr->username;
            qDebug() << "查询到用户(lastName):" << userPtr->password;
            // 校验密码
            QString passwordEncrypted = EncryptUtils::passwordEncrypt (password);
            if (passwordEncrypted == userPtr->password)
            {
                qDebug() << "密码校验通过" << passwordEncrypted;
                user.username = userPtr->username;
                user.id       = userPtr->id;
                user.password = userPtr->password;
            }
            else
            {
                qDebug() << "密码错误, orginPassword(Encrypted)=" << userPtr->password << "加密的密码(基于明文)：" << passwordEncrypted;
            }
        }
        else
        {
            qDebug() << "未查询到用户";
        }
    }
    else
    {
        qDebug() << "未查询到用户";
    }
    return user;
}

bool UserDao::batchSaveUser (std::vector<User> userList)
{
    qDebug() << "UserDao batch Save...";
    return saveBatch (userList);
}
