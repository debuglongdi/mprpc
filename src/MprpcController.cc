#include "MprpcController.h"

using namespace mprpc;


MprpcController::MprpcController()
                :failed_(false),
                errorText_()
{ }

void MprpcController::Reset()
{
    failed_ = false;
    errorText_ = "";
}
inline bool MprpcController::Failed() const
{
    return failed_;
}
void MprpcController::SetFailed(const std::string& reason)
{
    failed_ = true;
    errorText_ = reason;
}
std::string MprpcController::ErrorText() const
{
    return errorText_;
}

//未实现
void MprpcController::StartCancel()
{

}
bool MprpcController::IsCanceled() const
{
    return false;
}
void MprpcController::NotifyOnCancel(::google::protobuf::Closure* callback)
{

}