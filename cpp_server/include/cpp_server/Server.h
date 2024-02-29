/*
 * @Author: string
 * @Date: 2024-02-26 10:25:31
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 19:56:30
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Server.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<functional>
#include<string>
#include<memory>
#include<cpp_server/SubReactor.h>
#include<vector>
#include<cpp_server/conf.h>
#include<cpp_server/Client/HttpClient.h>
#include<cpp_server/Client/WSClient.h>
using std::string;
using std::function;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
class Server{
    private:
        int port; //端口
        int listenfd; //监听描述符
        int epollfd; // 主Reactor的事件描述符
        vector<SubReactor*> sub_reactor_list; // subreactor列表
        int now_sub_reactor_i; // 下一个处理的sub_reactor的索引
        epoll_event epoll_events[EPOLL_SIZE];
    public:
        Server();
        void listen(int port_, std::function<void()> fn = NULL,int thread_num = 4);
        void get(const string& url, const std::function<void(Req&, Res&)> &fn);
        void post(const string& url, const std::function<void(Req&, Res&)> &fn);
        void ws(const string& url, const std::function<void(WSClient*)> &fn);
        void run();
        void static_file(string url, string local_url);
        ~Server();
};