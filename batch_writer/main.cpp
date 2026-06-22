#include<iostream>
#include<thread>
#include "receiver.hpp"
#include "impl.hpp"
#include "global.hpp"
#include "writer.hpp"

int main(){
    std::jthread receiv(run_reciver);
    std::jthread writer(batch_writer);
}