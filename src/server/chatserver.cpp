#include "chatserver.hpp"
#include"chatservice.hpp"
#include"json.hpp"
#include <functional>
#include<string>

using namespace std;
using namespace placeholders;
using  json=nlohmann::json;

ChatSever::ChatSever(EventLoop *loop,
                     const InetAddress &listenAddr,
                     const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    //注册链接回调
    _server.setConnectionCallback(std::bind(&ChatSever::onConnection, this, _1));

    //注册消息回调
    _server.setMessageCallback(std::bind(&ChatSever::onMsessage, this, _1, _2, _3));

    //设置线程数量
    _server.setThreadNum(4);
}
void ChatSever::start()
{
    _server.start();
}
//上报链接相关信息的回调函数
void ChatSever::onConnection(const TcpConnectionPtr &conn)
{
    //用户断开连接
    if(!conn->connected()){
      ChatService::instance()-> clientCloseException(conn);
      conn->shutdown(); 
      
    }
}

//上报读写事件相关信息的回调函数
void ChatSever::onMsessage(const TcpConnectionPtr &conn,
                            Buffer *buffer,Timestamp time)
{
      string buf=buffer->retrieveAllAsString();  
      //数据的反序列化
      json js=json::parse(buf);
      //通过js[“msgid”]获取=》业务handler coon js time
      //达到的目的：完全解耦模块的代码和业务模块的代码,根据消息id获得事件处理器，进行消息回调
    auto _msghandler= ChatService::instance()->getHandler(js["msgid"].get<int>());//将数据类型转化为整形
    //回调消息绑定好的事件处理器，来执行相对应的业务处理
    _msghandler(conn,js,time);      
}