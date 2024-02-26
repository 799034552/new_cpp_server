/*
 * @Author: string
 * @Date: 2024-02-26 10:25:31
 * @LastEditors: string
 * @LastEditTime: 2024-02-26 21:34:37
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Server.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<functional>
#include<string>
#include<memory>
#include<SubReactor.h>
#include<vector>
using std::string;
using std::function;
using std::vector;
using std::unique_ptr;
class Server{
    private:
        int port; //端口
        int listenfd; //监听描述符
        vector<unique_ptr<SubReactor>> sub_reactor_list; // subreactor列表
    public:
        Server()=default;
        void listen(int port_, std::function<void()> fn = NULL,int thread_num = 4);
        void get(const string &url, function<void()>);
        void run();
};

void test();