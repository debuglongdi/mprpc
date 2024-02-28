#include "config.h"
#include <iostream>

using namespace mprpc;

//负责加载配置文件
void Config::loadConfigFile(const char *conifg_file)
{
    FILE *pf = fopen(conifg_file, "r");
    if(pf == nullptr)
    {
        std::cout<<"can't open file\n";
        exit(EXIT_FAILURE);
    }

    //注释、正确配置项、去掉多余空格
    while(!feof(pf))
    {
        char line[1024] = {0};
        if(fgets(line, 1024, pf) == nullptr)
            continue;
        std::string str = line;
        int idx = str.find('=',0);
        if(idx ==-1) //无效配置项
        {
            continue;
        }
        int endix = str.find('\n',idx);
        std::string key = str.substr(0,idx);
        std::string value = str.substr(idx+1,endix-idx-1);
        //插入map
        configMap_.insert({key,value});
    }
}
//查询配置项信息
std::string  Config::Load(std::string key)
{
    if(configMap_.find(key) == configMap_.end())
    {
        return "";
    }
    else
    {
        return configMap_[key];
    }
}