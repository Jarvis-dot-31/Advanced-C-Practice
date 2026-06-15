#include<iostream>

class Base{
    public:
        Base(){}
        virtual void print(int&& x){
            std::cout<<"HI\n";
        }
        void print1(){
            std::cout<<"HI3\n";
        }
};

class Derived: public Base{
    public:
        void print(int&& x)override{
            std::cout<<"Hi2\n";
        }
};

int main(){
    Derived obj;
    obj.print(27);
    obj.print1();
}