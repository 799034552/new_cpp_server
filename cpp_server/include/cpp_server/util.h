/*
 * @Author: string
 * @Date: 2024-02-26 11:09:11
 * @LastEditors: string
 * @LastEditTime: 2024-02-28 15:55:01
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/util.h
 * @Description: 工具函数库
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<fcntl.h>
#include<vector>
#include<string>
#include<algorithm>
#include<sys/epoll.h>
using std::vector;
using std::string;
// 设置描述符为非阻塞的
void set_fd_noblock(int &fd);

// 向epoll事件表中加入可读事件
void epoll_add_read_LT(int epollfd, int fd);


// 使用串进行分割字符
vector<string> split_const(const string &s, std::initializer_list<const char> sp);
void trim(string &s);