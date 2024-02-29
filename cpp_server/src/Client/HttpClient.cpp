/*
 * @Author: string
 * @Date: 2024-02-26 10:45:31
 * @LastEditors: string
 * @LastEditTime: 2024-02-28 21:06:50
 * @FilePath: /new_cpp_server/cpp_server/src/Client/HttpClient.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <cpp_server/Client/HttpClient.h>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;

unordered_map<string, std::vector<std::function<void(Req&, Res&)>>> HttpClient::get_progress;
unordered_map<string, std::vector<std::function<void(Req&, Res&)>>> HttpClient::post_progress;

/**
 * @brief 初始化httpclient
 *
 * @param fd_
 */
HttpClient::HttpClient(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll)
    :Client(fd_, _epollfd, _BUF, is_need_epoll),
    req(kv_data, header, body)
{
    reset();
};

/**
 * @brief 重置状态，用于长连接
 *
 */
void HttpClient::reset()
{
    send_buf.clear();
    rec_buf.clear();
    header.clear();
    kv_data.clear();
    http_state = HTTP_STATE::URL;
    res.http_status = 200;
    res.header.clear();
    res.header["Connection"] = "Keep-Alive";
    res.header["Keep-Alive"] = "timeout=20000";
    res.header["Content-Type"] = "text/plain";
    res.header["Server"] = "string\'s server";
}

/**
 * @brief http读事件
 *
 */
void HttpClient::read_fn()
{
    // 如果读关闭了
    if (is_read_close){
        is_close = true;return;
    }
        

    // 开始解析
    string line;

    // 如果数据需要被按行处理
    if (http_state == HTTP_STATE::URL || http_state == HTTP_STATE::HEAD){
        while (!is_close && get_line(line)){
            switch (http_state)
            {
            case HTTP_STATE::URL:
                http_state = parse_url(line);
                break;
            case HTTP_STATE::HEAD:
                http_state = parse_head(line);
                break;
            default:
                goto NO_NEED_LINE;
            case HTTP_STATE::BODY:
                http_state = parse_body();
            case HTTP_STATE::BAD:
                is_close = true;
            }
        }
    }
    NO_NEED_LINE:
    if (http_state == HTTP_STATE::BODY)
        http_state = parse_body();

    // http请求读取完成
    if (http_state == HTTP_STATE::OK){
        printf("http读取完成\n");
        string &url = header["url"];
        // 如果是GET请求
        if (header["method"] == "GET"){
            auto progress_list = get_progress.find(url);
            if (progress_list == get_progress.end())
                res.http_status = 404;
            else{
                res.http_status = 200;
                for(auto &fn: progress_list->second){
                    fn(req, res);
                }
            }
        } else if (header["method"] == "POST"){
            auto progress_list = post_progress.find(url);
            if (progress_list == post_progress.end())
                res.http_status = 404;
            else{
                res.http_status = 200;
                for(auto &fn: progress_list->second){
                    fn(req, res);
                }
            }
        } else
            res.http_status = 404;
        res2send_buf();
        change_to_write();
    } else if (http_state == HTTP_STATE::BAD)
        is_close = true;
}

void HttpClient::res2send_buf(){
    send_buf.clear();
    switch (res.http_status)
    {
    case 200:
        send_buf += "HTTP/1.1 200 OK\r\n";
        break;
    case 404:
        send_buf += "HTTP/1.1 404 Not Found!\r\n";
        res.send_buf = "404了";
        break;
    }
    res.header["Content-Length"] = std::to_string(res.send_buf.size());
    for(const auto &temp: res.header)
        send_buf += (temp.first+ ": " + temp.second + "\r\n");
    send_buf += "\r\n";
    send_buf += res.send_buf;
}

