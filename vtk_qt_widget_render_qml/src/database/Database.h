#ifndef DATABASE_H
#define DATABASE_H

#include <QxOrm.h>

class Database
{
public:
    static void init(); // 连接 SQLite 数据库
};

#endif // DATABASE_H
