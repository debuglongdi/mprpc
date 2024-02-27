#include "test.pb.h"
#include <iostream>
#include <string>

using namespace fixbuf;

int main()
{
    // LoginResponse rsp;
    // ResultCode *rc =  rsp.mutable_result();
    // rc->set_errcode(1);
    // rsp.set_success(false);

    //
    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);
    rc->set_errmsg("succeed");
    
    User *user1 = rsp.add_friend_lists();
    user1->set_age(20);
    user1->set_name("zhang san");
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_lists();
    user2->set_age(20);
    user2->set_name("zhang san");
    user2->set_sex(User::MAN);
    std::cout<< rsp.friend_lists_size()<< std::endl;

    //获取列表中的数据
    
}

int main1()
{
    //封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    //序列化：对象->字节流
    std::string send_str;
    if(req.SerializePartialToString(&send_str))
    {
        std::cout << send_str << std::endl;
    }

    //从send_str 反序列化一个login请求对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str));
    {
        std::cout<<reqB.name()<<std::endl;
        std::cout<<reqB.pwd()<<std::endl;
    }

    return 0;
}   