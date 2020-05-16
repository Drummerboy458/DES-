#ifndef CONFIG_H
#define CONFIG_H
#include "des-src/des.h"
#include "rsa-src/rsa.h"

#include <iostream>
#include <string>
#include "sys/socket.h"//提供socket函数及数据结构
#include "unistd.h"//提供通用的文件、目录、程序及进程操作的函数
#include "arpa/inet.h"//提供IP地址转换函数
using namespace std;

//TCP客户端与服务端之间提前约定好的密钥及最大的传输长度防止接收方缓冲区溢出
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

bool send_public_key(int sockfd,Public_key p_key)   //发送RSA公钥
{
    size_t size  = sizeof(Public_key);
    size_t total = 0;
    while(total < size)
    {
        size_t wr = write(sockfd,&p_key, size);
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
bool send_des_key(int sockfd,unsigned int* encrpt_des_key)  // 发送加密后的DES密钥
{
    size_t size  = sizeof(unsigned int) * 8;
    size_t total = 0;
    while(total < size)
    {
        size_t wr = write(sockfd,encrpt_des_key,size);
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