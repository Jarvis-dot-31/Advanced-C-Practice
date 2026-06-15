#include<iostream>
#include<atomic>
#include<mutex>
#include "pool.hpp"

std::atomic<int>id{0};
std::mutex cout_mtx;
void print(){
    int curr=id.fetch_add(1);
    std::lock_guard lck(cout_mtx);
    std::cout<<curr<<" ";
}

int main(){
    auto pool=std::make_unique<ThreadPool>(4);
    for (int i=0;i<1000;i++){
        pool->enqueue(print);
    }
}