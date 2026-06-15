#include<mutex>
#include<condition_variable>
#include<iostream>

class ThreadSafeQueue{
    private:
        std::int64_t q[2048];
        std::mutex m;
        int siz=2048;
        int head;
        int tail;
    public:
        ThreadSafeQueue(){
            head=0;
            tail=0;
        }
        bool push(std::int64_t val){
            std::unique_lock<std::mutex>lock(m);
            if ((head+1)%siz==tail) return false;
            q[head%siz]=val;
            head=(head+1)%siz;
            return true;
        }
        bool pop(std::int64_t &poped){
            std::unique_lock<std::mutex>lock(m);
            if (head==tail) return false;
            poped=q[tail];
            tail=(tail+1)%siz;
            return true;
        }
        int size(){
            std::unique_lock<std::mutex>lock(m);
            return siz-abs(head-tail);
        }
        void print(){
            std::unique_lock<std::mutex>lock(m);    
            int th=tail;
            while(th!=head){
                std::cout<<q[th]<<" ";
                th=(th+1)%siz;
            }
        }
};