#include "zookeeperutils.h"
#include "MprpcApplication.h"
#include <string>
#include <iostream>

namespace mprpc
{
// 全局的watcher观察器   zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type,
                   int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  // 回调的消息类型是和会话相关的消息类型
	{
		if (state == ZOO_CONNECTED_STATE)  // zkclient和zkserver连接成功
		{
			sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
		}
	}
}

}// namespace mprpc


using namespace mprpc;

ZkClient::ZkClient()
                :zhandle_(nullptr)
{

}
ZkClient::~ZkClient()
{
    if (zhandle_ != nullptr)
    {
        zookeeper_close(zhandle_); // 关闭句柄，释放资源  MySQL_Conn
    }
}
void ZkClient::start()
{
    std::string host = MprpcApplication::getInstance().getConfig().Load("zookeeperip");
    std::string port = MprpcApplication::getInstance().getConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    /*
	zookeeper_mt：多线程版本
	zookeeper的API客户端程序提供了三个线程
	API调用线程：当前线程
	网络I/O线程:开一个线程做 pthread_create  poll
	watcher回调线程：global_watcher pthread_create
	*/
    zhandle_ = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == zhandle_) 
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(zhandle_, &sem);

    //等待global_watcher  sem_post(sem);
    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}
void ZkClient::create(const char *path, const char *data, int len, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
	// 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
	flag = zoo_exists(zhandle_, path, 0, nullptr);
	if (ZNONODE == flag) // 表示path的znode节点不存在
	{
		// 创建指定path的znode节点了
		flag = zoo_create(zhandle_, path, data, len,
			&ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
		if (flag == ZOK)
		{
			std::cout << "znode create success... path:" << path << std::endl;
		}
		else
		{
			std::cout << "flag:" << flag << std::endl;
			std::cout << "znode create error... path:" << path << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}
std::string ZkClient::getData(const char *path) const
{
    char buffer[64];
	int bufferlen = sizeof(buffer);
	int flag = zoo_get(zhandle_, path, 0, buffer, &bufferlen, nullptr);
	if (flag != ZOK)
	{
		std::cout << "get znode error... path:" << path << std::endl;
		return "";
	}
	else
	{
		return buffer;
	}
}