/*
 * @Author: string
 * @Date: 2024-02-27 09:49:27
 * @LastEditors: string
 * @LastEditTime: 2024-02-27 23:14:14
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Client/ReflexClient.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<cpp_server/Client/Client.h>
#include<memory>
#include <functional>
class ReflexClient : public Client{
    public:      
        using Client::Client;
        void read_fn(){
            this->send_buf += this->rec_buf;
            this->rec_buf.clear();
            this->change_to_write();
        }
        void write_fn(){
            this->change_to_read();
        }
};
using SP_ReflexClient = std::shared_ptr<ReflexClient>;