#include<sys/socket.h>
#include<unistd.h>
#include<cstring>
#include<netdb.h>
#include<fcntl.h>
#include<poll.h>
#include<iostream>
#define PORT "7070"

int main(){
    struct addrinfo hints,*res;
    struct pollfd fds[10];
    int id=0;
    memset(&hints,0,sizeof(hints));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_family=AF_UNSPEC;
    hints.ai_flags=AI_PASSIVE;
    int r=getaddrinfo(NULL,PORT,&hints,&res);
    if (r) return 0;

    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    fcntl(sockfd,F_SETFL,O_NONBLOCK);
    if (sockfd==-1) return 0;

    r=bind(sockfd,res->ai_addr,res->ai_addrlen);
    if (r==-1) return 0;
    listen(sockfd,10);

    struct pollfd listenfd;
    listenfd.fd=sockfd;
    listenfd.events=POLLIN;
    fds[0]=listenfd;
    id++;

    char sucmsg[]="SUCCESS";
    char faimsg[]="FAILURE";
    while(true){
        int ready=poll(fds,id,500);
        if (ready<=0) continue;
        if (fds[0].revents & POLLIN){
            int newfd=accept(sockfd,NULL,NULL);
            if (newfd!=-1){
                fcntl(newfd,F_SETFL,O_NONBLOCK);
                if (id<10){
                    struct pollfd temp;
                    temp.fd=newfd;
                    temp.events=POLLIN;
                    fds[id]=temp;
                    id++;
                    r=send(newfd,sucmsg,sizeof(sucmsg),0);
                    if (r<=0) continue;
                }else{
                    r=send(newfd,faimsg,sizeof(faimsg),0);
                    close(newfd);
                }
            }
        }
        for (int i=1;i<id;i++){
            if (!(fds[i].revents & POLLIN) || fds[i].fd==-1) continue;
            char msg[2048];
            r=recv(fds[i].fd,msg,sizeof(msg),0);
            if (r<=0){
                close(fds[i].fd);
                fds[i].fd=-1;
                continue;
            }
            std::cout<<"Message from Client "<<i<<" "<<msg<<"\n";
        }
    }
}