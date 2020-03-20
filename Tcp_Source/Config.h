#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <string>
#include "sys/socket.h"//提供socket函数及数据结构
#include "unistd.h"//提供通用的文件、目录、程序及进程操作的函数
#include "arpa/inet.h"//提供IP地址转换函数
using namespace std;
//TCP客户端与服务端之间提前约定好的密钥及最大的传输长度防止接收方缓冲区溢出
const string key = "123456";
const int bufferSize = 1024;

bool send_data(int sockfd,string data) //发送数据
{
    const char* cmd = data.c_str();
    size_t size  = strlen(cmd)*sizeof(char);
    size_t total = 0;
    while(total < size)
    {
        size_t wr = write(sockfd, cmd, size);
        if(wr < 0)
            perror("write error");
        else
            total += wr;
    }
   if(total == size)
        return true; 
    else
        return false;
}
#endif