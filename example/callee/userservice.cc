#include <iostream>
#include <string>
#include "user.pb.h"
#include "MprpcApplication.h"
#include "RpcProvider.h"
/**
 * UserService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/

//库的namespace
using namespace mprpc;
class UserService : public fixbug::UserServiceRpc //rpc服务提供者
{
public:
    bool Login(std::string name, std::string password)
    {
        std::cout<<"doing local service: Login"<<std::endl;
        std::cout<<"name="<<name<<" password="<<password<<std::endl;
        return true;
    }

    /**
     * 重写基类UserServiceRpc的基类的虚函数，下面的方法是被框架调用的
     * 1.caller ====> Login(LoginRequest) =>muduo=> callee
     * 2.callee ====> Login(LoginRequest) =>交到下面rpc重写的方法
     */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done) override
    {
        //1.框架给业务上报了请求参数，业务获取相应数据，作本地业务
        std::string name = request->name();
        std::string password = request->password();
        //2.执行本地业务
        bool ret = Login(name, password);
        
        //2.把响应写入response，包括：错误码，错误消息，返回值，
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(ret);
        
        //4.执行回调,将响应数据序列化，使用网络发送，由框架完成
        done->Run();

    }

private:

};

int main(int argc, char **argv)
{
    // 1、框架的初始化操作
    MprpcApplication::init(argc, argv);
    
    //2、使用框架发布服务:将UserService发布rpc节点上
    RpcProvider provider;
    provider.notifyService(new UserService);
    // provider.notifyService(new OtherService);

    //3.启动一个rpc发布节点;run后，程序阻塞等待远程rpc调用请求
    provider.run();

}