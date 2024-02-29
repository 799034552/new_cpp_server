/*
 * @Author: string
 * @Date: 2024-02-26 10:25:26
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 16:37:15
 * @FilePath: /new_cpp_server/cpp_server/src/Server.cpp
 * @Description: 主要的客户端
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include <cpp_server/Server.h>
#include <stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<cpp_server/util.h>
#include<signal.h>
#include<cpp_server/AsyncHandle.h>

int sigfd = eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC); //信号的事件描述符

void sig_handle(int _sig){
    uint64_t sig = _sig;
    write(sigfd, &sig, sizeof(sig));
}
//构造函数
Server::Server()
    :epollfd(epoll_create1(0))
{
    // 统一信号源
    epoll_add_read_LT(epollfd, sigfd);
    signal(SIGINT,sig_handle);
}

void Server::listen(int port_, std::function<void()> fn,int thread_num){
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        perror("create listen socket fail"),exit(0);
    sockaddr_in addr;
    inet_pton(AF_INET, "0.0.0.0",&addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    if ( bind(listenfd, (sockaddr*)&addr, sizeof(addr)) < 0)
        perror("create listen socket fail"),exit(0);
    
    // 跳过wait_time直接关闭，仅调试时开启
    // #ifdef DEBUG
    int temp = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));
    // #endif
    ::listen(listenfd, 10240);

    // 加入监听事件
    epoll_add_read_LT(epollfd, listenfd);
    
    // 创建subreactor
    while (thread_num--)
        sub_reactor_list.emplace_back(new SubReactor());
    if (fn) fn();
}

// 每一个sub_reactor循环
void* start_thread(void *_sub_reactor){
    SubReactor* sub_reactor = (SubReactor*)_sub_reactor;
    sub_reactor->run();
    return nullptr;
}

// 主循环
void Server::run(){
    setbuf(stdout, NULL);
    // 创建多线程
    for(SubReactor* sub_reactor: sub_reactor_list){
        pthread_t p;
        pthread_create(&p, NULL, start_thread, sub_reactor);
    }
    async_task_io = shared_ptr<AsyncHandleThread>(new AsyncHandleThread());

    // 主循环
    while(1){
        int n = epoll_wait(epollfd, epoll_events, EPOLL_SIZE, -1);
        for(int i = 0; i < n; ++i){
            int fd = epoll_events[i].data.fd;
            // 新用户到来
            if (fd == listenfd){
                int connfd = accept(listenfd, NULL, NULL);
                // 挑选一个sub_reactor加入这个用户;
                sub_reactor_list[now_sub_reactor_i]->add_job(connfd);
                // 选出负责连接下一个reactor的
                now_sub_reactor_i = (now_sub_reactor_i + 1) % sub_reactor_list.size();
            }
            // 信号到来
            else if (fd == sigfd){
                uint64_t sig;
                read(sigfd, &sig, sizeof(sig));
                switch (sig)
                {
                case SIGINT:
                    exit(0);
                    break;
                }
            }
        }
    }

}

Server::~Server(){
    for(SubReactor* sub_reactor: sub_reactor_list)
        delete sub_reactor;
}

void Server::get(const string& url, const std::function<void(Req&, Res&)> &fn)
{
    HttpClient::get_progress[url].emplace_back(fn);
}
void Server::post(const string& url, const std::function<void(Req&, Res&)> &fn)
{
    HttpClient::post_progress[url].emplace_back(fn);
}

void Server::static_file(string url, string local_url){
    HttpClient::static_map[url] = local_url;
}


