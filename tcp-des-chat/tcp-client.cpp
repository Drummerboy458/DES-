//
//  TCPClient.cpp
//
//  Created by Lww on 2020/3/20.
//  Copyright © 2019 Lww. All rights reserved.
//

#include "config.h"
static int sockfd;
static Des des;
static string key = "12345678"; // 客户端的64为密钥
void chat_with_server()
{
    while(1)
   {
        char buffer[bufferSize];
        memset (buffer,0,sizeof(buffer));
    
        //接收服务端发送来的内容
        if(read(sockfd,buffer,sizeof(buffer))<0)
        {
            perror("read error");
        }
        string s = buffer;
        string decode_data = des.decode(s);
        cout<<"server："<<decode_data<<endl<<">>";
       
        //客户端回应

        string reply = "";
        cin>>reply;
        string encode_reply = des.encode(reply);

        //发送
        if(!send_data(sockfd, encode_reply))
        {
            cout<<"error in chat at send_data()"<<endl;
            exit(1);
        }
        if(reply == "-exit")    //客户端发起exit命令,结束本次通信
            exit(0);
   }
}
void online()
{
    des.setKey(key);
    // 第一步，接收服务端发送的公钥
    Public_key p_key;
    //接收服务端发送来的内容
    if(read(sockfd,&p_key,sizeof(Public_key))<0)
    {
        perror("read error");
    }
    cout<<"接收到来自服务端的RSA公钥<n："<<p_key.n<<",e："<<p_key.e<<">"<<endl;

    // 第二步，将使用 RSA 加密后的DES密钥发送给服务端
    unsigned int* encrpt_des_key = new unsigned int[key.length()];
    const char* des_key = key.c_str();
    for(int i = 0; i < key.length(); i++)
    {
        char a = des_key[i];
        int b = atoi(&a);
        encrpt_des_key[i] = Rsa::Encry(b,p_key);
    }
    // 发送加密des密钥
    if(!send_des_key(sockfd,encrpt_des_key ))
    {
        cout<<"error in chat at send_des_key()"<<endl;
        exit(1);
    }

    chat_with_server();
}
int main(int argc,const char* argv[])
{
    if(argc<3)
    {
        cout<<"usage:"<<argv[0]<<"  #ip #port"<<endl;
        exit(1);
    }
    //(一)创建socket   int socket(int domain, int type, int protocol);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        perror("socket error");
        exit(1);
    }
    
    struct sockaddr_in serveraddr;
    //给结构体f赋值，服务器ip，端口；并转换成网络字节序
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);//int inet_pton(int af, const char *src, void *dst);
                                                        //转换字符串到网络地址，第一个参数af是地址族，转换后存在dst中。
    
    //（二）客户端调用connect函数连接到服务器   int  connect(int  sockfd,  const struct sockaddr *serv_addr, socklen_t addrlen);
    if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }
    else
    {    //（三）调用IO函数(read/write)和服务器端双向通信
      cout<<"与服务器连接成功!\n输入[-help]查看相应请求命令"<<endl;
        while(1)
        {
            char cmd[6];
            memset(cmd,0,sizeof(cmd));
            
            string help = "";
            cout<<">>";
            cin.clear();
            cin.sync();   //清空缓冲区
            cin>>help;
            if(help == "-help")
            {
                cout<<"usage: -chat 开始聊天 \nusage: -exit 退出"<<endl;
            }
            else if (help == "-chat")
            {
                strcpy(cmd,help.c_str());
                if(send_data(sockfd, cmd))    //向服务端请求通信
                {
                        cout<<"begin to chat with the server!"<<endl;
                        online(); //开始进入聊天
                }
                else
                    cout<<"聊天请求失败！"<<endl;
            }
            else if (help == "-exit" || help == "-Exit")
            {
                strcpy(cmd,help.c_str());
                send_data(sockfd, cmd); //请求数据
                exit(0);    //正常退出
            }
            else
            {
                cout<<"syntax error"<<endl;
                continue;
            }
            cout<<endl;
        }
    }
}

