/*moudu网络库给用户提供了两个主要的类
 TCP server:用户编写服务器程序的
 TCP Client :用于编写客户端程序
 epoll+线程池
 好处：能够将网络I/O的代码和业务代码区分开
                    只需要关注：用户的连接和断开    用户的可读写事件
 */
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional> //绑定器
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
/*基于muduo网络库开发服务器程序
1.组合TCPserver对象
2.创建EVEVTLOOP事件循环对象的指针
3.明确TcpServer对象构造函数需要什么参数，输出CHATSERVER的构造函数
4在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5、设置合适的服务端线程数量，muduo库会自己划分I/O线程和worker线程
*/
class Chatserver
{
public:
    Chatserver(EventLoop *loop,               //事件循环 reactor
               const InetAddress &listenAddr, //IP+端口
               const string &nameArg)         //服务器的名字
        : _server(loop, listenAddr, nameArg),_loop(loop)
        
    {
        //给服务器注册用户连接的创建和断开回调
        //当发现用户的的创建和断开时调用onConnection函数
        _server.setConnectionCallback(std::bind(&Chatserver::onConnection, this, _1));

        //给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&Chatserver::onMessage, this, _1, _2, _3)); //_1参数占位符
        //设置服务器端的线程数量1个I/O线程3个工作线程
        _server.setThreadNum(4);
    }
    //开启时间循环
    void start()
    {
        _server.start();
    }

private:
    //  专门处理用户的连接创建和断开
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:online" <<endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:offline" <<endl;
            conn->shutdown();//clode(fd)把socket释放掉
            //_loop->quit();
        }
    }

    //专门处理用户的读写事件
    void onMessage( const TcpConnectionPtr &coon, //连接
                   Buffer *buffer,                        //缓冲区
                   Timestamp time)
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        coon->send(buf);
    }                  //接收到数据的时间信息
    TcpServer _server; //#1对象
    EventLoop *_loop;  //#2 epoll事件循环
};
 int main()
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    Chatserver server(&loop, addr, "ChatServer");
    server.start(); //listen epoll_ctl=>epoll
    loop.loop();    //最后epoll_wait以阻塞方式等待新用户的连接，已连接用户的读写事件等
    return 0;
}
//g++ -std=c++11 -o server muduo_server.cpp -lmuduo_net -lmuduo_base -lpthread