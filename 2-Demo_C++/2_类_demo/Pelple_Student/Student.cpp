#include<iostream>
#include "Student.h"
using namespace std;

//派生类 Student 定义
void Student::setscore(float score){ 
	m_score = score; 
}
float Student::getscore(){ 
	return m_score; 
}

int main(){
    Student stu;
    stu.setname("小明");
    stu.setage(16);
    stu.setscore(95.5f);
    cout<<stu.getname()<<"的年龄是 "<<stu.getage()<<"，成绩是 "<<stu.getscore()<<endl;

    return 0;
}