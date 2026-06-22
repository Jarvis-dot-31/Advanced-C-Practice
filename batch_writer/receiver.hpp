#pragma once
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<cstring>
#include<stdio.h>
#include<iostream>
#include "global.hpp"

int run_reciver(){
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    int r=getaddrinfo(NULL,"7070",&hints,&res);
    if (r){
        perror("Couldn't get the address");
        return 0;
    }
    int sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sockfd==-1){
        perror("Couldn't allocate sockfd");
        return 0;
    }
    connect(sockfd,res->ai_addr,res->ai_addrlen);
    struct pkt tp;
    while(true){
        int r=recv(sockfd,&tp,sizeof(tp),0);
        if (r<=0) continue;
        if (!qu->push(tp)) continue;
    }
}
