#pragma once
#include <google/protobuf/service.h>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>


namespace mprpc
{
class RpcProvider
{
public:
    /// @brief 框架提供给外部程序发布rpc服务的接口
    /// @param service 
    void notifyService(google::protobuf::Service *service);

    /// @brief 启动rpc服务节点，开始提供rpc远程网络调用服务
    void run();
private:
    //设置回调
    void onConnection(const muduo::net::TcpConnectionPtr&);
    void onMessage(const  muduo::net::TcpConnectionPtr& conn,
                             muduo::net::Buffer* buffer,
                             muduo::Timestamp receiveTime);
    //组合EventLoop
    muduo::net::EventLoop eventLoop_;

};
} // namespace mprpc
