#include <iostream>
using namespace std;

//类的声明
class Student{
private:  //私有的
    char *m_name;
    int m_age;
    float m_score;

public:  //共有的
    void setname(char *name);
    void getname();
    void setage(int age);
    void getage();
    void setscore(float score);
    void getscore();
    void show();
};