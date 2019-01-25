//============================================================================
// Name        : aa.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#if 0
#include <iostream>
#include <string>
using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	string s(1024, 'x');

	cout << "1. " << s.capacity() << ", " << s.size() << endl;

	s.resize(512);
	cout << "2. " << s.capacity() << ", " << s.size() << endl;

	s.shrink_to_fit();
	cout << "3. " << s.capacity() << ", " << s.size() << endl;
	return 0;
}

#endif

#if 0

// move_if_noexcept example
#include <utility>      // std::move_if_noexcept
#include <iostream>     // std::cout

// function with lvalue and rvalue reference overloads:
template <class T> void overloaded (T&  x) {std::cout << "[lvalue]\n";}
template <class T> void overloaded (T&& x) {std::cout << "[rvalue]\n";}
//template <typename T> void overloaded (T&  x) {std::cout << "[lvalue]\n";}
//template <typename T> void overloaded (T&& x) {std::cout << "[rvalue]\n";}

struct A {   // copyable + moveable (noexcept)
  A() noexcept {}
  A (const A&) noexcept {}
  A (A&&) noexcept {}
};

struct B {   // copyable + moveable (no noexcept)
  B() {}
  B (const B&) {}
  B (B&&) {}
};

struct C {   // moveable only (no noexcept)
  C() {}
  C (C&&) {}
};

int main () {
//  std::cout << "A: "; overloaded (std::move_if_noexcept(A()));
  std::cout << "A: "; overloaded (std::move_if_noexcept(A()));
  std::cout << "B: "; overloaded (std::move_if_noexcept(B()));
  std::cout << "C: "; overloaded (std::move_if_noexcept(C()));
  return 0;
}

#endif

#if 0
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
	int a;
	const double &m = a;

	cout << "a is " << a << ", m is " << m << endl;

	return  EXIT_SUCCESS;
}
#endif


#if 1
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

namespace std
{
	class MyClass
	{
	private:
		int i;
	public:
		explicit MyClass(int _i) : i(_i) {}
		MyClass(const std::MyClass& _mc) : i(_mc.i)
		{
		}
		MyClass(std::MyClass&& _mc) noexcept : i(_mc.i)
		{
		}

		MyClass& operator=(const MyClass& _mc)
		{
			if(this==&_mc) return *this;
			i = _mc.i;
			return *this;
		}

		MyClass& operator=(MyClass&& _mc)
		{
			if(this==&_mc) return *this;
			i = _mc.i;
			return *this;
		}

		friend ostream& operator<<(ostream& os, const MyClass& o)
		{
			os << "(& = " << &o << ", i = " << o.i << ")";
			return os;
		}

		friend const istream& operator>>(istream& is, MyClass& o)
		{
			is >> o.i;
			return is;
		}
	};
};

string getStr()
{
	string s{"112233"};
	cout <<  &s << " in getStr" << endl;
	return s;
}

vector<int> getIntV()
{
	vector<int> t{1,2,3,4,5};
	cout << &t << " in getIntV" << endl;
	return t;
}

MyClass getMyClass()
{
	MyClass mc(10000);
	cout <<  &mc << " in getMyClass" << endl;
	return mc;
}

vector<MyClass> getMyClassV()
{
	vector<MyClass> v;
	for (int i = 0; i < 3; ++i) {
		v.push_back(MyClass(i+10));
	}
	return v;
}

void task01()
{
	while(true)
	{
		vector<int> t = getIntV();
		cout << &t << " in main" << endl;

		MyClass mc= getMyClass();
		cout << &mc << " in main" << endl;

		vector<MyClass> v = getMyClassV();
		for_each(v.begin(), v.end(), [v](const MyClass& o)
				{
			cout << "[" << v.size() << "]" << o;
				});
		cout << " done..." << endl;

		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

int main() {

//	string s = getStr();
//	cout <<  &s << " in main" << endl;

	string line;

	thread t(task01);

	while(true)
	{
		getline(cin, line);
		cout << "Input : " << line << endl;
		cin.clear();
	}

	return 0;
}
#endif
