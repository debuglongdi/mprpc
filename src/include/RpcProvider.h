#pragma once
#include <google/protobuf/service.h>
#include <functional>

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
    
};
} // namespace mprpc
