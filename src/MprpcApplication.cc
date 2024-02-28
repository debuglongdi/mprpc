#include "MprpcApplication.h"
#include <iostream>
#include <string>
#include <unistd.h>

namespace
{
    void showHelp()
    {
        std::cout<<"format: command -i <configfile>\n";
    }
}

using namespace mprpc;


Config MprpcApplication::config_;

void MprpcApplication::init(int argc, char **argv)
{
    if(argc < 2)
    {
        showHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    //从命令行获取配置文件参数
    while((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            //获取配置文件名
            config_file = optarg;
            break;
        case '?':
            showHelp();
            exit(EXIT_FAILURE);
        case ':':
            showHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }//while 读取文件名

    //开始加载配置文件 rpcserver_ip= rpcserver_port= zookeeper_ip= zookeeper_port=
    config_.loadConfigFile(config_file.c_str());

    // std::cout<<"rpcserverip="<<config_.Load("rpcserverip")<<std::endl;
    // std::cout<<"rpcserverport="<<config_.Load("rpcserverport")<<std::endl;
    // std::cout<<"zookeeperip="<<config_.Load("zookeeperip")<<std::endl;
    // std::cout<<"zookeeperport="<<config_.Load("zookeeperport")<<std::endl;


}

MprpcApplication& MprpcApplication::getInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcApplication::MprpcApplication()
{

}