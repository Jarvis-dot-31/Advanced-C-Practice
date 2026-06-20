#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/epoll.h>
#include<openssl/sha.h>
#include<openssl/bio.h>
#include<openssl/evp.h>
#include<openssl/buffer.h>
#include<cstring>
#include<fcntl.h>
#include<iostream>
#include<unordered_map>
#include<queue>

enum class status{
    HANDSHAKE,
    WEBSOCKET
};
unsigned char bit1=0b10000001;
unsigned char bit2=0b00001011;
unsigned char hello[]={bit1,bit2,'o','k','a','y','!',' ','B','i','t','c','h'};

std::string base64_encode(const unsigned char* input, int length) {
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);

    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    std::string result(bptr->data, bptr->length);
    BIO_free_all(b64);

    return result;
}

bool do_handshake(int newfd){
    char buffer[2048];
    int r;
    for (int i=0;i<10;i++){
        r=recv(newfd,buffer,2048,0);
        if (r<=0) continue;
        else{ 
            buffer[r]='\0';
            break;
        }
    }
    if (r<=0) return false;
    std::string req(buffer);
    std::string head="Sec-WebSocket-Key";
    int id=req.find(head);
    if(id==std::string::npos) return false;
    id+=head.size()+2;
    std::string key="";
    while(req[id]!='\n' && req[id]!='\r'){
        key.push_back(req[id]);
        id++;
    }
    std::string GUID="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    key=key+GUID;
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)key.c_str(),key.length(),hash);
    std::string acceptance_key=base64_encode(hash,SHA_DIGEST_LENGTH);
    std::string res="HTTP/1.1 101 Switching Protocols\r\n"
                "Upgrade: websocket\r\n"
                "Connection: Upgrade\r\n"
                "Sec-WebSocket-Accept: " + acceptance_key + "\r\n\r\n";
    r=send(newfd,res.c_str(),res.length(),0);
    if (r<=0) return false;
    return true;
}

int main(){
    struct addrinfo hints,*res;
    struct epoll_event ev;
    memset(&hints,0,sizeof(hints));
    std::unordered_map<int,status>curr_status;
    std::queue<std::pair<int,epoll_event>>reply;
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    int epfd=epoll_create1(0);
    int r=getaddrinfo(NULL,"7070",&hints,&res);
    if (r){ 
        std::cout<<"Failed to get addrinfo\n";
        return 0;
    }
    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1){ 
        std::cout<<"Couldn't provide sockfd\n";
        return 0;
    }
    int yes=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
    fcntl(sockfd,F_SETFL,O_NONBLOCK);
    r=bind(sockfd,res->ai_addr,res->ai_addrlen);
    if (r==-1){ 
        std::cout<<"Couldn't bind to the port\n";
        return 0;
    }
    ev.data.fd=sockfd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
    listen(sockfd,10);
    while (true){
        struct epoll_event events[64];
        int n=epoll_wait(epfd,events,64,100);
        for (int i=0;i<n;i++){
            if (events[i].data.fd==sockfd){
                int newfd=accept(sockfd,nullptr,nullptr);
                if (newfd<=0) continue;
                fcntl(newfd,F_SETFL,O_NONBLOCK);
                struct epoll_event tp;
                tp.data.fd=newfd;
                tp.events=EPOLLIN | EPOLLHUP;
                epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&tp);
                curr_status[newfd]=status::HANDSHAKE;
            }else{
                if (events[i].events & EPOLLHUP){
                    int id=events[i].data.fd;
                    close(id);
                    curr_status.erase(id);
                    continue;
                }
                if (curr_status[events[i].data.fd]==status::HANDSHAKE && do_handshake(events[i].data.fd)){
                    curr_status[events[i].data.fd]=status::WEBSOCKET;
                }else{
                    char msg[2048];
                    r=recv(events[i].data.fd,msg,2048,0);
                    if (r<=0){
                        epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&events[i]);
                        close(events[i].data.fd);
                        curr_status.erase(events[i].data.fd);    
                        continue;
                    }
                    int opcode = msg[0]&0x0F;
                    int payload_len = msg[1]&0x7F;
                    unsigned char mask[4];
                    for(int i=0;i<4;i++)
                        mask[i]=msg[2+i];

                    std::string payload;
                    for(int i=0;i<payload_len;i++){
                        payload.push_back(
                            msg[6+i]^mask[i % 4]
                        );
                    }
                    std::cout<<"From Client"<<events[i].data.fd<<" "<<payload<<"\n";
                    reply.push({(int)events[i].data.fd,events[i]});
                }
            }
        }
        while(!reply.empty()){
            auto id=reply.front();
            reply.pop();
            r=send(id.first,hello,sizeof(hello),0);
            if (r<=0){
                epoll_ctl(epfd,EPOLL_CTL_DEL,id.first,&id.second);
                close(id.first);
                curr_status.erase(id.first);
            }
        }
    }  
}