/*
 * @Author: string
 * @Date: 2024-02-26 11:09:11
 * @LastEditors: string
 * @LastEditTime: 2024-02-26 19:56:43
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
using std::vector;
using std::string;
// 设置描述符为非阻塞的
void set_fd_noblock(int &fd);
// 使用串进行分割字符
vector<string> split(string &s, const char *sp);
vector<string> split_const(const string &s, std::initializer_list<const char> sp);
vector<string> split_const(const string &s, const char sp);