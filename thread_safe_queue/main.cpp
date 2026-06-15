#include<iostream>
#include<thread>
#include "global.hpp"
#include "queueImpl.hpp"

int prod=4;
int consum=4;
auto q=std::make_unique<ThreadSafeQueue>();


void produce(){
    while (id<data.size()){
        if (q->push(data[id])) id++;
    }
}
void consume(){
    std::int64_t poped;
    while (q->pop(poped)){
        std::cout<<poped<<" ";
    }
}

int main(){
    for (int i=1;i<2050;i++) data.push_back(static_cast<int64_t>(i));
    id=0;
    for (int i=0;i<prod;i++) producers.emplace_back(produce);
    for (int i=0;i<consum;i++) consumers.emplace_back(consume);
}
