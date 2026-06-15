#include<iostream>
#include<vector>

template<typename T>
std::vector<bool>helper(std::vector<T>&temp){
    return temp;
}

int main(){
    std::vector<bool>temp(10,false);
    auto tp=static_cast<bool>(helper(temp)[5]);
    std::cout<<tp<<"\n";
}