#pragma once
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

namespace mprpc
{
class MprpcChannel : public google::protobuf::RpcChannel
{
public:
    /// @brief 所有通过stub代理对象调用的rpc方法，走到这里，统一做rpc方法调用的数据序列化和网络发送
    /// @param method 
    /// @param controller 
    /// @param request 
    /// @param response 
    /// @param done 
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                        google::protobuf::Message* response, google::protobuf::Closure* done) override;
};

} // namespace mprpc
