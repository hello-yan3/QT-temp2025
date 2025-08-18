#ifndef SQLITEUSER_H
#define SQLITEUSER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#if _MSC_VER >= 1600
#pragma execution_character_set( "utf-8")
#endif

class SQLiteUser
{
public:
    SQLiteUser();

    static QList<QString> data;

    static QSqlDatabase database;
    static void dataBaseInit();  // 数据库初始化
    static bool inquire();       // 数据库获取
    static bool updata(QList<QString> data);   // 数据库更新
};

#endif // SQLITEUSER_H
