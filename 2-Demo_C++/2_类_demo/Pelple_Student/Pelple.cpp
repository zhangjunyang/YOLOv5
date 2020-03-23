#include<iostream>
#include "People.h"
using namespace std;

//基类 Pelple 定义
void People::setname(char *name){ 
    m_name = name; 
}
void People::setage(int age){ 
    m_age = age; 
}
char* People::getname(){ 
    return m_name; 
}
int People::getage(){ 
    return m_age;
}
