/*
 * @Author: string
 * @Date: 2024-02-26 15:42:07
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 14:52:28
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/SubReactor.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<memory>
#include<unordered_set>
#include<sys/eventfd.h>
#include<vector>
#include<unordered_map>
#include<sys/epoll.h>
#include<cpp_server/conf.h>

#include<cpp_server/Client/ReflexClient.h>
#include<cpp_server/Client/HttpClient.h>
using std::unordered_set;
using std::unordered_map;
using std::vector;

// class Client;
// using SP_Client = std::shared_ptr<Client>;

class SubReactor{
    public:
        // 读数据时候的缓存池
        char BUF[BUF_SIZE];
        // 内核事件表描述符
        int epollfd;
        // 主线程通知描述符
        int notify_fd;
        // 工作列表
        vector<int> job_list;
        // 与工作列表相匹配的锁
        pthread_mutex_t mutex;
        void add_job(int fd);
        void get_job();
        void add_client(int fd);
        void delete_clients();
        epoll_event epoll_events[EPOLL_SIZE];

        //文件传输部分
        int end_file_wake_up_fd;

        SubReactor();
        void run();
        
        ~SubReactor(){
            pthread_mutex_destroy(&mutex);
        }

    private:
        // 每个subReactor的客户池
        unordered_set<SP_Client> client_pool;
        unordered_map<int, SP_Client> fd2Client;
        // 等待删除的客户池
        unordered_set<SP_Client> delete_client_pool;
};
using SP_SubReactor = std::shared_ptr<SubReactor>;