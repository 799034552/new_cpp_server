/*
 * @Author: string
 * @Date: 2024-02-26 10:45:24
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 17:33:11
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/Client/HttpClient.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include <unordered_map>
#include <string>
#include<cpp_server/Client/Client.h>
#include<cpp_server/Client/HttpClient.h>
#include<map>
using std::unordered_map;
using std::string;
// HTTP请求的Req
class Req {
  public:
    const unordered_map<string,string> &kv_data;
    const unordered_map<string,string> &header;
    // 普通数据的text
    const string &body;
    Req(const unordered_map<string,string> &_kv_data,const unordered_map<string,string> &_header,const string &_body)
        :kv_data(_kv_data),header(_header),body(_body){}
};

class Res {
  private:
    
  public:
    // 发送缓存
    int http_status;
    string send_buf;
    std::unordered_map<string,string> header;
    void send(const string &s) { send_buf += s; }
    void add_header(const string &a, const string &b) {header[a] = b;}
    Res()=default;
};

class HttpClient: public Client {
  private:
    // HTTP解析状态
    // 正在解析URL  |  正在解析HEAD  | 正在解析BODY | 解析完成 | 解析错误 | 升级成WS
    enum class HTTP_STATE {URL, HEAD, BODY, OK, BAD, WS};
    HTTP_STATE http_state = HTTP_STATE::URL;
    void read_fn();
    void write_fn();
    unordered_map<string, string> kv_data;
    string body;
    string send_first;
    // 解析URL行
    HTTP_STATE parse_url(const string &);
    // 解析header行
    HTTP_STATE parse_head(const string &);
    // 解析BODY行
    HTTP_STATE parse_body();
    //重置http状态
    void reset();
    void res2send_buf();
    bool get_line(string &line);

  public:
    unordered_map<string, string> header;
    Req req;
    Res res;
    // get与post的处理程序
    static unordered_map<string, std::vector<std::function<void(Req&, Res&)>>> get_progress;
    static unordered_map<string, std::vector<std::function<void(Req&, Res&)>>> post_progress;
    static unordered_map<string, string> static_map;
    HttpClient(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll=true);
};
using SP_HttpClient = std::shared_ptr<HttpClient>;