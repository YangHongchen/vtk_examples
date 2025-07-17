#ifndef USER_H
#define USER_H

#include "src/database/export.h"

class USER_DLL_EXPORT User
{
public:
   User() : id (0) { }

   long id;
   QString username;
   QString password;
};
QX_REGISTER_HPP_USER (User, qx::trait::no_base_class_defined, 1)

#endif // USER_H
