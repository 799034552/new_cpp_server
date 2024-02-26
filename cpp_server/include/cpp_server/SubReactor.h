/*
 * @Author: string
 * @Date: 2024-02-26 15:42:07
 * @LastEditors: string
 * @LastEditTime: 2024-02-26 21:13:13
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/SubReactor.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<memory>
#include<unordered_set>
#include<sys/eventfd.h>
#include<cpp_server/Client/Client.h>
using std::unordered_set;
#define BUF_SIZE 1024
class SubReactor{
    public:
        // 读数据时候的缓存池
        char buf[BUF_SIZE];
        // 内核事件表描述符
        int epoll_fd;
        
        SubReactor(int _notify_fd)
            :epoll_fd(epoll_create1(0)),
            notify_fd(eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC))
        {
            epoll_fd = epoll_create1(0);
        }
        void add_delete_client(SP_Client c){ delete_client_pool.insert(c);}
        void run();

    private:
        // 主线程通知描述符
        int notify_fd;
        // 每个subReactor的客户池
        unordered_set<SP_Client> client_pool;
        // 等待删除的客户池
        unordered_set<SP_Client> delete_client_pool;
};
using SP_SubReactor = std::shared_ptr<SubReactor>;