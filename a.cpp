#include<iostream>

template<typename T>
void f(T& param){
    std::cout<<decltype(param);
    std::cout<<param[0]<<"\n";
}

int main(){
    int arr[]={1,23};
    f(arr);
}