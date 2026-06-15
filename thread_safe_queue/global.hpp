#include<thread>
#include<vector>
#include<atomic>

std::vector<std::jthread>producers;
std::vector<std::jthread>consumers;
std::vector<std::int64_t> data;
std::atomic<int> id;
