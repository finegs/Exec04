//============================================================================
// Name        : CodeTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <string>
#include <functional>
using namespace std;


class A {
public:
	A(std::string _name) : name(_name)
	{
		cout << "## 4 ##" <<  "A() : " << &_name << endl;
		cout << "## 5 ##" <<  "A() : " << &name << endl;
	}

	A(const A& a) : name(a.name) {}
	A(A&& rhs) noexcept : name(std::move(rhs.name)) {}

	A& operator=(const A& a) {
		this->name = std::string(a.name);
		return *this;
	}

	A& operator=(A&& a) noexcept
	{
		this->name = std::move(a.name);
		return *this;
	}

	friend ostream& operator<<(ostream& os, const A& a)
	{
		std::cout << a.name << std::endl;
		return os;
	}

	std::string name;

};


namespace std
{
	template<>
	struct hash<A>
	{
		std::size_t
		operator()(const A& a) const
		{
			return std::hash<std::string>()(a.name);
		}
	};
}

A f1(std::string s)
{
	cout << "## 3 ##" <<  &s << "=" << s <<  endl;
	A a(s);
	cout << "## 6 ##" <<  " &a=" << &a <<", &a.name=" << &a.name << endl; // prints !!!Hello World!!!
	return a;
}
A* f2(std::string s)
{
	cout << "## 3 ##" <<  &s << "=" << s <<  endl;
	A* a = new A(s);
	cout << "## 6 ##" <<  " &a=" << &a <<", &a.name=" << a->name << endl; // prints !!!Hello World!!!
	return a;
}

struct AHasher
{
  size_t
  operator()(const A & obj) const
  {
    return std::hash<std::string>()(obj.name);
  }
};

// Custom comparator that compares the string objects by length
struct AComparator
{
  bool
  operator()(const A & obj1, const A & obj2) const
  {
    if (obj1.name == obj2.name)
      return true;
    return false;
  }
};


std::unordered_map<std::string, A, AHasher, AComparator> m1;
std::unordered_map<std::string, int*> m2;

int main(int argc, char* argv[]) {

//	std::string s("aa");
//	cout << "## 1 ##" << &s << endl;
//	s+=std::to_string(1);
//	cout << "## 2 ##" <<  &s << endl;
//
//	A a = f1(s);
//
//	cout << "## 7 ##" <<  " &a=" << &a <<", &a.name=" << &a.name << endl; // prints !!!Hello World!!!

//	const char* aa = "bbb";
//	const char* bb = std::move(aa);
//	const char* cc = aa;
//	const char* dd = aa;
//
//	cout << "aa=" << aa << endl;
//	cout << "bb=" << bb << endl;
//	cout << "cc=" << cc << endl;
//	cout << "dd=" << dd << endl;
//
////	cout << "sizeof(char*)=" << sizeof(char*) << endl;
////	cout.flush();
//
//	cout << "&aa=" << &aa << ", aa=" << aa << endl;
//	cout << "&bb=" << &bb << ", bb=" << bb << endl;
//	cout << "&cc=" << &cc << ", cc=" << cc << endl;
//	cout << "&dd=" << &dd << ", dd=" << dd << endl;


	string s1("aaa");
	string s2(std::move(s1));
	string s3(s1);

	cout << "&s1=" << &s1 << ", s1=" << s1 << endl;
	cout << "&s2=" << &s2 << ", s2=" << s2 << endl;
	cout << "&s3=" << &s3 << ", s3=" << s3 << endl;

	for (int i = 0; i < 1000000; ++i) {
		A a = f1("aaaabbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccccccccccccc");
		m1.insert(std::make_pair(a.name, a));
		m1.erase(a.name);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
