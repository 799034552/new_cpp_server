/*
 * @Author: string
 * @Date: 2024-02-26 11:09:16
 * @LastEditors: string
 * @LastEditTime: 2024-02-29 18:53:39
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


static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
std::string m_base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
 
	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
 
			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}
 
	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';
 
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
 
		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];
 
		while ((i++ < 3))
			ret += '=';
 
	}
 
	return ret;
}

void umask(char *data,int len,char *mask)
{
    int i;
    for (i=0;i<len;++i)
        *(data+i) ^= *(mask+(i%4));
}