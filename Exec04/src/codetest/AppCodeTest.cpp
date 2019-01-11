/*
 * AppCodeTest.cpp
 *
 *  Created on: 2018. 12. 16.
 *      Author: finegs
 */
#include <iostream>
#include <string>

#if 0
struct V {
	virtual void f() {};
	virtual ~V();
};

V::~V() {}

struct A : virtual V {
	virtual ~A();
};
A::~A() {};

struct B : virtual V {
	B(V* v, A* a) {
		dynamic_cast<B*>(v);
		dynamic_cast<B*>(a);
	}
	virtual ~B();
};
B::~B() {};

struct D : A, B {
	D() : B(static_cast<A*>(this), this) {}
};

struct Base {
	virtual ~Base() {}
};

struct Derived: Base
{
	virtual void name() {}
};

int main(int argc, char* argv[])
{
	D d;
	A& a = d;
	D& new_d = dynamic_cast<D&>(a);
	B& new_b = dynamic_cast<B&>(a);

	std::cout << "new_d is " << &new_d << std::endl;
	std::cout << "new_b is " << &new_b << std::endl;

	Base* b1 = new Base;
	if(Derived* d = dynamic_cast<Derived*>(b1))
	{
		std::cout << "downcast from b1 to d successfull\n";
		d->name();
	}

	Base* b2 = new Derived;
	if(Derived* d = dynamic_cast<Derived*>(b2))
	{
		std::cout << "downcast from b2 to d successfull\n";
		d->name();
	}

	delete b1;
	delete b2;
}


#endif

#if 0
#include <cstdlib>

int main(int argc, char* argv[])
{
	std::string str("C++-String");
	str += " C-String";
	std::cout << str << std::endl;
	const char* cString = str.c_str();
	char buffer[10];
	str.copy(buffer, 10);
	str += "works";
	const char* cString2 = cString;
	std::cout << cString2 << std::endl;
	std::string str2(buffer, buffer+10);
	std::cout << str2 << std::endl;

	return EXIT_SUCCESS;
}

#endif


#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstdlib>

double fAvg(int* ar, int size)
{
	double sum;
	sum = 0;
	for (int i = 0; i < size; ++i) sum+=*(ar+i);
	return sum/size;
}

double fDist(int* ar, int size, double avg)
{
	 double sum, diff;
	 sum = diff = 0;

	 for (int i = 0; i < size; ++i) sum+= (avg-*(ar+i))*(avg-*(ar+i));
	 diff  = sum/size;
	 return diff;
}

double fStdDiff(double diff)
{
	return sqrt(diff);
}

int main(int argc, char* argv[])
{
	int size;
	int* ar;
	double avg;
	double dist;
	double stdDiff;
	printf("Enter size : "); fflush(stdout);
	scanf("%d", &size);
	if(!(ar = (int*)malloc(sizeof(int)*size)))
	{
		fprintf(stderr, "Fail to malloc : size=%d", size);
		return EXIT_FAILURE;
	}

	for (int i = 0; i < size; ++i) {
		scanf("%d", ar+i);
	}

	printf("ar=["); for(int i = 0;i<size;i++) printf("%d%s", *(ar+i), i<size-1?", " : ""); printf("]\n");

	printf("avg=%lf\n", avg = fAvg(ar, size));
	printf("dist=%lf\n", dist = fDist(ar, size, avg));
	printf("stdDiff=%lf\n", stdDiff = fStdDiff(dist));

	return EXIT_SUCCESS;
}
