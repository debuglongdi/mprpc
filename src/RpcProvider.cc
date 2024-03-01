#include "RpcProvider.h"
#include "MprpcApplication.h"
#include "rpcheader.pb.h"
#include "zookeeperutils.h"

using namespace mprpc;

/// @brief 记录一张表，每一个rpc服务对象，发布了哪些方法
/// @param service
void RpcProvider::notifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;

    // 获取服务对象的：对象描述信息
    const google::protobuf::ServiceDescriptor *serviceDescPtr = service->GetDescriptor();
    // 获取服务对象名字
    std::string serviceName = serviceDescPtr->name();
    // 获取务对象的对象方法数量
    int methodCount = serviceDescPtr->method_count();

    std::cout << "service name:" << serviceName << std::endl;

    // 获取服务对象的所有的：方法描述
    service_info.m_service = service;
    for (int i = 0; i < methodCount; i++)
    {
        const google::protobuf::MethodDescriptor *methodDescPtr = serviceDescPtr->method(i);
        std::string method_name = methodDescPtr->name();
        service_info.m_methodMap.insert({method_name, methodDescPtr});
        std::cout << "method name:" << method_name << std::endl;
    }

    // 放入成员变量serviceMap_中
    serviceMap_.insert({serviceName, service_info});
}

/// @brief 启动rpc服务节点，开始提供rpc远程网络调用服务
void RpcProvider::run()
{
    // 设置服务端ip,地址
    std::string ip = MprpcApplication::getInstance().getConfig().Load("rpcserverip");
    uint16_t port = std::stoi(MprpcApplication::getInstance().getConfig().Load("rpcserverport"));
    muduo::net::InetAddress address(ip, port);

    // 创建TcpServer对象
    muduo::net::TcpServer server(&eventLoop_, address, "RpcProvider");

    // 绑定连接回调和消息读写回调,分离网络代码和业务代码
    server.setConnectionCallback(
        std::bind(&RpcProvider::onConnection, this, std::placeholders::_1));
    server.setMessageCallback(
        std::bind(&RpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置muudo库的线程数量，work工作线程
    server.setThreadNum(3);

    //把当前rpc节点上的要发布的服务的地址注册到zk上面，让rpc client 可以从zk上发现服务
    ZkClient zkCli;
    zkCli.start();

    //service_name 为永久性节点 method_name为临时节点，有多个
    for(auto &sp : serviceMap_)
    {
        std::string service_path = "/"+sp.first;
        zkCli.create(service_path.c_str(), nullptr, 0);
        for(auto &mp : sp.second.m_methodMap)
        {
            //当前节点存储该方法所在服务 "ip:port"
            std::string method_path = service_path + "/" + mp.first;
            char method_path_data[128] = {0};
            sprintf(method_path_data,"%s:%d",ip.c_str(),port);
            zkCli.create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);

        }
    }// END ZkCli




    std::cout << "RpcProvider start serveic at ip:" << ip << " port:" << port << std::endl;
    //启动网络服务
    server.start();
    // mainReactor 接受连接，阻塞在这里
    eventLoop_.loop();
}

// 新的连接回调
void RpcProvider::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        // 和rpc client 断开
        conn->shutdown();
    }
}

/**
 * 在框架内部，RpcProvider 和RpcConsumer协商好通信用的protobuf数据
 * service_name method_name args
 * 所以框架也用proto定义message，进行数据的序列化和反序列化
 *      service_name method_name args_size
 * 并且要考虑应用程粘包问题
 * header(4个字节) + header_str + args_str
 */
void RpcProvider::onMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp receiveTime)
{
    std::string recv_buf = buffer->retrieveAllAsString();

    // 从字符中读取前4个字节的数据
    uint32_t header_size = 0;
    // copy
    recv_buf.copy((char *)&header_size, 4, 0);

    // 根据header_size读取数据头的原始字符流,反序列化数据
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size = 0;
    if (rpcHeader.ParseFromString(rpc_header_str))
    {
        // 数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else
    {
        // 数据头反序列化失败
        std::cout << "rpc_header_str:" << rpc_header_str << "parse error\n";
        conn->shutdown();
        return;
    }
    // 打印调试信息
    std::string args_str = recv_buf.substr(4 + header_size, args_size);
    std::cout << "====================================================" << std::endl;
    std::cout << "header_size=" << header_size << std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str << std::endl;
    std::cout << "service_name:" << service_name << std::endl;
    std::cout << "method_name:" << method_name << std::endl;
    std::cout << "args_size=" << args_size << std::endl;
    std::cout << "args_str:" << args_str << std::endl;
    std::cout << "====================================================" << std::endl;

    /**
     * 框架调用rpc服务对象的服务方法
     */

    // 获取service对象
    auto it = serviceMap_.find(service_name);
    if (it == serviceMap_.end())
    {
        std::cout << "service" << service_name << " didn't exist\n";
        conn->shutdown();
        return;
    }
    // 获取方法
    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end())
    {
        std::cout << " service:" << service_name << " method " << method_name << "didn't exist\n";
        conn->shutdown();
        return;
    }

    google::protobuf::Service *service = it->second.m_service;
    const google::protobuf::MethodDescriptor *method = mit->second;

    //生成rpc请求的request 和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        std::cout<<"request parse error:"<<args_str<<std::endl;
        conn->shutdown();
        return;
    }

    // 给下面的CallMethod（）绑定一个Closure的回调函数
    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider,
                                                                const  muduo::net::TcpConnectionPtr&, 
                                                                google::protobuf::Message*>
                                                                (this, 
                                                                &RpcProvider::sendRpcResponse,
                                                                conn,
                                                                response);
    
    //在框架上根据远端rpc请求，调用当前节点的上发布的方法
    service->CallMethod(method, nullptr, request, response, done);
}

void RpcProvider::sendRpcResponse(const  muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response)
{
    std::string response_str;
    if(response->SerializePartialToString(&response_str))
    {
        //序列化成功后，通过网络将rpc方法执行的结果发送回rpc调用方
        // mprpc::RpcHeader rpcHeader;
        conn->send(response_str);
    }
    else
    {
        std::cout<< "Serialize response_str err!"<<response_str<<std::endl;
    }
    //模拟http短连接，服务方主动断开
    conn->shutdown();
}