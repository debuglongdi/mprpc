#pragma once
#include <unordered_map>
#include <string>

namespace mprpc
{
/// 框架读取配置文件类
class Config
{
public:
    //负责加载配置文件
    void loadConfigFile(const char *conifg_file);
    //查询配置项信息
    std::string Load(std::string key);
private:
    std::unordered_map<std::string, std::string> configMap_;
}; 
} // namespace mprpc

