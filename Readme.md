# web c++服务器
> linux c++服务器，由于c++语法本身繁琐，大多数的cpp服务器都不能像node服务器一样能快速上手，因此，打算做一个仿nodejs的express框架风格的c++服务器，懂一点点c++就能快速上手部署

# 亮点

✅大文件使用异步IO（io_uring），防止阻塞

✅支持websocket服务器推送

✅仿nodejs的express框架语法，上手简单

✅支持get、post，以及静态文件

# 快速编译

基于cmake，需要先安装[liburing](https://github.com/axboe/liburing)（异步io封装）与openssl：`sudo apt-get install libssl-dev`

```cpp
cmake -B build
cmake --build build
./build/test_cpp_server
```

# 快速使用

## GET请求

```c++
#include<cpp_server/Server.h>
#include<stdio.h>
int main(){
    Server app;
    // 监听端口
    app.listen(8000, []{
        printf("监听在8000端口\n");
    }, 4);
    
    // get请求
    app.get("/", [](Req &req, Res &res){
        printf("name: %s\n", req.kv_data.at("name").c_str()); // 获取get参数
        res.send("hello get");
    });
    app.run();
}
```

## POST请求

```c++
app.post("/", [](Req &req, Res &res){
    printf("name: %s\n", req.kv_data.at("name").c_str()); // 获取post参数
    res.send("hello post");
});
```

## websocket请求

```c++
app.ws("/websocket", [](WSClient* client){
    client->on_connect([](WSClient* _client){
        _client->send("hello socket");
    });
    client->on_get([](string data,WSClient* _client){
        _client->send("i get " + data);
    });
});
```

## 静态文件映射

```c++
app.static_file("/static", "/var/www/static");
// 访问127.0.0.1/static/xxx 就是访问 /var/www/static/xxx
```

# 整体框架

![img](https://cdn.nlark.com/yuque/0/2024/png/22090119/1709215891952-42b79147-098f-4ec2-b4e2-000e98aaa2a9.png)

# webbench压力测试

## 在没有下载文件时

![img](https://cdn.nlark.com/yuque/0/2024/png/22090119/1709214631629-461d20a7-c200-4cd0-9409-c393aa5a9e79.png)

## 正在外网下载一个很大的文件时

![img](https://cdn.nlark.com/yuque/0/2024/png/22090119/1709214743305-7e6dc864-3f8a-4096-a83e-ad048589900d.png)

![img](https://cdn.nlark.com/yuque/0/2024/png/22090119/1709214785255-8c7fd776-5ad8-4386-b110-67a5d1f73fdd.png)

# 参考

* 异步io的参考示例[shuveb/io_uring-by-example: A companion repository for the io_uring by Example article series (github.com)](https://github.com/shuveb/io_uring-by-example/tree/master)