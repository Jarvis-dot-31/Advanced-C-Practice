#pragma once
#include<iostream>

struct FreeNode{
    FreeNode*next;
};
template<typename T,std::size_t siz>
class MemoryPool{
    private:
        T buffer[siz];
        size_t sz=siz;
        FreeNode*head;
    public:
        MemoryPool(){
            this->head=reinterpret_cast<FreeNode*>(&buffer[0]);
            for (int i=0;i<this->sz;i++){
                auto* curr=reinterpret_cast<FreeNode*>(&buffer[i]);
                if (i+1<this->sz) curr->next=reinterpret_cast<FreeNode*>(&buffer[i+1]);
                else curr->next=nullptr;
            }
        }    
        ~MemoryPool() = default;
        T* alloc(){
            if (!head){
                throw std::bad_alloc();
            }
            auto*node=head;
            head=head->next;
            return reinterpret_cast<T*>(node);
        }  
        void dealloc(T* pkt){
            auto*node=reinterpret_cast<FreeNode*>(pkt);
            node->next=head;
            head=node;
        }
};