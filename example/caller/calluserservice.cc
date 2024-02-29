#include <iostream>
#include "user.pb.h"
#include "MprpcApplication.h"
#include "MprpcChannel.h"
#include "MprpcController.h"

int main(int argc, char **argv)
{
    //框架初始化
    mprpc::MprpcApplication::init(argc, argv);

    //演示调用远程调用的rpc方法Login()
    fixbug::UserServiceRpc_Stub stub(new mprpc::MprpcChannel());

    fixbug::LoginRequest request;//rpc请求
    fixbug::LoginResponse response;//rpc响应
    std::string name = "lisi";
    std::string pwd = "123456";
    
    request.set_name(name);
    request.set_password(pwd);
    
    //携带rpc执行状态的信息
    mprpc::MprpcController controller;
    //在RpcChannel代理类上处理, MprpcChannel::CallMethod()
    stub.Login(&controller, &request, &response, nullptr);//RpcChannel->RpcChannel::CallMethod()

    //一次rpc调用完成，读调用结果
    if(!controller.Failed())
    {
        if(0 == response.result().errcode())
        {
            //调用正确,返回结果
            std::cout<<"rpc login response success:"<<response.success()<<std::endl;
        }
        else
        {
            //调用成功，但是是其他错误
            std::cout<<"rpc login response  error code:"<<response.result().errcode()<<std::endl;
            std::cout<<"rpc login response  error msg:"<<response.result().errmsg()<<std::endl;
        }
    }
    else
    {
        std::cout<<controller.ErrorText()<<std::endl;
    }
    
    return 0;
}