#include "src/user/User.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_USER (User)

namespace qx {
template <> void register_class (QxClass<User> &t)
{
    // 注册 User::id <=> 数据库中的主键
    t.id (&User::id, "id");

    // 注册 User::username 属性，使用的 key 是 username 是 1。
    t.data (&User::username, "username");

    // 注册 User::password 属性，使用的 key 是 password
    t.data (&User::password, "password");
}
}
