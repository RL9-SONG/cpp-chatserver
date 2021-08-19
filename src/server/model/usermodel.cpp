#include "usermodel.hpp"
#include "db.h"
#include <iostream>
#include <mysql/mysql.h>
using namespace std;
//User表的增加方法
bool UserModel::insert(User &user)
{

    //1、组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into User(name, password, state) values('%s','%s','%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            //获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
        return false;
    }
}
//登录{"msgid":1,"id":1,"password":"123456"}
//登录{"msgid":1,"id":2,"password":"666666"}
//注册{"msgid":3,"name":"li si","password":"666666"}
//onechat发送聊天消息{"msgid":5,"id":1,"from":"zhang san","to":2,"msg":"hello222"}
//onechat发送聊天消息{"msgid":5,"id":2,"from":"li si","to":1,"msg":"你好"}
//添加好友{"msgid":6,"id":1,"friendid":2}
User UserModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id = %d", id);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }

    return User();
}
bool UserModel::updateState(User user)
{
    char sql[1024] = {0};
    sprintf(sql, "update User set state='%s' where id=%d", user.getState().c_str(), user.getId());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
        return false;
    }
}
   //重置用户的状态信息
void UserModel::resetstate()
{
    char sql[1024] = "update User set state='offline' where state='online'";

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}