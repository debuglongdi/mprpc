#pragma once
#include <zookeeper/zookeeper.h>
#include <string>
#include <semaphore.h>

namespace mprpc
{
/// @brief 封装的zookeeper客户端类
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    //启动连接zkserver
    void start();

    /// @brief 在zkserver上根据指定的path创建znode节点
    /// @param path 路径
    /// @param data 
    /// @param len 长度
    /// @param state 默认0表示创建永久节点，其他创建临时节点
    void create(const char *path, const char *data, int len, int state=0);

    /// @brief 根据参数path 获取对应节点的值
    /// @param path 
    /// @return 
    std::string getData(const char *path) const;
private:
    //zk客户端句柄,表示了
    zhandle_t *zhandle_;
};
} // namespace mprpc
