#include "lut.h"

#include <string>
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace lut {

struct TestContext 
{
	const char* base;
	const char* name;
	void (*func)();
};

static std::vector<lut::TestContext>* tests;

bool RegisterTest(const char* base, const char* name, void (*func)()) 
{
	if (tests == NULL) 
	{
		tests = new std::vector<TestContext>;
	}

	TestContext t;
	t.base = base;
	t.name = name;
	t.func = func;

	tests->push_back(t);

	return true;
}

int RunAllTests() 
{
	int num = 0;

	for (size_t i = 0; i < tests->size(); ++i) 
	{
		const TestContext& t = (*tests)[i];
		std::string name = t.base;
		name.push_back('.');
		name.append(t.name);
	
		fprintf(stderr, "==== Test %s.%s\n", t.base, t.name);
		(*t.func)();
		++num;
	}
	
	fprintf(stderr, "==== PASSED %d tests\n", num);
	return 0;
}

}  // namespace lut
