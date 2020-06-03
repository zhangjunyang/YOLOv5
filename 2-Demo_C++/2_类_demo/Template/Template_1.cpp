#include "Template_1.h"
using namespace std;

//定义构造函数
Student::Student(char *name, int age, float score) {
    m_name = name;
    m_age = age;
    m_score = score;
}
//定义析构函数
Student::~Student(char *name, int age, float score) {

}
//定义普通成员函数
void Student::show(){
    cout<<m_name<<"的年龄是"<<m_age<<"，成绩是"<<m_score<<endl;
}

int main(){
    //创建对象时向构造函数传参
    // Student stu("小明", 15, 92.5f);
    Student stu;
    stu.Student("小明", 15, 92.5f);
    stu.show();
    //创建对象时向构造函数传参
    // Student *pstu = new Student("李华", 16, 96);
    Student *pstu = new Student;
    pstu -> Student("李华", 16, 96);
    pstu -> show();
    delete pstu;

    return 0;
}