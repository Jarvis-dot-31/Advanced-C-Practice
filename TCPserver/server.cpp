#include<unistd.h>
#include<sys/socket.h>
#include<iostream>
#include<netdb.h>
#include<cstring>
#include<unordered_set>

#define PORT "6969"

int main(){
    struct addrinfo hints,*res;
    std::unordered_set<int>connections;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    getaddrinfo(NULL,PORT,&hints,&res);
    int recfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (recfd==-1){
        throw std::runtime_error("couldn't set recfd");
        return 0;
    }
    if (bind(recfd,res->ai_addr,res->ai_addrlen)==-1) return 0;
    
    char resp[]="Hey There";
    listen(recfd,10);
    while(true){
        int newfd=accept(recfd,NULL,NULL);
        if (newfd==-1) continue;
        connections.insert(newfd);
        for(auto it = connections.begin();it!=connections.end();){
            int fd = *it;
            int r = send(fd, resp, sizeof(resp), MSG_NOSIGNAL);
            if (r <= 0) {
                close(fd);
                it = connections.erase(it);
            }else{
                it++;
            }
        }
    }

}