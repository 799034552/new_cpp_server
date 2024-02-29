/*
 * @Author: string
 * @Date: 2024-02-26 10:34:48
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 21:46:49
 * @FilePath: /new_cpp_server/cpp_server/src/Client/Client.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <cpp_server/Client/Client.h>
#include <cpp_server/AsyncHandle.h>
// 构造函数
Client::Client(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll)
    :fd(fd_),epollfd(_epollfd),BUF(_BUF) { 
    // 设置为未阻塞
    set_fd_noblock(fd); 
    if (is_need_epoll){
        // 将监听事件加入到内核中
        epoll_event t;
        t.data.fd = fd;
        t.events = EPOLLIN | EPOLLET;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &t);
    }
};
// 析构函数
Client::~Client() {
    if (fd > 0){
        if (need_close_fd){
            // printf("真正地删除事件");
            // 将监听事件从内核删除
            epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
            close(fd);
        }
    }
};



// 根据revent调研相应的回调函数
void Client::handle_event()
{
    // 关闭连接就不处理任何东西了
    if (is_close)
        return;
    // 出错了
    if (revent & EPOLLERR)
    {
        perror("poll error\n");
        is_close = true;
    }
    // 可读
    else if (revent & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {

        // 每次写入都执行写入回调
        read_all();
        // printf("用户可读，消息为:%s\n", rec_buf.c_str());
        read_fn();
    }
    // 可写
    else if (revent & EPOLLOUT)
    {
        // printf("用户可写:%s\n", send_buf.c_str());
        // 发送后完成回调
        if(write_all()){
            // printf("用户写完成\n");
            write_fn();
            // 写完后如果读的也关闭了那么就关闭
            if (is_read_close)
                is_close = true;
        }
    }
    // 关闭连接了
    if (is_close){
        // printf("要关闭连接\n");
        if (close_fn) close_fn();
    }
}

// 更新内核监听的事件
void Client::update_event(EventType ev)
{
    epoll_event t;
    t.data.fd = fd;
    t.events = ev;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &t);
}

// 读取数据到read_buf中
int Client::read_all()
{
    int n, sum = 0;
    while ((n = read(fd, BUF, BUF_SIZE - 1)) > 0)
    {
        BUF[n] = '\0';
        rec_buf += BUF;
        sum += n;
    }
    if (n == 0){
        is_read_close = true;
        return sum;
    }
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)){
        // printf("读完这次了\n");
        return sum;
    }
        
    else
        return -1;
}

// 从write_buf中写入数据到socket
bool Client::write_all(){
    int n = write(fd, send_buf.data(), send_buf.size());
    // 全部发送成功
    if (n == send_buf.size()){
        send_buf.clear();
        return true;
    } else if (n > 0){
    // 部分发送成功过
        send_buf = send_buf.substr(n);
    } else if (n < 0){
    // 写入失败关闭连接
        is_close = true;
    }
    return false;
}

void Client::change_to_read(){
    update_event(EPOLLIN | EPOLLET);
}
void Client::change_to_write(){
    update_event(EPOLLOUT | EPOLLET);
}