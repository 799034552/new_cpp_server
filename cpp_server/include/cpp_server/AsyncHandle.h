/*
 * @Author: string
 * @Date: 2024-02-29 09:29:56
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 16:33:37
 * @FilePath: /new_cpp_server/cpp_server/include/cpp_server/AsyncHandle.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once
#include<string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<memory>
#include<cpp_server/conf.h>
#include<thread>
#include<functional>
#include<vector>
#include <liburing.h>
#include<sys/eventfd.h>
#include<queue>
#include<sys/stat.h>
using std::queue;
using std::shared_ptr;
using std::string;
using std::function;
using std::vector;



class MmapSpace{
    public:
        int size = 400 * 1024;
        char * addr;
        MmapSpace(string name){
            // 创建一个虚拟的fd
            int shmfd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666 );
            ftruncate( shmfd, size ); 
            // 创建共享内存
            addr = (char*)mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
            close( shmfd );
        }
        ~MmapSpace(){
            munmap(addr, size);
        }
};
// 通知的类型
enum class NOTIFY_TYPE {ADD_JOB, END_READ, END_WRITE};

struct AsyncJobStruct{
    string file_name; // 文件名
    int file_fd; // 文件的fd
    int client_fd; // 用户的fd
    int wake_up_fd; // 结束完成后通知的fd
    int file_size; //文件大小
    int had_send_size = 0; //发送的大小
    int read_size = 0; //读取的大小
    NOTIFY_TYPE notify_type; // 通知消息类型
    shared_ptr<MmapSpace> space; //缓冲区
};

class AsyncHandle{
    public:
        // 描述通知类型
        AsyncJobStruct add_job_notiry;
        shared_ptr<MmapSpace> space_list[MMAP_NUM]; //空间列表
        queue<shared_ptr<MmapSpace>> avaliable_space_list; //可用空间列表
        vector<AsyncJobStruct> job_list; // 工作列表
        io_uring ring;
        // 与工作列表相匹配的锁
        pthread_mutex_t mutex;
        char null_buff[100]; // 读通知的无用缓存
        AsyncHandle();
        // 对添加任务的监听
        void add_notify_listen();
        // 开启读任务
        void read_task(AsyncJobStruct* pass_job);
        // 开启写任务
        void write_task(AsyncJobStruct* pass_job);
        // 尝试读取任务
        void start_slove_job();
        void run();
};

void* start_async_thread(void *h);

// 与异步io通信的接口类
class AsyncHandleThread{
    AsyncHandle handler;
    public: 
        AsyncHandleThread(){
            pthread_t p;
            pthread_create(&p, nullptr, start_async_thread, &handler);
        }
        // 发送文件
        // 文件名 | 要发送去的fd | 发送完成后通知的fd
        void send_file(string file_name, int client_fd,int wake_up_fd);
};

extern shared_ptr<AsyncHandleThread> async_task_io;