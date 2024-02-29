/*
 * @Author: string
 * @Date: 2024-02-29 09:19:11
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 09:23:46
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
    app.get("/", [](Req &req, Res &res){
        res.send("hello get");
    });
    app.post("/", [](Req &req, Res &res){
        res.send("hello post");
    });
    app.run();
}