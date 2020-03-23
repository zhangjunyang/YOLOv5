
#ifndef STUDENT_H_
#define STUDENT_H_

#ifndef __cplusplus
#error ERROR: This file requires C++ compilation. Use a .cpp suffix
#endif

#include <Schoolstub.h>
#include <ncore/NCSyncObj.h>
#include <ncore/NCAutoSync.h>
#include <iostream>

namespace nutshell {

//类的继承与声明
class Student : public Schoolstub { 

public:
	Student();
	virtual ~Student();
	static Student* instance();
	static void destory();

	void setname(char *name);
	void setage(int age);
	void setscore(float score);
	void show();

private:
    char *m_name;
    int m_age;
    float m_score;

    static NCSyncObj sInstanceSync;
	static Student* sInstance;
	    
}; 
} // namespace nutshell
#endif // STUDENT_H_