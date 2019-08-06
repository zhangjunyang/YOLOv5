#include "TemplateSetGet_1.h"
using namespace std;

//成员函数的定义
void Student::setname(char *name){
    m_name = name;
}
void Student::getname(){
    return m_name;
}

void Student::setage(int age){
    m_age = age;
}
void Student::getage(){
    return m_age;
}

void Student::setscore(float score){
    m_score = score;
}
void Student::getscore(){
    return m_score; 
}

void Student::show(){
    cout<<getname()<<"的年龄是"<<getage()<<"，成绩是"<<getscore()<<endl;
}

int main(){
    //在栈上创建对象
    Student stu;
    stu.setname("小明");
    stu.setage(15);
    stu.setscore(92.5f);
    stu.show();

    //在堆上创建对象
    Student *pstu = new Student;
    pstu -> setname("李华");
    pstu -> setage(16);
    pstu -> setscore(96);
    pstu -> show();
    delete pstu;

    // //三个成员变量都是私有的，不能通过对象直接访问，所以必须借助三个public属性的成员函数来修改它们的值。下面的代码是错误的：
    // Student stu;
    // stu.m_name("小明");
    // stu.m_age(15);
    // stu.m_score(92.5f);
    // stu.show();
    // //三个成员变量都是私有的，不能通过对象直接访问，所以必须借助三个public属性的成员函数来修改它们的值。下面的代码是错误的：
    // Student *pstu = new Student;
    // pstu -> m_name("李华");
    // pstu -> m_age(16);
    // pstu -> m_score(96);
    // pstu -> show();
    // delete pstu;

    return 0;
}