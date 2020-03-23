#include <iostream>
using namespace std;
class A{
public:
    A(){cout<<"constructor A "<<endl;}
    ~A(){cout<<"destructor A "<<endl;}
};
class B: public A{
public:
    B(){cout<<"constructor B "<<endl;}
    ~B(){cout<<"destructor B "<<endl;}
};
class C: public B{
public:
    C(){cout<<"constructor C "<<endl;}
    ~C(){cout<<"destructor C "<<endl;}
};
int main(){
    C test;
    return 0;
}

/****************
运行结果：
	constructor A 
	constructor B 
	constructor C 
	destructor C 
	destructor B 
	destructor A 
*****************/