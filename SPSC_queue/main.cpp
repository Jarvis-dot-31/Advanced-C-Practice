#include<iostream>
#include<thread>
#include "impl.hpp"
#include<atomic>

struct pkt{
    char value;
    pkt(char v):value(v){}
    pkt(){}
};
std::atomic<bool>stop_flag{false};
auto q=std::make_unique<SPSCqueue<pkt>>();
void process_pkt() {
    pkt popped;
    while (true) {
        if (q->pop(popped)) {
            // std::cout<<popped.value<<" ";
        }else if(stop_flag.load(std::memory_order_acquire)) {
            break;
        }
    }
}
void pkt_parser(){
    for (int i=0;i<1e6;i++){
        pkt packet;
        packet.value='a'+i%26;
        while(!q->push(packet));
    }
    stop_flag.store(true,std::memory_order_release);
}
int main(){
    std::jthread producer(pkt_parser);
    std::jthread consumer(process_pkt);
}