//============================================================================
// Name        : CodeTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#if 0
#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <string>
#include <functional>
#include <cstdlib>
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


//std::unordered_map<std::string, A, AHasher, AComparator> m1;
std::unordered_map<std::string, A> m1;
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

#if 1
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
#endif

#if 0
	using namespace std;
	string prompt("Enter a line of text: "),    // Global
			line( 50, '*');                      // strings
	string text;                      // Empty string
	cout << line << endl << prompt << endl;
	getline( cin, text);        // Reads a line of text
	cout << line << endl
			<< "Your text is " << text.size()
			<< " characters long!" << endl;
	// Two new strings:
	string copy(text),            // a copy and the
			start(text,0,10);      // first 10 characters
	// starting with
	// position 0.
	cout << "Your text:\n" << copy << endl;
	text = "1234567890";          // Assignment
	cout << line << endl
			<< "The first 10 characters:\n" << start << endl
			<< text << endl;
#endif
	return EXIT_SUCCESS;
}

#endif

#if 1

#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>

class MyString
{
private:
	std::string name;
	int iValue;
	std::string sValue;

public:
	MyString() : name(), iValue(0), sValue() {}
	explicit MyString(std::string str) : name(str), iValue(0), sValue() {}
	MyString(const MyString& s) : name(s.name), iValue(s.iValue), sValue(s.sValue) {}
	MyString(MyString&& s) : name(std::move(s.name)), iValue(s.iValue), sValue(std::move(s.sValue)) {}
	MyString& operator=(const MyString& s)
	{
		if(this==&s) return*this;
		name = std::string(s.name);
		iValue = s.iValue;
		sValue = std::string(s.sValue);
		return *this;
	}
	MyString& operator=(MyString&& s)
	{
		if(this==&s) return s;
		name = std::move(s.name);
		iValue = s.iValue;
		sValue = std::move(s.sValue);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const MyString& s)
	{
		os << s.name << ", " << s.iValue << ", " << s.sValue;
		return os;
	}

	const std::string& getName() const
	{
		return name;
	}
};

namespace std
{
	template<>
	struct hash<MyString>
	{
		std::size_t
		operator()(const MyString& s) const
		{
			return std::hash<std::string>()(s.getName());
		}
	};

};
std::unordered_map<std::string, MyString> map;

using namespace std;
int main(int argc, char* argv[])
{

	if(argc != 3)
	{
		cerr << "Usage : " << argv[0] << " {cnt} {dela}";
		return EXIT_FAILURE;
	}
	int cnt = std::atoi(argv[1]);
	int delay = std::atoi(argv[2]);
	int i;

	for (i = 0; i < cnt; ++i) {
		MyString s("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		map.insert(std::make_pair(s.getName(), s));

		std::this_thread::sleep_for(std::chrono::milliseconds(delay));

		MyString ss = map[s.getName()];

		map.erase(ss.getName());

		std::cout << std::setw(-10) << i << " : " << ss << std::endl;
	}

	return EXIT_SUCCESS;

}

#endif

