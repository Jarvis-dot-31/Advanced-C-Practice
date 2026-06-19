#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<cstring>
#include<iostream>

#define PORT "6969"

int main(){
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    getaddrinfo("127.0.0.1",PORT,&hints,&res);
    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1) return 0;
    int r=connect(sockfd,res->ai_addr,res->ai_addrlen);
    if (r==-1) return 0;
    char output[2048];
    while(true){
        r=recv(sockfd,output,sizeof(output),0);
        if (r<=0) return 0; 
        std::cout<<output<<"\n";
    }
}