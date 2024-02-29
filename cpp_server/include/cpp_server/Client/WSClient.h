/*
 * @Author: string
 * @Date: 2024-02-29 16:49:10
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 20:17:17
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Client/WSClient.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include<cpp_server/Client/HttpClient.h>

struct frame_head {
    char fin;
    char opcode;
    char mask;
    unsigned long long payload_length;
    char masking_key[4];
};

class WSClient: public Client{
    public:
        string url;
        static const string GUID;
        WSClient(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll, string key, string _url);
        enum class PARSE_STATE {TWO_CHAR = 0, PAY_LOAD_Length, Masking_Key, PAY_LOAD, END} parse_state;
        frame_head f_head; //读取帧
        frame_head write_head; //写入帧
        bool is_first_send = true; // 是否是第一次发送完成
        void read_fn();
        void write_fn();
        function<void(WSClient*)> connect_fn;
        function<void(string &data, WSClient*)> get_fn;
        void on_connect(function<void(WSClient*)> fn){
            connect_fn = fn;
        }
        void on_get(function<void(string &data,WSClient*)> fn){
            get_fn = fn;
        }
        void send(string a);
        void change_to_read_write(){
            update_event(EPOLLIN | EPOLLOUT | EPOLLET);
        }
        static unordered_map<string, function<void(WSClient*)>> handle_fn;
        ~WSClient(){};
};
using SP_WSClient = std::shared_ptr<WSClient>;
string get_send_head(frame_head* head);