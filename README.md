# cpp-chatserver
可以工作在nginx TCP负载均衡环境中的集群聊天服务器和客户端源码 基于muduo网络库和mysql、redis等


编译方式：
cd bulid
rm -rf *
cmake ..
make
需要nginx的tcp负载均衡
