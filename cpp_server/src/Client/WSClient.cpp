/*
 * @Author: string
 * @Date: 2024-02-29 16:49:20
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 17:47:23
 * @FilePath: /new_cpp_server/cpp_server/src/Client/WSClient.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include<cpp_server/Client/WSClient.h>
#include<string.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include<cpp_server/util.h>
const string WSClient::GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

WSClient::WSClient(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll, string key)
    :Client(fd_, _epollfd, _BUF, is_need_epoll)
{
    send_buf.clear();
    char sha1_data[30] = {0};
    char sec_accept[32] = {0};
    string sha_in = key + GUID;
    SHA1((unsigned char*)&sha_in[0], sha_in.size(), (unsigned char*)sha1_data);
    string res = m_base64_encode((unsigned char *)&sha1_data, strlen(sha1_data));//sec_accept;
    send_buf = 
        "HTTP/1.1 101 Switching Protocols\r\n" 
        "Upgrade: websocket\r\n" 
        "Connection: Upgrade\r\n" 
        "Sec-WebSocket-Accept: "+ res +"\r\n\r\n";
    change_to_write();
}


