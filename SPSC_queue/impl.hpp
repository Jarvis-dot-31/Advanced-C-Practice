#include<atomic>

template<typename T>
class SPSCqueue{
    private:
        static constexpr std::size_t siz=2048;
        static constexpr std::size_t mask=siz-1;
        T arr[siz];
        alignas(64) std::atomic<int> head;
        alignas(64) std::atomic<int> tail;
    public:
        SPSCqueue(): head(0), tail(0){}
        ~SPSCqueue(){}
        inline bool push(const T& pkt) noexcept{
            int t=tail.load(std::memory_order_relaxed);
            int h=head.load(std::memory_order_acquire);
            if (((t+1)&mask)==h) return false;
            arr[t]=pkt;
            t=((t+1)&mask);
            tail.store(t,std::memory_order_release);
            return true;
        }
        inline bool pop(T& poped) noexcept{
            int h=head.load(std::memory_order_relaxed);
            int t=tail.load(std::memory_order_acquire);
            if (h==t) return false;
            poped=std::move(arr[h]);
            h=((h+1)&mask);
            head.store(h,std::memory_order_release);
            return true;
        }
};