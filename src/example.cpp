#include "serialize.h"
#include <iostream>

class MyTest :public Serializable
{
public:
	std::string m_name;
	int m_age;
	float m_salary;

public:
	MyTest()
	{
		m_name.clear();
		m_age=0;
		m_salary=0.;
	}

	MyTest(const char *name,const int age,const float salary)
	{
		m_name.assign(name);
		m_age=age;
		m_salary=salary;
	}

	MyTest(const MyTest& other)
	{
		m_name=other.m_name;
		m_age=other.m_age;
		m_salary=other.m_salary;
	}

	~MyTest()
	{
		m_name.clear();
	}

	MyTest& operator=(const MyTest& other)
	{
		if(this!=&other)
		{
			m_name=other.m_name;
			m_age=other.m_age;
			m_salary=other.m_salary;
		}

		return *this;
	}

	virtual std::string serialize()
	{
		OutStream os;
		os<<m_name<<m_age<<m_salary;
		return os.str();
	}

	virtual int deserialize(std::string &str)
	{
		InStream is(str);
		is>>m_name>>m_age>>m_salary;
		return is.size();
	}

	void display()
	{
		std::cout<<m_name<<","<<m_age<<","<<m_salary<<std::endl;
	}
}; 

OutStream& operator<<(OutStream& os, MyTest &obj)
{
	os<<obj.m_name<<obj.m_age<<obj.m_salary;
	return os;	
}

InStream& operator>>(InStream& is, MyTest &obj)
{
	is>>obj.m_name>>obj.m_age>>obj.m_salary;
	return is;	
}

int main(int argc,char *argv[])
{
	short a=1;
	int b=2;
	long c=3;
	float d=4;
	long long e=5;
	std::string f="hello";
	MyTest t("zhang",23,3200.5);
	MyTest m("lucy",44,2332.2);
	std::vector<MyTest> n;
	n.push_back(MyTest("aaa",111,222));
	n.push_back(MyTest("bbb",222,333));
	n.push_back(MyTest("ccc",333,444));

	OutStream os;
	os<<a<<b<<c<<d<<e<<f<<t<<m<<n;
	std::string serializestr=os.str();
	

	short a1;
	int b1;
	long c1;
	float d1;
	long long e1;
	std::string f1;
	MyTest t1;
	MyTest m1;
	std::vector<MyTest> n1;
	InStream is(serializestr);
	
	is>>a1>>b1>>c1>>d1>>e1>>f1>>t1>>m1>>n1;
	std::cout<<"a="<<a<<",a1="<<a1<<std::endl;
	std::cout<<"b="<<b<<",b1="<<b1<<std::endl;
	std::cout<<"c="<<c<<",c1="<<c1<<std::endl;
	std::cout<<"d="<<d<<",d1="<<d1<<std::endl;
	std::cout<<"e="<<e<<",e1="<<e1<<std::endl;
	std::cout<<"f="<<f<<",f1="<<f1<<std::endl;
	t1.display();
	m1.display();
	for(std::vector<MyTest>::iterator it=n1.begin();it!=n1.end();++it)
		(*it).display();
	
	return 0;
}
