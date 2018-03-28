#ifndef _LIGHT_UNIT_TEST_HEADER_H_
#define _LIGHT_UNIT_TEST_HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

namespace lut {

class Tester 
{
private:
	bool ok_;               //result status
	const char* fname_;     //file name
	int line_;              //line number
	std::stringstream ss_;  //message for display

public:
	Tester(const char* f, int l)
	: ok_(true), fname_(f), line_(l) 
	{
	}

	~Tester() 
	{
		if (!ok_) 
		{
			fprintf(stderr, "%s:%d:%s\n", fname_, line_, ss_.str().c_str());
			exit(1);
		}
	}

	Tester& Is(bool b, const char* msg) 
	{
		if (!b) 
		{
			ss_ << " Assertion failure " << msg;
			ok_ = false;
		}
		return *this;
	}


#define BINARY_OP(name,op)    \
	template <class X, class Y>   \
	Tester& name(const X& x, const Y& y) \
	{\
		if (! (x op y)) \
		{\
			ss_ << " failed: " << x << (" " #op " ") << y;    \
			ok_ = false;  \
		}                     \
		return *this;        \
	}

	BINARY_OP(IsEq, ==)
	BINARY_OP(IsNe, !=)
	BINARY_OP(IsGe, >=)
	BINARY_OP(IsGt, >)
	BINARY_OP(IsLe, <=)
	BINARY_OP(IsLt, <)

	Tester& IsEq (const std::string&x , const std::string &y)
	{
 		if (! x.compare (y))
		 {
			ss_ << " failed: " << x << (" == ") << y;    
			ok_ = false;
 		}

		 return *this;
 	}

#undef BINARY_OP

	template <class V>
	Tester& operator<<(const V& value) 
	{
		if (!ok_) 
		{
		ss_ << " " << value;
		}
		return *this;
	}
};


#define ASSERT_TRUE(c) ::lut::Tester(__FILE__, __LINE__).Is((c), #c)
#define ASSERT_EQ(a,b) ::lut::Tester(__FILE__, __LINE__).IsEq((a),(b))
#define ASSERT_NE(a,b) ::lut::Tester(__FILE__, __LINE__).IsNe((a),(b))
#define ASSERT_GE(a,b) ::lut::Tester(__FILE__, __LINE__).IsGe((a),(b))
#define ASSERT_GT(a,b) ::lut::Tester(__FILE__, __LINE__).IsGt((a),(b))
#define ASSERT_LE(a,b) ::lut::Tester(__FILE__, __LINE__).IsLe((a),(b))
#define ASSERT_LT(a,b) ::lut::Tester(__FILE__, __LINE__).IsLt((a),(b))

#define TEST(base,name)                                                            \
class _test_##base##name                                                           \
{                                                                                  \
public:                                                                            \
       	void _run();                                                               \
	static void Run()                                                          \
	{                                                                          \
		_test_##base##name t;                                              \
		t._run();                                                          \
	}                                                                          \
};                                                                                 \
class _register_##base##name                                                       \
{                                                                                  \
public:                                                                            \
	_register_##base##name()                                                   \
	{                                                                          \
		::lut::RegisterTest(#base, #name, &_test_##base##name::Run);  \
	}                                                                          \
};                                                                                 \
_register_##base##name auto_register_##base##name;                                 \
void _test_##base##name::_run()


bool RegisterTest(const char *base, const char* name, void (*func)());

int RunAllTests();

}//namespace lut

#endif//_LIGHT_UNIT_TEST_HEADER_H_
