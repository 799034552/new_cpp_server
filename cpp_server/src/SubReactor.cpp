/*
 * @Author: string
 * @Date: 2024-02-26 15:43:03
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 19:57:29
 * @FilePath: /new_cpp_server/cpp_server/src/SubReactor.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include<cpp_server/SubReactor.h>
#include<cpp_server/Client/WSClient.h>
#include<cpp_server/util.h>

SubReactor::SubReactor()
    :epollfd(epoll_create1(0)),
    notify_fd(eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC))
{
    epoll_add_read_LT(epollfd, notify_fd);
    epoll_add_read_LT(epollfd, end_file_wake_up_fd);
    pthread_mutex_init(&mutex, NULL);
}

// 工作队列增加新用户
void SubReactor::add_job(int fd){
    pthread_mutex_lock(&mutex);
    job_list.emplace_back(fd);
    pthread_mutex_unlock(&mutex);
    // 通知subreactor
    uint64_t tmp = 1;
    write(notify_fd, &tmp, sizeof(tmp));
}

// 从工作队列获取新用户
void SubReactor::get_job(){
    vector<int> t_job_list;
    pthread_mutex_lock(&mutex);
    t_job_list.swap(job_list);
    pthread_mutex_unlock(&mutex);
    for(int &fd: t_job_list){
        add_client(fd);
    }
}

// 增加用户
void SubReactor::add_client(int fd){
    // 默认用户是ReflexClient的
    SP_Client new_client = SP_HttpClient(new HttpClient(fd, epollfd, BUF));
    new_client->end_file_wake_up_fd = end_file_wake_up_fd;

    // 添加删除事件
    new_client->set_close_fn([this, new_client]{
        this->delete_client_pool.insert(new_client);
    });
    client_pool.insert(new_client);
    fd2Client[fd] = new_client;
}

// 删除用户
void SubReactor::delete_clients(){
    if (!delete_client_pool.empty()){
        for(auto &now_client: delete_client_pool){
            client_pool.erase(now_client);
            fd2Client.erase(now_client->fd);
            now_client->set_close_fn(nullptr);
            // printf("用户%d删除\n", now_client->fd);
            if (!now_client->need_close_fd){
                
                // 不用关闭fd，肯定是升级为websocket协议,添加websocket用户
                HttpClient* http_now_client = (HttpClient*)now_client.get();
                SP_Client new_client = SP_WSClient(new WSClient(now_client->fd, epollfd, BUF, false, http_now_client->header["Sec-WebSocket-Key"], http_now_client->header["url"]));
                new_client->end_file_wake_up_fd = end_file_wake_up_fd;
                // 添加删除事件
                new_client->set_close_fn([this, new_client]{
                    this->delete_client_pool.insert(new_client);
                });
                client_pool.insert(new_client);
                fd2Client[now_client->fd] = new_client;
            }
        }
        delete_client_pool.clear();
    }
}

// 每个线程的主循环
void SubReactor::run(){
    while(1){
        int n = epoll_wait(epollfd, epoll_events, EPOLL_SIZE, -1);
        for(int i = 0; i < n; ++i){
            int fd = epoll_events[i].data.fd;
            // 如果是主线程通知
            if (fd == notify_fd){
                read(notify_fd, BUF, BUF_SIZE);
                get_job();
            }
            // 如果是文件传输结束通知
            else if (fd == end_file_wake_up_fd){
                uint64_t client_fd;
                read(end_file_wake_up_fd, &client_fd, sizeof(client_fd));
                auto &now_client = fd2Client[client_fd];
                now_client->file_name.clear();
                // 写入完毕回调
                now_client->write_fn();
            }
            else{
            // 是客户通知
                auto &now_client = fd2Client[epoll_events[i].data.fd];
                now_client->revent = epoll_events[i].events;
                now_client->handle_event();
            }
        }
        delete_clients();
    }
}