HttpClient::HTTP_STATE HttpClient::parse_url(const string &line)
{
    auto line_res = split_const(line, {' ', '\t'});

    if (line_res.size() == 3)
    {
        header["method"] = line_res[0];
        header["version"] = line_res[2];

        if (line_res[1].find("?") != string::npos)
        {
            auto s_res = split_const(line_res[1], {'?'});
            header["url"] = s_res[0];
            s_res = split_const(s_res[1], {'&'});

            for (const auto &temp : s_res)
            {
                const auto m_res = split_const(temp, {'='});
                kv_data[m_res[0]] = m_res[1];
            }
        }
        else
            header["url"] = line_res[1];
        return HTTP_STATE::HEAD;
    }
    
    return HTTP_STATE::BAD;
}
HttpClient::HTTP_STATE HttpClient::parse_head(const string &line)
{
    // 最后一行了
    if (line.size() == 0)
    {
        if (header["method"] == "GET")
        {
            if (header.find("Sec-WebSocket-Extensions") != header.end())
            {
                return HTTP_STATE::WS;
            }
            else
            {
                return HTTP_STATE::OK;
            }
        }
        else if (header["method"] == "POST")
        {
            return HTTP_STATE::BODY;
        }
        else
            return HTTP_STATE::BAD;
    }
    // 不是最后一行
    auto line_res = split_const(line, {':'});

    static const string care_key[] = {"Connection", "Content-Length", "Content-Type",
                                     "Sec-WebSocket-Extensions", "Sec-WebSocket-Key"};
    if (line_res.size() > 1)
    {
        for (const auto &t : care_key)
        {
            if (line_res[0] == t)
                header[t] = line_res[1];
        }
        return HTTP_STATE::HEAD;
    }
    else
        return HTTP_STATE::BAD;
}
HttpClient::HTTP_STATE HttpClient::parse_body()
{
    // if (rec_buf.size() < atoi(header["Content-Type"].c_str()))
    if (rec_buf.size() < atoi(header["Content-Length"].c_str()))
        return HTTP_STATE::BODY;
    else
    {
        if (header["Content-Type"] == "application/x-www-form-urlencoded") // 这种类型才解析
        {
            const auto &s_res = split_const(rec_buf, {'&'});
            for (const auto &temp : s_res)
            {
                const auto m_res = split_const(temp, {'='});
                kv_data[m_res[0]] = m_res[1];
            }
        }
        return HTTP_STATE::OK;
    }
}

/**
 * @brief websocke握手
 *
 */
// void HttpClient::handle_ws_connect()
// {

//     char sha1_data[30] = {0};
//     char sec_accept[32] = {0};
//     string sha_in = header["Sec-WebSocket-Key"] + WSClient::GUID;
//     SHA1((unsigned char *)&sha_in[0], sha_in.size(), (unsigned char *)sha1_data);
//     // base64_encode(&sha1_data[0],strlen(sha1_data),sec_accept);
//     string res = m_base64_encode((unsigned char *)&sha1_data, strlen(sha1_data)); // sec_accept;
//     send_buf =
//         "HTTP/1.1 101 Switching Protocols\r\n"
//         "Upgrade: websocket\r\n"
//         "Connection: Upgrade\r\n"
//         "Sec-WebSocket-Accept: " +
//         res + "\r\n\r\n";
// }

void HttpClient::write_fn()
{
    if (header["Connection"] != "Keep-Alive")
        is_close = true;
    reset();
    change_to_read();
}

// void HttpClient::read_to_send(const Res &res)
// {
//     send_buf.clear();
//     send_buf += send_first + "\r\n";
//     header["Content-Length"] = std::to_string(res.get_buf().size());
//     for (const auto &temp : header)
//         send_buf += (temp.first + ": " + temp.second + "\r\n");
//     send_buf += "\r\n";
//     send_buf += res.get_buf();
// }

bool HttpClient::get_line(string &line)
{
    int n;
    if ((n = rec_buf.find("\r\n")) != string::npos)
    {
        line = rec_buf.substr(0, n);
        rec_buf.erase(0, n + 2);
        return true;
    }
    else
        return false;
}