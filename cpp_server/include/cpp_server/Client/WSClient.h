/*
 * @Author: string
 * @Date: 2024-02-29 16:49:10
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 17:48:14
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Client/WSClient.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include<cpp_server/Client/HttpClient.h>

class WSClient: public Client{
    public:
        static const string GUID;
        WSClient(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll, string key);
        void read_fn();
        void write_fn();
};
using SP_WSClient = std::shared_ptr<WSClient>;