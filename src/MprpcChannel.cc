
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include <cstring>   // for memset() and sockaddr_in::sin_zero
#include <errno.h>
#include <string>

#include "MprpcChannel.h"
#include "rpcheader.pb.h"
#include "MprpcApplication.h"


using namespace mprpc;

/**
 * header_size + service_name  method_name args_size + agrs
*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                        google::protobuf::Message* response, google::protobuf::Closure* done)
{
    //获取方法所属的服务
    const ::google::protobuf::ServiceDescriptor *sd =  method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //获取参数的序列化字符串长度
    uint32_t args_size = 0;
    std::string args_str;
    if(request->SerializePartialToString(&args_str))
    {
        //序列化成功
        args_size = args_str.size();

    }
    else
    {
        std::cout<<"args Serialize error!\n";
        return ;
    }

    //定义rpc的请求header
    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name(service_name);
    rpc_header.set_method_name(method_name);
    rpc_header.set_args_size(args_size);
    
    std::string header_str;
    uint32_t header_size = 0;
    if(rpc_header.SerializePartialToString(&header_str))
    {
        //序列化成功
        header_size = header_str.size();
    }
    else
    {
        //序列化失败
        std::cout<<"header_str Serialize error!\n";
        return ;
    }

    //组织待发送的rpc请求字符串
    std::string send_str;
    send_str.insert(0,std::string((char*)&header_size,4));//在每个请求前面放请求头的长度=service_name + method_name + args_size
    send_str += header_str;
    send_str += args_str;

   

        // 打印调试信息
    std::cout << "====================================================" << std::endl;
    std::cout<<"send_str:"<<send_str<<std::endl;
    std::cout << "header_size=" << header_size << std::endl;
    std::cout << "header_str:" << header_str << std::endl;
    std::cout << "service_name:" << service_name << std::endl;
    std::cout << "method_name:" << method_name << std::endl;
    std::cout << "args_size=" << args_size << std::endl;
    std::cout << "args_str:" << args_str << std::endl;
    std::cout << "====================================================" << std::endl;

    //通过网络发送，使用Tcp编程，完成rpc方法的远程调用

    //1 socket
    int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
    std::shared_ptr<int> p(&clientfd,[](const int * ptr){
        std::cout<<"client is closeed\n";
        ::close(*ptr);
    });
    if(clientfd == -1)
    {
        std::cout<<"socket failed to create socket errno="<<errno<<std::endl;
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_address;
    //2 设置服务端地址、端口等信息
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htobe16(stoi(MprpcApplication::getInstance().getConfig().Load("rpcserverport")));
    ::inet_pton(AF_INET, MprpcApplication::getInstance().getConfig().Load("rpcserverip").c_str(), &server_address.sin_addr);
    socklen_t addrlen= sizeof(server_address);


    std::cout<<"rpcip="<<server_address.sin_addr.s_addr<<std::endl;
    std::cout<<"rpcport="<<server_address.sin_port<<std::endl;


    //3、建立连接,其实可以多尝试几次
    if(::connect(clientfd, (const struct sockaddr *)&server_address, addrlen) != 0)
    {
        std::cout<<"connet to rpc server errno:"<<errno<<std::endl;
        exit(EXIT_FAILURE);
    }

    
    //4、开始发送

    if(-1 == send(clientfd, send_str.c_str(), send_str.size(),0))
    {
        std::cout<<"send err, errno:"<<errno<<std::endl;
        return;
    }

    //5、接受rpc请求的响应值
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        std::cout<<"recv err, errno:"<<errno<<std::endl;
        return;
    }
    
    //6、填充返回值到response,反序列化
    // std::string response_str(recv_buf, 0, recv_size);

    if(!response->ParseFromArray(recv_buf, recv_size))
    {
        std::cout<<"recv_buf Parse err, "<<std::endl;
        return;
    }
}