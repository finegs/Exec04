/*
 * AppCodeTest.cpp
 *
 *  Created on: 2018. 12. 16.
 *      Author: finegs
 */
#include <iostream>

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


