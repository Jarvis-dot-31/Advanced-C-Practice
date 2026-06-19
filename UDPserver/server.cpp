#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<cstring>
#include<vector>
#include<utility>

#define PORT "7070"

int main(){
    struct addrinfo hints,*res;
    struct sockaddr_storage their_addr;
    socklen_t siz=sizeof(their_addr);
    std::vector<std::pair<sockaddr_storage,socklen_t>>connections;
    memset(&hints,0,sizeof(hints));
    hints.ai_flags=AI_PASSIVE;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_family=AF_UNSPEC;
    int r=getaddrinfo(NULL,PORT,&hints,&res);
    if (r) return 0;
    if (r) return 0;
    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1) return 0;
    r=bind(sockfd,res->ai_addr,res->ai_addrlen);
    if (r==-1) return 0;
    char msg[]="Success";
    char recmsg[2048];
    while (true){
        socklen_t siz=sizeof(their_addr);
        recvfrom(sockfd,recmsg,sizeof(recmsg),0,(sockaddr*)&their_addr,&siz);
        if (strcmp(recmsg, "OK") != 0) continue;
        connections.push_back({their_addr,siz});
        for (auto &addr:connections){
            sendto(sockfd,msg,sizeof(msg),0,(sockaddr*)&addr.first,addr.second);
        }
    }
}