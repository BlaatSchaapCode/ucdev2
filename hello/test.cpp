extern "C" { void cpptest(void); }

#include <cstdio>

class cppTest{
private:
	int a;
public:
	cppTest(void);
	int getA(void);	
};

cppTest::cppTest(void){
	a = 5;
}

int cppTest::getA(void) {
	return a;
}

cppTest test;
void cpptest(void) {

	printf("a is %d\n", test.getA());
}
