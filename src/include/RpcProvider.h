#pragma once
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>
#include <unordered_map>



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
    //service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; //服务对象指针
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; //服务对象包含的所有方法描述
    };

    //存储注册到rpc节点上的服务对象和其所有的服务方法的描述
    std::unordered_map<std::string, ServiceInfo> serviceMap_;

    //设置回调
    void onConnection(const muduo::net::TcpConnectionPtr&);
    void onMessage(const  muduo::net::TcpConnectionPtr& conn,
                             muduo::net::Buffer* buffer,
                             muduo::Timestamp receiveTime);
    //组合EventLoop
    muduo::net::EventLoop eventLoop_;

    //Closure 回调操作，用于序列化rpc的响应和网络发送

    void sendRpcResponse(const  muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response);

};
} // namespace mprpc
