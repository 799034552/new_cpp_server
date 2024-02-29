/*
 * @Author: string
 * @Date: 2024-02-29 16:49:20
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 21:47:34
 * @FilePath: /new_cpp_server/cpp_server/src/Client/WSClient.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <cpp_server/Client/WSClient.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cpp_server/util.h>
const string WSClient::GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
unordered_map<string, function<void(WSClient*)>> WSClient::handle_fn;
WSClient::WSClient(const int &fd_, int _epollfd, char (&_BUF)[BUF_SIZE], bool is_need_epoll, string key, string _url)
    : Client(fd_, _epollfd, _BUF, is_need_epoll), parse_state(PARSE_STATE::TWO_CHAR),url(_url)
{
    send_buf.clear();
    char sha1_data[30] = {0};
    char sec_accept[32] = {0};
    string sha_in = key + GUID;
    SHA1((unsigned char *)&sha_in[0], sha_in.size(), (unsigned char *)sha1_data);
    string res = m_base64_encode((unsigned char *)&sha1_data, strlen(sha1_data)); // sec_accept;
    send_buf =
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: " +
        res + "\r\n\r\n";
    change_to_write();
}

void WSClient::read_fn()
{
    // printf("websocket读取开始\n");
    char one_char;
    while (rec_buf.size() > 0)
    {
        // printf("rec_buf大小%d\n", (int)rec_buf.size());
        switch (parse_state)
        {
        case PARSE_STATE::TWO_CHAR:
            if (rec_buf.size() < 2)
                return;
            one_char = rec_buf[0];
            f_head.fin = (one_char & 0x80) == 0x80;
            f_head.opcode = one_char & 0x0F;
            one_char = rec_buf[1];
            f_head.mask = (one_char & 0x80) == 0X80;
            f_head.payload_length = one_char & 0x7F;
            parse_state = PARSE_STATE::PAY_LOAD_Length;
            rec_buf = rec_buf.substr(2);
            // printf("fin的值%d\n", f_head.fin);
            // if (f_head.fin)
            //     is_close = true;
            break;
        case PARSE_STATE::PAY_LOAD_Length:
            if (f_head.payload_length == 126)
            {
                if (rec_buf.size() < 2)
                    return;
                f_head.payload_length = (rec_buf[0] & 0xFF) << 8 | (rec_buf[1] & 0xFF);
                rec_buf = rec_buf.substr(2);
            }
            else if (f_head.payload_length == 127)
            {
                if (rec_buf.size() < 8)
                    return;
                char temp;

                for (int i = 0; i < 4; i++)
                {
                    temp = rec_buf[i];
                    rec_buf[i] = rec_buf[7 - i];
                    rec_buf[7 - i] = temp;
                }
                memcpy(&(f_head.payload_length), &rec_buf, 8);
                rec_buf = rec_buf.substr(8);
            }
            parse_state = PARSE_STATE::Masking_Key;
            break;
        case PARSE_STATE::Masking_Key:
            if (rec_buf.size() < 4)
                return;
            strncpy(f_head.masking_key, &rec_buf[0], 4);
            parse_state = PARSE_STATE::PAY_LOAD;
            break;
        case PARSE_STATE::PAY_LOAD:
            if (rec_buf.size() < f_head.payload_length)
                return;
            umask(&rec_buf[0], rec_buf.size(), f_head.masking_key);
            parse_state = PARSE_STATE::END;
            goto OUT;
            break;
        }
    }
    OUT:
    if (parse_state == PARSE_STATE::END){
        // 完成了一帧的读取
        // printf("websocket读取完成\n");
        parse_state = PARSE_STATE::TWO_CHAR;
        if (get_fn)
            get_fn(rec_buf,this);
        rec_buf.clear();
        // send_buf = "i get";
        // f_head.payload_length = send_buf.size();
        // send_buf =  get_send_head(&f_head) + send_buf;
        // change_to_write();
    }
}

string get_send_head(frame_head* head)
{
    char response_head[12];
    int head_length = 0;
    if(head->payload_length<126)
    {
        //response_head = (char*)malloc(2);
        response_head[0] = 0x81;
        response_head[1] = head->payload_length;
        head_length = 2;
    }
    else if (head->payload_length<0xFFFF)
    {
        //response_head = (char*)malloc(4);
        response_head[0] = 0x81;
        response_head[1] = 126;
        response_head[2] = (head->payload_length >> 8 & 0xFF);
        response_head[3] = (head->payload_length & 0xFF);
        head_length = 4;
    }
    else
    {
      return "";
    }
    response_head[head_length] = '\0';
    string res = response_head;
    return res;
}

void WSClient::write_fn()
{
    // 如果是第一次发送完成，那么就是建立连接
    if(is_first_send){
        is_first_send = false;
        auto find_res = handle_fn.find(url);
        if (find_res != handle_fn.end()){
            // 如果找到了
            find_res->second(this);
            if (connect_fn)
                connect_fn(this);
        }
    }
    if (send_buf.empty())
        change_to_read();
    else
        change_to_read_write();
}
void WSClient::send(string a){
    write_head = f_head;
    send_buf = a;
    write_head.payload_length = send_buf.size();
    send_buf =  get_send_head(&write_head) + send_buf; 
    change_to_read_write();
}
