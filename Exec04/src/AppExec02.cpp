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


#if 0
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
	protected:
		int i;
	public:
		explicit MyClass(int _i) : i(_i) {}
		MyClass(const std::MyClass& _mc) : i(_mc.i)
		{
		}
		MyClass(std::MyClass&& _mc) noexcept : i(_mc.i)
		{
		}

		virtual ~MyClass() {};

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

		virtual int pow() { return i*i;}
	};

	class MyClass2 : virtual MyClass
	{
	public:
		explicit MyClass2(int _i) : MyClass{_i}, ii(_i) {}
		MyClass2(const std::MyClass2& o) : MyClass(o), ii{o.ii} {}
		MyClass2(std::MyClass2&& o) : MyClass(o), ii(o.ii) {}
		~MyClass2() {}

		friend ostream& operator<<(ostream& os, const MyClass2& obj)
		{
			os << (const MyClass&)obj << ", ii=" << obj.ii;
			return os;
		}

		friend const istream& operator>>(istream& is, MyClass2& o)
		{
			is >> (MyClass&)o;
			is >> o.ii;
			return is;
		}

		int pow()
		{
			return MyClass::i*MyClass::i;
		}
	private:
		int ii;
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

vector<MyClass> getMyClassV(int cnt = 3)
{
	vector<MyClass> v;
	for (int i = 0; i < cnt; ++i) {
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

		vector<MyClass> v = getMyClassV(10000000);
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


#if 0

#include <vector>
#include <iostream>
#include <string>

#include <windows.h>

namespace {

const int PUSH_BACK_COUNT_1000 = 2000;

}

using namespace std;

struct JJ
{
    int c[50];
    JJ(int i) { c[5] = 3; c[29] = 4; c[30] = i; c[49] = c[5]; }
};

void fill_direct_stack()
{
    vector<JJ> vec;
	for (int i = 0; i < PUSH_BACK_COUNT_1000; ++i)
        vec.push_back(i);
}

void fill_direct_heap()
{
    vector<JJ>* pVec = new vector<JJ>();
	for (int i = 0; i < PUSH_BACK_COUNT_1000; ++i)
        pVec->push_back(i);
    delete pVec;
}

CRITICAL_SECTION cs_print;

void print(string msg, DWORD val)
{
    EnterCriticalSection(&cs_print);
    cout << msg << val << endl;
    LeaveCriticalSection(&cs_print);
}

DWORD __stdcall threadEntry(void*)
{
    DWORD ticks1,ticks2;

    ticks1 = GetTickCount();
    for (int i=0; i<10000; ++i)
        fill_direct_stack();
    ticks2 = GetTickCount();
    print("ticks (stack): ", ticks2 - ticks1);

    ticks1 = GetTickCount();
    for (int i=0; i<10000; ++i)
        fill_direct_heap();
    ticks2 = GetTickCount();
    print("ticks (heap): ", ticks2 - ticks1);

    return 0;
}

int main()
{
    cout<<"hi"<<endl;

    InitializeCriticalSection(&cs_print);

#define N_THREADS 5

    HANDLE thr[N_THREADS];
    for (int i=0; i<N_THREADS; ++i)
        thr[i] = CreateThread(NULL, 0, &threadEntry, NULL, 0, NULL);

    for (int i=0; i<N_THREADS; ++i)
        WaitForSingleObject(thr[i], INFINITE);

    DeleteCriticalSection(&cs_print);

    system("pause");
}

#endif


#if 1

// declval example
#include <utility>      // std::declval
#include <iostream>     // std::cout

struct A {              // abstract class
  virtual int value() = 0;
  virtual ~A() {}
};

class B : public A {    // class with specific constructor
  int val_;
public:
  B(int i,int j):val_(i*j){}
  ~B() {}
  int value() {return val_;}
};

int main() {
  decltype(std::declval<A>().value()) a;  // int a
  decltype(std::declval<B>().value()) b;  // int b
  decltype(B(0,0).value()) c;   // same as above (known constructor)
  a = b = c = B(10,2).value();
  std::cout << a << '\n';
  return 0;
}

#endif
