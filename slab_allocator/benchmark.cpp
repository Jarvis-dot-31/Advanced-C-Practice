#pragma once
#include<iostream>
#include <memory>
#include <chrono>
#include <vector>
#include "slab_allocator.hpp"

Slaballocator allocator;
int main(){
    auto start = std::chrono::steady_clock::now();
    std::vector<char*> ptrs;
    ptrs.reserve(1024);

    for(int r=0;r<1000000;r++) {
        for(int i=0;i<1024;i++) {
            auto* p = allocator.alloc(64);
            ptrs.push_back(p);
        }
        for(auto* p : ptrs)
            allocator.dealloc(p,64);

        ptrs.clear();
    }
    auto end = std::chrono::steady_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();
    std::cout<<"Slab: "<<ns<<" ms\n";
    start = std::chrono::steady_clock::now();
    std::vector<long long*> ptrs1;
    ptrs1.reserve(1024); 
    volatile long long sum = 0;
    for(int r = 0; r < 1000000; r++) {
        for(int i = 0; i < 1024; i++) {
            auto* p = new long long[8];
            ptrs1.push_back(p);
        }

        for(auto* p : ptrs1)
            delete[] p;

        ptrs1.clear();
    }
    end = std::chrono::steady_clock::now();
    ns = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();
    std::cout<<"New: "<<ns<<" ms\n";
}
