/*
 * @Author: string
 * @Date: 2024-02-26 11:09:16
 * @LastEditors: string
 * @LastEditTime: 2024-02-28 16:25:03
 * @FilePath: /new_cpp_server/cpp_server/src/util.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <cpp_server/util.h>

// 设置描述符为非阻塞
void set_fd_noblock(int &fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

// 向epoll事件表中加入可读事件
void epoll_add_read_LT(int epollfd, int fd)
{
    epoll_event t;
    t.data.fd = fd;
    t.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &t);
}

// 使用任意字符进行分割
vector<string> split_const(const string &s, std::initializer_list<const char> sp)
{
    vector<string> res;
    std::size_t be = 0, next = 0;
    auto be_it = s.begin();

    next = string::npos;
    for (const auto &t : sp)
        next = std::min(s.find(t, be), next);
    while (next != string::npos)
    {
        // 不是只有分割符一个选项
        if (be != next)
            res.emplace_back(be_it + be, be_it + next);
        be = next + 1;
        next = string::npos;
        for (const auto &t : sp)
            next = std::min(s.find(t, be), next);
    }
    if (be != s.size())
        res.emplace_back(be_it + be, s.end());

    std::for_each(res.begin(), res.end(), trim);
    return res;
}

void trim(string &s)
{
  auto a = s.begin();
  s.assign(a + s.find_first_not_of(' '),a + s.find_last_not_of(' ')+1);
}