#pragma once
#include "noncopyable.h"
#include "config.h"
// 框架初始化类

namespace mprpc
{
//sigleton
class MprpcApplication : noncopyable
{
public:
    static void init(int argc, char **argv);
    static MprpcApplication& getInstance();
    static Config& getConfig()
    { return config_;}
private:
    static Config config_;
    MprpcApplication();
};

} // namespace mprpc


