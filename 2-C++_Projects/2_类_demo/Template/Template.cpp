#include <iostream>
using namespace std;

//类的声明
class Student{
private:  //私有的
    char *m_name;
    int m_age;
    float m_score;

    int *m_arr; //数组指针

     static int m_total;  //静态成员变量

public:  
    //声明构造函数
    Student(); 
    Student(char *name, int age, float score); //声明的构造函数重载
    //声明析构函数
    // ~Student(char *name, int age, float score);
    virtual ~Student(); //析构函数没有参数，不能被重载，因此一个类只能有一个析构函数。如果用户没有定义，编译器会自动生成一个默认的析构函数。
    //声明普通成员函数
    void setname(char *name);
    void setage(int age);
    void setscore(float score);
    void show();
};

// /初始化静态成员变量
int Student::m_total=0;
//定义构造函数
Student::Student(){ 
    m_name = NULL;
    m_age = 0;
    m_score = 0.0;

    int len;
    m_arr = new int[len];  /*分配内存*/ 

    m_total++;  //操作静态成员变量

}
Student::Student(char *name, int age, float score) { //定义的构造函数重载
    m_name = name;
    m_age = age;
    m_score = score;
}
//定义析构函数
Student::~Student(){ 
    delete[] m_arr; // delete 构造函数申请(new)的内存
}

//定义普通成员函数
void Student::setname(char *name){
    m_name = name;
}
void Student::setage(int age){
    m_age = age;
}
void Student::setscore(float score){
    m_score = score;
}
void Student::show(){
    if(m_name == NULL || m_age <= 0){
        cout<<"成员变量还未初始化"<<endl;
    }else{
        cout<<m_name<<"的年龄是"<<m_age<<"，成绩是"<<m_score<<"（当前共有"<<m_total<<"名学生）"<<endl;
    }
}

int main(){
    //创建对象时向构造函数传参
    Student stu;
    stu.Student("小明", 15, 92.5f);
    stu.show();

    //创建对象时向构造函数传参
    // Student *pstu = new Student("李华", 16, 96);
    Student *pstu = new Student;
    // Student *pstu = new Student();
    pstu -> Student("李华", 16, 96);
    pstu -> show();
    delete pstu;


    /*
    * 注意：调用没有参数的构造函数也可以省略括号。创建对象时没有写括号，其实是调用了默认的构造函数。
    * 构造函数在实际开发中会大量使用，它往往用来做一些初始化工作，例如对成员变量赋值、预先打开文件等。
    * 构造函数的调用是强制性的，一旦在类中定义了构造函数，那么创建对象时就一定要调用，不调用是错误的。
    * 如果写作Student stu或者new Student,且后面没有调用构造函数就是错误的，因为类中包含了构造函数，而创建对象时却没有调用。
    */
    // Student *pstu = new Student; 
    Student *pstu = new Student(); 
    pstu -> setname("李华");
    pstu -> setage(16);
    pstu -> setscore(96);
    pstu -> show();
    delete pstu;

    Student *pstu = new Student;
    // Student *pstu = new Student();
    pstu -> Student(NULL, 0, 0.0);
    pstu -> show();
    delete pstu;

    Student *pstu = new Student;
    // Student *pstu = new Student();
    pstu -> Student();
    pstu -> show();
    delete pstu;

    Student *pstu = new Student;
    // Student *pstu = new Student();
    pstu -> setname(NULL);
    pstu -> setage(0);
    pstu -> setscore(0.0);
    pstu -> show();
    delete pstu;

    Student *pstu = new Student;
    // Student *pstu = new Student();
    pstu -> show(); // 成员变量还未初始化,没有初始化值
    delete pstu;

    return 0;
}

