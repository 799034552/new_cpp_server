/*
 * @Author: string
 * @Date: 2024-02-26 10:34:54
 * @LastEditors: string
 * @LastEditTime: 2024-02-26 19:56:57
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Client/Client.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<functional>
#include<sys/epoll.h>
#include<stdio.h>
#include<vector>
#include<memory>
#include<unistd.h>
#include<string>
#include<cpp_server/util.h>
#include<cpp_server/SubReactor.h>
using std::string;
class Client {
    using CallBack = std::function<void()>;
    using EventType = decltype(epoll_event::events);
    using SP_Client = std::shared_ptr<Client>;
    protected:
        int fd; //管理的描述符
        string rec_buf; //发送缓存
        string send_buf; //接收缓存
        EventType event; // 监听的事件
        EventType revent; // 收到的事件
        int &epollfd; //内核epoll描述符
        int read_all(); // 读取所有内容
        bool write_all(); // 写入缓存内容
        CallBack read_fn;
        CallBack write_fn;
        CallBack close_fn;
        bool is_close = false;
        bool is_read_close = false;
        time_t live_time;
        // 更新内核监听的事件
        void update_event(EventType);
        void change_to_read();
        void change_to_write();
        SubReactor *parent;

    public:
        char *BUF;
        Client(const int &fd_, SubReactor *_parent);
        // Client(const int &fd_, SubReactor *_parent):fd(fd_),is_close(false),parent(_parent),epollfd(_parent->epoll_fd) { set_fd_noblock(fd); };
        // 设置事件
        void set_event(const EventType &e){ event = e; }
        void set_revent(const EventType &e){ revent = e;}
        EventType get_event() const { return event; }
        // 设置回调函数
        void set_read_fn(const CallBack &fn) { read_fn = fn; }
        void set_write_fn(const CallBack &fn) { read_fn = fn; }
        void set_close_fn(const CallBack &fn) { close_fn = fn; }
        virtual void handle_event() = 0;
        // 销毁时关闭描述符
        ~Client();
};
using SP_Client = std::shared_ptr<SP_Client>;


