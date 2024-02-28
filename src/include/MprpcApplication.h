#pragma once
#include "noncopyable.h"
// 框架初始化类

namespace mprpc
{
//sigleton
class MprpcApplication : noncopyable
{
public:
    static void init(int argc, char **argv);
    static MprpcApplication& getInstance();
private:
    MprpcApplication();
};

} // namespace mprpc


