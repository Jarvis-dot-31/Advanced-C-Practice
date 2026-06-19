#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>
#include<cstring>
#include<iostream>
#define PORT "6969"
#define PORT1 "7070"

int main(){
    struct addrinfo hints,*res;
    struct addrinfo servhints,*servres;
    memset(&hints,0,sizeof(hints));
    memset(&servhints,0,sizeof(servhints));
    servhints.ai_family=AF_UNSPEC;
    servhints.ai_socktype=SOCK_DGRAM;
    servhints.ai_flags=AI_PASSIVE;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_family=AF_UNSPEC;
    hints.ai_flags=AI_PASSIVE;
    int r=getaddrinfo(NULL,PORT,&hints,&res);
    if (r) return 0;
    r=getaddrinfo(NULL,PORT1,&servhints,&servres);
    if (r) return 0;
    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1) return 0;
    r=bind(sockfd,res->ai_addr,res->ai_addrlen);
    if (r==-1) return 0;
    char msg[2048];
    char app_msg[]="OK";
    r=sendto(sockfd,app_msg,sizeof(app_msg),0,servres->ai_addr,servres->ai_addrlen);
    if (r<=0) return 0;
    while (true)
    {
        r=recvfrom(sockfd,msg,sizeof(msg),0,nullptr,nullptr);
        if (r<=0) return 0;
        std::cout<<msg<<"\n";
    }
    
}