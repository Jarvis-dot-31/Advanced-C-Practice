#pragma once
#include<iostream>
#include <cstddef>
#include "memory_pool.hpp"

struct alignas(std::max_align_t) Block32 {
    char data[32];
};
struct alignas(std::max_align_t) Block64 {
    char data[64];
};
struct alignas(std::max_align_t) Block128 {
    char data[128];
};
struct alignas(std::max_align_t) Block256 {
    char data[256];
};
class Slaballocator{
    private:
        MemoryPool<Block32,1024>pool32;
        MemoryPool<Block64,1024>pool64;
        MemoryPool<Block128,1024>pool128;
        MemoryPool<Block256,1024>pool256;
    public:
        Slaballocator(){};
        ~Slaballocator()=default;
        char* alloc(size_t bytes){
            if (bytes<=32) return pool32.alloc()->data;
            else if (bytes<=64) return pool64.alloc()->data;
            else if (bytes<=128) return pool128.alloc()->data;
            else if (bytes<=256) return pool256.alloc()->data;
            else throw std::bad_alloc();
        }
        void dealloc(char*tp,size_t bytes){
            if (!tp) return;
            if (bytes<=32) pool32.dealloc(reinterpret_cast<Block32*>(tp));
            else if (bytes<=64) pool64.dealloc(reinterpret_cast<Block64*>(tp));
            else if (bytes<=128) pool128.dealloc(reinterpret_cast<Block128*>(tp));
            else if (bytes<=256) pool256.dealloc(reinterpret_cast<Block256*>(tp));
            else throw std::bad_alloc();
        }
};
