#include<cpp_server/AsyncHandle.h>
// 与异步线程通讯的描述符
static int notify_fd = eventfd(0,0);


AsyncHandle::AsyncHandle(){
    // 先创建共享区
    for(int i = 0; i < MMAP_NUM; ++i){
        space_list[i] = shared_ptr<MmapSpace>(new MmapSpace("/my_space" + std::to_string(i)));
        avaliable_space_list.push(space_list[i]);
    }
    add_job_notiry.notify_type = NOTIFY_TYPE::ADD_JOB;

}
// 对添加任务的监听
void AsyncHandle::add_notify_listen(){
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring); // 获取一个可用的sq
    io_uring_prep_read(sqe ,notify_fd,  null_buff, sizeof(null_buff), 0); // 准备读取，每次读都直接把队列清空，并且丢弃数据
    io_uring_sqe_set_data(sqe, &add_job_notiry); // 出现响应就是添加序列
    io_uring_submit(&ring);
}
// 开启读任务
void AsyncHandle::read_task(AsyncJobStruct* pass_job){
    pass_job->notify_type = NOTIFY_TYPE::END_READ;
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring); // 获取一个可用的sq
    io_uring_prep_read(sqe, pass_job->file_fd, pass_job->space->addr, pass_job->space->size, 0); 
    io_uring_sqe_set_data(sqe, pass_job);
    io_uring_submit(&ring);
}
// 开启写任务
void AsyncHandle::write_task(AsyncJobStruct* pass_job){
    pass_job->notify_type = NOTIFY_TYPE::END_WRITE;
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring); // 获取一个可用的sq
    io_uring_prep_write(sqe, pass_job->client_fd, pass_job->space->addr, pass_job->read_size, 0); 
    io_uring_sqe_set_data(sqe, pass_job); // 出现响应就是添加序列
    io_uring_submit(&ring);
}
// 尝试读取任务
void AsyncHandle::start_slove_job(){
    pthread_mutex_lock(&mutex);
    while(!avaliable_space_list.empty() && !job_list.empty()){
        // 拿出一个可用的缓存区
        shared_ptr<MmapSpace> space = avaliable_space_list.front();
        avaliable_space_list.pop();
        // 拿出一个任务
        auto job = job_list.back();
        job_list.pop_back();
        AsyncJobStruct* pass_job = new AsyncJobStruct(job); //拷贝构造一个
        pass_job->space = space;
        read_task(pass_job);
    }
    pthread_mutex_unlock(&mutex);
}
void AsyncHandle::run(){
    // 先监听通知的描述符
    io_uring_queue_init(256, &ring, 0); // 创建sq, cq
    add_notify_listen(); // 监听通知

    struct io_uring_cqe *cqe;
    while(1){
        int ret = io_uring_wait_cqe(&ring, &cqe); // 等待接受队列有数据
        if (ret != 0)
            continue;
        AsyncJobStruct &job = *((AsyncJobStruct *) cqe->user_data);

        switch (job.notify_type)
        {
        case NOTIFY_TYPE::ADD_JOB:
            // printf("异步线程有新工作\n");
            start_slove_job();
            add_notify_listen();
            break;
        case NOTIFY_TYPE::END_READ:
            if (cqe->res < 0)
                perror("读取返回异常"),exit(0);
            job.read_size = cqe->res;
            // printf("异步线程读%d\n", job.read_size);
            write_task(&job);
            break;
        case NOTIFY_TYPE::END_WRITE:
            if (cqe->res < 0)
                perror("写入返回异常"),exit(0);
            job.had_send_size += cqe->res;
            // printf("异步线程写%d\n", job.had_send_size);
            if (job.had_send_size < job.file_size){
                read_task(&job);
            } else {
                // 传输完成
                uint64_t tmp = job.client_fd;
                write(job.wake_up_fd, &tmp, sizeof(tmp));
                avaliable_space_list.push(job.space);
                close(job.file_fd);
                delete &job;
                start_slove_job();
            }
            break;
        }
        io_uring_cqe_seen(&ring, cqe);
    }
}


void* start_async_thread(void *h){
    AsyncHandle* handler = (AsyncHandle*)h;
    handler->run();
    return nullptr;
}

// 发送文件
// 文件名 | 要发送去的fd | 发送完成后通知的fd
void AsyncHandleThread::send_file(string file_name, int client_fd,int wake_up_fd){
    AsyncJobStruct job;
    job.file_name = file_name;
    job.client_fd = client_fd;
    job.wake_up_fd = wake_up_fd;
    job.file_fd = open(file_name.c_str(), O_RDONLY);
    struct stat statBuf;
    stat(file_name.c_str(), &statBuf);
    job.file_size = statBuf.st_size;
    
    // 写入工作队列
    pthread_mutex_lock(&handler.mutex);
    handler.job_list.push_back(job);
    pthread_mutex_unlock(&handler.mutex);
    // 通知线程写入了
    uint64_t tmp = 1;
    write(notify_fd, &tmp, sizeof(tmp));
}

shared_ptr<AsyncHandleThread> async_task_io;