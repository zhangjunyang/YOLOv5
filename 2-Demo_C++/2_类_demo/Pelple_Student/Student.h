#include<iostream>
#include "People.h"
using namespace std;

//派生类 Student 声明
class Student: public People{
public:
    void setscore(float score);
    float getscore();
private:
    float m_score;
};