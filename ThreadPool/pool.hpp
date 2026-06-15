#include<thread>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<vector>

class ThreadPool{
    private:
        std::queue<std::function<void()>>task;
        std::mutex mtx;
        std::condition_variable cv;
        bool stop=false;
        std::vector<std::jthread>workers;
        void process(){
            while (true){
                std::unique_lock lock(mtx);
                cv.wait(lock,[&]{ 
                    return stop || !task.empty();
                });
                if (stop && task.empty()) return;
                auto tsk=std::move(task.front());
                task.pop();
                lock.unlock();
                tsk();
            }
        }
    public:
        ThreadPool(size_t num){
            for (int i=0;i<num;i++){
                workers.emplace_back(&ThreadPool::process,this);
            }
        }
        ~ThreadPool(){
            {   
                std::lock_guard lock(mtx);
                stop=true;
            }
            cv.notify_all();
        }
        template<typename F>
        void enqueue(F&& tsk){
            std::unique_lock lock(mtx);
            task.push(std::forward<F>(tsk));
            lock.unlock();
            cv.notify_one();
        }
};