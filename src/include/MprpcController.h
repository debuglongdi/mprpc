#pragma once
#include <string>
#include <google/protobuf/service.h>

namespace mprpc
{
class MprpcController : public ::google::protobuf::RpcController
{
public:
    MprpcController();

    void Reset() override;
    bool Failed() const override;
    std::string ErrorText() const override;
    void StartCancel() override;
    void SetFailed(const std::string& reason) override;
    bool IsCanceled() const override;
    void NotifyOnCancel(::google::protobuf::Closure* callback) override;
private:
    bool failed_;
    std::string errorText_;
};
} // namespace mprpc
