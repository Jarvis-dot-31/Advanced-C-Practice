#pragma once
#include<liburing.h>
#include<fcntl.h>
#include<string>
#include<stdexcept>
#include "global.hpp"

inline struct io_uring ring;

void wait(int r){
    for (int i=0;i<r;i++){
        io_uring_cqe *cqe;
        int r=io_uring_wait_cqe(&ring,&cqe);
        if (r<0){
            throw std::runtime_error("wait_cqe failed");
        }
        if (cqe->res<0){
            throw std::runtime_error("write failed");
        }
        io_uring_cqe_seen(&ring,cqe);
    }
}

void batch_writer(){
    io_uring_queue_init(512,&ring,0);
    int filefd=open("data.txt",O_CREAT | O_WRONLY | O_APPEND,0644);
    off_t offset_=lseek(filefd,0,SEEK_END);
    struct pkt tp;
    while(true){
        auto *sqe=io_uring_get_sqe(&ring);
        if (!sqe){
            int r=io_uring_submit(&ring);
            if (r<0){
                throw std::runtime_error("submit in batch_writer failed");
            }
            wait(r);
            continue;
        }
        if (!qu->pop(tp)) continue;
        std::string data=std::to_string(tp.data);
        io_uring_prep_write(sqe,filefd,&data,data.size(),offset_);
        offset_+=sizeof(data);
    }
}