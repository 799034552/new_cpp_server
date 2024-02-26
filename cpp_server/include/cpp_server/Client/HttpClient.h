#pragma once
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;
// HTTP请求的Req
class Req {
  public:
    // 判断是否是json数据
    bool is_json;
    std::unordered_map<string,string> *data;
    // 普通数据的text
    string *text;
    Req() = default;
    void set(bool is_json_, std::unordered_map<string,string> *data_, string *text_)
      {is_json = is_json_; data = data_; text =text_;}
};

class Res {
  private:
    // 发送缓存
    string send_buf;
    std::unordered_map<string,string> &header;
  public:
    void send(const string &s) { send_buf += s; }
    void add_header(const string &a, const string &b) {header[a] = b;}
    string get_buf() const { return send_buf; }
    Res(std::unordered_map<string,string> &header_):header(header_){}
};