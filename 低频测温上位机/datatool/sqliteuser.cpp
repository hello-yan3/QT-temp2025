/* 数据库相关 */
/* u1s1，我觉得，不需要管 */
#include "sqliteuser.h"
#include <QDebug>

QSqlDatabase SQLiteUser::database;
QList<QString> SQLiteUser::data;

SQLiteUser::SQLiteUser()
{

}

void SQLiteUser:: dataBaseInit()
{
    //step1 确认是否存在数据库
    if (QSqlDatabase::contains("setting_connection"))
    {
        database = QSqlDatabase::database("setting_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE","setting_connection");
        database.setDatabaseName("settingDB.db");
    }
    //step2 打开数据库
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return;
    }
    else
    {
        // step3 创建表
        QSqlQuery sql_query(database);
        QString create_sql = "create table setting "
                             "(ID varchar(30) primary key, "
                             "S1 varchar(30),S2 varchar(30),S3 varchar(30),S4 varchar(30),"
                             "S5 varchar(30),S6 varchar(30),S7 varchar(30),S8 varchar(30))";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        }
        else
        {
            qDebug() << "Table created!";
            //step4 插入默认数据
            if(!sql_query.exec("INSERT INTO setting VALUES( \"Cylinder 3\", "
                               "\"测点位置1\", \"测点位置2\", \"测点位置3\", \"测点位置4\","
                               "\"测点位置5\", \"测点位置6\", \"测点位置7\", \"测点位置8\")"))
            {
                qDebug() << sql_query.lastError();
            }
            else
            {
                qDebug() << "插入成功1";
            }

            if(!sql_query.exec("INSERT INTO setting VALUES( \"Cylinder 4\", "
                               "\"测点位置1\", \"测点位置2\", \"测点位置3\", \"测点位置4\","
                               "\"测点位置5\", \"测点位置6\", \"测点位置7\", \"测点位置8\")"))
            {
                qDebug() << sql_query.lastError();
            }
            else
            {
                qDebug() << "插入成功2";
            }
        }
        database.close();
    }
}

bool SQLiteUser:: inquire()
{
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return false;
    }

    QSqlQuery sql_query(database);
    QString select_all_sql = "select * from setting";
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return false;
    }
    else
    {
        data.clear();
        while(sql_query.next())
        {
            for(int i=0;i<9;i++)
                data<<sql_query.value(i).toString();
        }
    }

    database.close();
    return true;
}

bool SQLiteUser::updata(QList<QString> data)
{
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return false;
    }

    QSqlQuery sql_query(database);
    QString update_sql = "update setting set ";
            update_sql.append("ID = \""+data.at(1)+"\"");
            update_sql.append(",S1 = \""+data.at(2)+"\"");
            update_sql.append(",S2 = \""+data.at(3)+"\"");
            update_sql.append(",S3 = \""+data.at(4)+"\"");
            update_sql.append(",S4 = \""+data.at(5)+"\"");
            update_sql.append(",S5 = \""+data.at(6)+"\"");
            update_sql.append(",S6 = \""+data.at(7)+"\"");
            update_sql.append(",S7 = \""+data.at(8)+"\"");
            update_sql.append(",S8 = \""+data.at(9)+"\"");
            update_sql.append(" where ID =\""+data.at(0)+"\"");
    sql_query.prepare(update_sql);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        database.close();
        return false;
    }
    else
    {
//        qDebug() << "updated!";
        database.close();
        return true;
    }

}
