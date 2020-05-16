//
//  TCPServer.cpp
//
//  Created by Lww on 20290/3/20.
//  Copyright © 2019 Lww. All rights reserved.
//
#include "config.h"
static Des des;
static int sockfd; //全局文件描述符，绑定用于监听的socket
//网络字节顺序NBO: 按从高到低的顺序存储，在网络上使用统一的网络字节顺序，避免兼容性问题。
//主机字节顺序HBO: 不同的机器HBO不相同
//输出连接上来的客户端相关信息

void out_addr(struct sockaddr_in *clientaddr)
{
    //将端口从网络字节序转成主机字节序
    int port = ntohs(clientaddr->sin_port);
    char ip[16];
    memset (ip,0,sizeof(ip));
    inet_ntop(AF_INET, &clientaddr->sin_addr.s_addr, ip, sizeof(ip));//将“整数” －> “点分十进制”]
    cout<<"client:"<<ip<<" "<<port<<" connected"<<endl;
}
void chat_with_client(int fd)
{
    while(1)
    {
        //服务端回应
        cout<<">>";
        string reply;
        getline(cin,reply);
        string encode_reply = des.encode(reply);
        
        if(!send_data(fd, encode_reply))
        {
            cout<<"error in chat at send_data()"<<endl;
            exit(1);
        }

        // 客户端回应
        char buffer[bufferSize];
        memset (buffer,0,sizeof(buffer));
        //接收客户端发送来的内容
        if(read(fd,buffer,sizeof(buffer))<0)
        {
            perror("read error");
        }
        string s = buffer;
        string decode_data = des.decode(s);
        if(decode_data[0] == '-' && decode_data[1] == 'e')    //客户端发起exit命令,结束本次通信
                exit(0);
        else
            cout<<"client："<<decode_data<<endl;
    }
}
void do_service(int fd)
{

          // 第一步，生成 RSA 公钥私钥对
        Rsa rsa;
        rsa.RsaGetParam();
        Public_key p_key = rsa.get_public_key();
        Secret_key s_key = rsa.get_secret_key();

        cout<<"公钥<n："<<p_key.n<<",e："<<p_key.e<<">"<<endl;
        cout<<"私钥<n："<<s_key.n<<",d："<<s_key.d<<">"<<endl;

        char buffer[bufferSize];
        memset (buffer,0,sizeof(buffer));
        //接收客户端发送来的内容
        if(read(fd,buffer,sizeof(buffer))<0)
            perror("read error");

        string s = buffer;

        if(s[0] == '-' && s[1] == 'e')    //客户端发起exit命令,结束本次通信
                exit(0);
        
       
       // 第二步，发送公钥对给客户端
        if(!send_public_key(fd, p_key))
        {
            cout<<"error in chat at send_public_key()"<<endl;
            exit(1);
        }

        cout<<"RSA密钥发送成功..."<<endl;
        // 第三步，接收客户端使用RSA公钥加密后的 DES 密钥
         unsigned int* encrpt_des_key = new unsigned int[8];
        //接收服务端发送来的内容
        if(read(fd,encrpt_des_key,sizeof(unsigned int)*8)<0)
            perror("read error");
        
        unsigned int* des_key = new unsigned int[8];
        string key = "";
        for(int i = 0; i < 8; i++)
        {
            key += to_string(Rsa::Decry(encrpt_des_key[i],s_key));
        }
        des.setKey(key);

        cout<<"DES密钥成功接收,可以通信了..."<<endl;
        // 正式与客户端进行通信
        chat_with_client(fd);
      
}
int main(int argc, const char * argv[]) 
{
    if(argc < 2)
    {
        cout<<"usage:"<<argv[0]<<" "<<"#port"<<endl;
    }
    
    //（一）创建socket  int socket(int domain, int type, int protocol);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//0表示自动选择type类型对应的默认协议。
    if(sockfd < 0) //返回-1创建失败
    {
        perror("socket error");
        exit(1);
    }
    //（二）调用bind函数绑定socket和地址  int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
    struct sockaddr_in serveraddr;
    int port  = atoi(argv[1]);
    memset(&serveraddr, 0, sizeof(serveraddr)); //初始化
    serveraddr.sin_family = AF_INET;//ipv4
    serveraddr.sin_port = htons(port); //htons将端口号从主机字节序转成网络字节序
    
    serveraddr.sin_addr.s_addr = INADDR_ANY;//本机的所有IP均可接收,s_addr按照网络字节顺序存储IP地址
    
    if(::bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)//it's in the global namespace
    {
        perror("bind error");
        exit(1);
    }
    //（三）调用listen函数监听  int listen(int socket, int backlog);
    //把进程变为一个服务器，并指定相应的套接字变为被动连接。
    if(listen(sockfd,10) < 0) //第二个参数：指定队列长度
    {
        perror("listen error");
    }
    cout<<"listening at port="<<port<<endl;
    //（四）调用accept函数从队列中取得一个客户端的请求   //int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len=sizeof(clientaddr);
    int fd =  accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddr_len);//提取出所监听套接字的等待连接队列中第一个连接请求，创建一个新的套接字，并返回指向该套接字的文件描述符
    if(fd<0)
    {
        perror("accept error");
    }
    out_addr(&clientaddr);
     //（五）调用IO函数（read/write）和连接的客户端进行双向通信
        //便于演示，当客户端发出exit请求时，执行exit(0)正常退出
    do_service(fd);
    //(六)6.关闭socket
    close(fd);
    return 0;
}

