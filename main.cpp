/*
 * @Author: string
 * @Date: 2024-02-29 09:19:11
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 22:46:14
 * @FilePath: /new_cpp_server/main.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include<cpp_server/Server.h>
#include<stdio.h>
int main(){
    Server app;
    app.listen(8000, []{
        printf("监听在8000端口\n");
    }, 4);

    // get请求
    app.get("/", [](Req &req, Res &res){
        printf("name: %s\n", req.kv_data.at("name").c_str()); // 获取get参数
        res.send("hello get");
    });

    // 静态文件服务
    app.static_file("/static", "/var/www/static");

    // post请求
    app.post("/", [](Req &req, Res &res){
        printf("name: %s\n", req.kv_data.at("name").c_str()); // 获取post参数
        res.send("hello post");
    });

    // websocket请求
    app.ws("/websocket", [](WSClient* client){
        client->on_connect([](WSClient* _client){
            _client->send("hello socket");
        });
        client->on_get([](string data,WSClient* _client){
            _client->send("i get " + data);
        });
    });
    app.run();
}