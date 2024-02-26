/*
 * @Author: string
 * @Date: 2024-02-26 11:09:16
 * @LastEditors: string
 * @LastEditTime: 2024-02-26 11:14:18
 * @FilePath: /new_cpp_server/cpp_server/src/util.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include<cpp_server/util.h>

// 设置描述符为非阻塞
void set_fd_noblock(int &fd)
{
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}


// 分割字符
vector<string> split(string &s, const char *sp)
{
  vector<string> res;
  std::size_t be = 0, next = 0;
  auto be_it = s.begin();
  while((next = s.find(sp, be)) != string::npos)
  {
    res.emplace_back(be_it + be, be_it + next);
    be = next+2;
  }
  s.assign(be_it + be, s.end());
  return res;
}