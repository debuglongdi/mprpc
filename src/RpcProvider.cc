#include "RpcProvider.h"
#include "MprpcApplication.h"
using namespace mprpc;

void RpcProvider::notifyService(google::protobuf::Service *service)
{

}

/// @brief 启动rpc服务节点，开始提供rpc远程网络调用服务
void RpcProvider::run()
{
    //设置服务端ip,地址
    std::string ip = MprpcApplication::getInstance().getConfig().Load("rpcserverip");
    uint16_t port = std::stoi(MprpcApplication::getInstance().getConfig().Load("rpcserverport"));
    muduo::net::InetAddress address(ip,port);

    //创建TcpServer对象
    muduo::net::TcpServer server(&eventLoop_, address, "RpcProvider");

    //绑定连接回调和消息读写回调,分离网络代码和业务代码
    server.setConnectionCallback(
        std::bind(&RpcProvider::onConnection,this, std::placeholders::_1)
    );
    server.setMessageCallback(
        std::bind(&RpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );

    //设置muudo库的线程数量，work工作线程
    server.setThreadNum(3);

    std::cout<<"RpcProvider start serveic at ip:"<<ip<<" port:"<<port<<std::endl;
    server.start();
    //mainReactor 接受连接，阻塞在这里
    eventLoop_.loop();
}


//回调
void RpcProvider::onConnection(const muduo::net::TcpConnectionPtr&)
{

}
void RpcProvider::onMessage(const  muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp receiveTime)
{

}
