#include <iostream>
using namespace std;

//类的声明
class Student{
private:  //私有的
    char *m_name;
    int m_age;
    float m_score;

public:  
    //声明构造函数
    Student(char *name, int age, float score);
    //声明析构函数
    // ~Student(char *name, int age, float score);
    virtual ~Student(char *name, int age, float score);
    //声明普通成员函数
    void show();
};