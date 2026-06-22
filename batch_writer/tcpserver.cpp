#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<cstring>
#include<sys/epoll.h>
#include<fcntl.h>
#include<stdio.h>
#include<unordered_set>
#include "global.hpp"

int main(){
    struct addrinfo hints,*res;
    std::unordered_set<int>connections;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    int r=getaddrinfo(NULL,"7070",&hints,&res);
    if (r){
        perror("Couldn't get the address");
        return 0;
    }
    int listenfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    fcntl(listenfd,F_SETFL,O_NONBLOCK);
    int yes=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
    if (listenfd==-1){
        perror("Couldn't allocate fd for listening");
        return 0;
    }
    r=bind(listenfd,res->ai_addr,res->ai_addrlen);
    if (r<0){
        perror("Couldn't bind to the address");
        return 0;
    }
    listen(listenfd,10);
    int epfd=epoll_create1(0);
    struct epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    while(true){
        struct epoll_event events[128];
        int n=epoll_wait(epfd,events,128,10);
        for (int i=0;i<n;i++){
            if (events[i].data.fd==listenfd){
                while(true){
                    int newfd=accept(listenfd,nullptr,nullptr);
                    if (newfd==-1) break;
                    fcntl(newfd,F_SETFL,O_NONBLOCK);
                    connections.insert(newfd);
                    struct epoll_event ev1;
                    ev1.data.fd=newfd;
                    ev1.events=EPOLLHUP;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&ev1);
                }
            }else if (events[i].events & EPOLLHUP){
                connections.erase(events[i].data.fd);
                epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&events[i]);
                close(events[i].data.fd);
            }
        }
        for (auto it=connections.begin();it!=connections.end();it++){
            struct pkt tp;
            tp.data=69;
            int r=send(*it,&tp,sizeof(tp),0);
            if (r<=0) continue;
        }
    }
}