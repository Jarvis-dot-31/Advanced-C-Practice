#include<sys/socket.h>
#include<unistd.h>
#include<cstring>
#include<netdb.h>
#include<iostream>
#define PORT "7070"

int main(){
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_family=AF_UNSPEC;
    hints.ai_flags=AI_PASSIVE;
    int r=getaddrinfo(NULL,PORT,&hints,&res);
    if (r) return 0;

    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1) return 0;
    r=connect(sockfd,res->ai_addr,res->ai_addrlen);
    if (r<0) return 0;
    char msg[2048]="FAILURE";
    while(strcmp(msg,"FAILURE")==0){
        r=recv(sockfd,msg,sizeof(msg),0);
        if (r<=0) continue;
    }
    char mymsg[2048]="YO, WASSUP";
    send(sockfd,mymsg,sizeof(mymsg),0);
}