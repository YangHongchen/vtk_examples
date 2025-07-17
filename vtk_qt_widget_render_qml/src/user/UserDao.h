#ifndef USERDAO_H
#define USERDAO_H

#include <QObject>
#include "src/user/User.h"
#include "src/database/BaseDao.h"

class UserDao : public BaseDao<User>
{
public:

    // 构造函数
    explicit UserDao() = default;

    // 通过用户名，密码查询用户
    User findOneByUsernameAndPassword (const QString username = "", const QString password = "");

    // 批量新增用户
    bool batchSaveUser (std::vector<User> userList);

};

#endif // USERDAO_H
