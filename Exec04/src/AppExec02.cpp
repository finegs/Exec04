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


#if 0

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


#if 0

#include <iostream>
#include <cstdlib>

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>

#define u_int32 UINT32  // Unix uses u_int32

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")


int                  /* OUT: whatever setsockopt() returns */
join_source_group(int sd, u_int32 grpaddr,
   u_int32 srcaddr, u_int32 iaddr)
{
   struct ip_mreq_source imr;

   imr.imr_multiaddr.s_addr  = grpaddr;
   imr.imr_sourceaddr.s_addr = srcaddr;
   imr.imr_interface.s_addr  = iaddr;
   return setsockopt(sd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char *) &imr, sizeof(imr));
}

int
leave_source_group(int sd, u_int32 grpaddr,
   u_int32 srcaddr, u_int32 iaddr)
{
   struct ip_mreq_source imr;

   imr.imr_multiaddr.s_addr  = grpaddr;
   imr.imr_sourceaddr.s_addr = srcaddr;
   imr.imr_interface.s_addr  = iaddr;
   return setsockopt(sd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (char *) &imr, sizeof(imr));
}

int main(int argc, char* argv[]) {




	return EXIT_SUCCESS;
}

#endif


#if 0

#include <iostream>
#include <cstdlib>
#include <atomic>
#include <vector>
#include <mutex>

using namespace std;

class Widget
{
public:
	Widget() = default;
	Widget(int _x, bool _b) : x(_x), b(_b) {};
	Widget(int _x, double _zz) : x(_x), zz(_zz) {};
//	Widget(std::initializer_list<bool> il);


	Widget(const Widget& o) : x(o.x), y(o.y), z(o.z), b(o.b) {};
	Widget(Widget&& o) : x(o.x), y(o.y), z(o.z), b(o.b) {};

	const Widget& operator=(const Widget& o)
	{	if(this==&o) return o;
		x = o.x;
		y = o.y;
		z = o.z;
		b = o.b;
		return o;
	}

	Widget& operator=(Widget& o)
	{
		if(this==&o) return o;
		x = o.x;
		y = o.y;
		z = o.z;
		b = o.b;
		return *this;
	}

	friend ostream& operator<<(ostream& os, const Widget& o)
	{
		os << "x=" << o.x
				<< ", y=" << o.y
				<< ", z=" << o.z
				<< ", b=" << o.b
			<< endl;
		return os;
	}

	friend istream& operator>>(istream& is, Widget& o)
	{
		is >> o.x;
		is >> o.y;
		is >> o.z;
		return is;
	}

public:
	using RootsType = std::vector<double>;

	RootsType roots() const
	{
		RootsType rootVals;
		std::lock_guard<std::mutex> g(m);
		if(!rootsAreValid)
		{
  			rootsAreValid = true;
		}
		return rootVals;
	}
private:
	mutable std::mutex m;
	mutable bool rootsAreValid{false};
	int x{0};
	int y = 0;
//	int z(0);
	int z{0};
	bool b{false};
	double zz{0.0};

};

std::atomic<int> ai1{ 0 };
std::atomic<int> ai2( 0 );
//std::atomic<int> ai3 = 0;

double x,y,z;

int sum1{x+y+z};
int sum2(x+y+z);

int sum3 = x+y+z;

Widget w0();
Widget w1{};

int main(int argc, char* argv[])
{
	Widget w2{1, 1.5};

	cout << w2 << endl;

	system("pause");

	return EXIT_SUCCESS;
}
#endif


#if 0

#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
	int aList[5][5] = {0};
	int x  = -1, y = 0, nCounter = 0;
	int i = 0, j = 0, nLength = 9, nDirection = 1;

	for(nLength = 9;nLength>0;nLength -= 2)
	{
		for(i = 0;i<nLength;++i)
		{
			if(i<nLength/2+1) x+= nDirection;
			else			  y+= nDirection;
			aList[y][x] = ++nCounter;
		}
		nDirection = -nDirection;
	}

	for(i = 0;i<5;++i)
	{
		for (j = 0; j < 5; ++j) {
			printf("%d\t", aList[i][j]);
		}
		putchar('\n');
	}

	return EXIT_SUCCESS;
}

#endif

#if 0

#include <cstdio>
#include <cstdlib>

int add(int a, int b)
{
	return a + b;
}

__inline int addInline(int a, int b)
{
	return a + b;
}

int main(int argc, char* argv[])
{

	printf("%d\n", add(1,2));
	printf("%d\n", addInline(1,2));


	return EXIT_SUCCESS;
}


#endif


#if 0

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

void mysort(vector<int>& arr);

string toString(const vector<int>& arr);

int main(int argc, char* argv[])
{

	while(true)
	{
		int n = rand() % 100 + 1;
		vector<int> input(n);

		for(int i = 0;i<n;i++)
			input[i] = rand();

		vector<int> mySorted = input;
		mysort(mySorted);
		vector<int> reference = input;
		sort(reference.begin(), reference.end());

		if(mySorted != reference)
		{
			cout << "Mismatch!" << endl;
			cout << "Input\t\t: " << toString(input) << endl;
			cout << "Expected\t: " << toString(reference) << endl;
			cout << "Got\t\t: " << toString(mySorted) << endl;
		}
		else {
			cout << "mysort and sort is same " << endl;
		}

		system("pause");
	}

	return EXIT_SUCCESS;

}

void mysort(vector<int>& arr)
{
	size_t i,j, mi;
	for(i = 0;i<arr.size();i++)
	{
		mi = i;
		for (j = i+1; j < arr.size(); j++) {
			if(arr[j]<arr[mi])
			{
				mi = j;
			}
		}
		if(mi != i) {
			int t = arr[i];
			arr[i]= arr[mi];
			arr[mi] = t;
		}
	}
}

string toString(const vector<int>& arr)
{
	ostringstream ss, header, t;
	long long sum;
	double min,max;
//	string s(arr.begin(), arr.end(), comma);
	min = 0xFFFFFF, max = -1, sum = 0;
	for_each(arr.begin(), arr.end(), [&](int n)
			{
				min = n < min ? n : min;
				max = n > max ? n : max;
				sum += n;
				ss << n << ", ";
			});
	header << "min=" << min << ", "
			<< "max=" << max << ", "
			<< "avg=" << (sum/arr.size());
	t << header.str() << ", " << ss.str();
	return t.str();
}

#endif


#if 1

#include <iostream>
#include <cstdlib>
#include <c++/algorithm>
#include <c++/utility>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>
#include <thread>

using namespace std;

int main(int argc, char* argv[])
{
	using stringmap = map<std::string, std::string>;

	int cnt;
	stringmap m;
	fstream fs;
	cnt = 0;
	fs.open("input.txt");
//	if(!fs.is_open())
	if(0 != (fs.rdstate() & std::fstream::failbit))
	{
		cout << "Fail to open file : "<< "input.txt" << "(" << fs.fail() << ", " << fs.bad() << ", " << fs.eof() << ")"  <<endl;
		return EXIT_FAILURE;
	}

	while(!fs.eof())
	{
		string k,v;
		fs >> k >> v;
		cout << "[" << cnt << "] : " << "(" << k << ", " << v << ")" << endl;
		m.emplace(k, v);
		cnt++;
	}
	fs.close();
	m.emplace(string("Cnt"), to_string(cnt));

	for_each(m.begin(), m.end(), [&](auto& i)
	{
		cout << "[" << cnt << "] : " << "(" << i.first << ", " << i.second << ")" << endl;
	});

	return EXIT_SUCCESS;
}

#endif
