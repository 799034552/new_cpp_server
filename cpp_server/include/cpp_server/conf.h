/*
 * @Author: string
 * @Date: 2024-02-27 11:14:21
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 10:29:19
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/conf.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
# pragma once
#define EPOLL_SIZE 1024 //EPOLL可一次响应事件的大小
#define BUF_SIZE 1024 // 读取数据时的缓存大小
#define MMAP_NUM 5 // 可以同时处理的静态文件的数量
