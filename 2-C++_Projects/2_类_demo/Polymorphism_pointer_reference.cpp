#include <iostream>
using namespace std;

//基类People
class People{
public:
    People(char *name, int age);
    virtual void display();  //声明为虚函数
protected:
    char *m_name;
    int m_age;
};

People::People(char *name, int age) {
    m_name = name;
    m_age = age;
}

void People::display(){
    cout<<m_name<<"今年"<<m_age<<"岁了，是个无业游民。"<<endl;
}

//派生类Teacher
class Teacher: public People{
public:
    Teacher(char *name, int age, int salary);
    virtual void display();  //声明为虚函数,为了方便，基类已经声明虚函数，派生类可以省略此处同名的virtual关键字
private:
    int m_salary;
};

Teacher::Teacher(char *name, int age, int salary) {
    People(name, age);
    m_salary = salary;
}

void Teacher::display(){
    cout<<m_name<<"今年"<<m_age<<"岁了，是一名教师，每月有"<<m_salary<<"元的收入。"<<endl;
}

int main(){
    People *p = new People("王志刚", 23); // 指针
    p -> display();
    p = new Teacher("赵宏佳", 45, 8200);
    p -> display();

    /*************************************************************************
        引用类似于常量，只能在定义的同时初始化，并且以后也要从一而终，不能再引用其他数据，所以本例中必须要定义两个引用变量，
        一个用来引用基类对象，一个用来引用派生类对象。不过引用不像指针灵活，指针可以随时改变指向，而引用只能指代固定的对象，
        在多态性方面缺乏表现力，多态时一般是说指针。

    *************************************************************************/
    People  p("王志刚", 23);
    Teacher t("赵宏佳", 45, 8200);
    People &rp = p;    // 引用
    People &rt = t;    // 引用
    p -> display();
    t -> display();

    return 0;
}