
#define _MY_DEF_01 0
#define _MY_DEF_02 0
#define _MY_DEF_03 0
#define _MY_DEF_04 0
#define _MY_DEF_05 0
#define _MY_DEF_06 0
#define _MY_DEF_07 0
#define _MY_DEF_08 0
#define _MY_DEF_09 0
#define _MY_DEF_10 0
#define _MY_DEF_11 1

#if _MY_DEF_01

#include <stdio.h>
#include <iostream>

#ifdef _DEBUG
setbuff(stdout, NULL);
setbuff(stderr, NULL);
#endif

int main()
{
	using namespace std;

	int examples;

	cout << "Enter examples" << endl;

	cin >> examples;

	cout << examples << endl;

	examples = 25;

	cout << "I have ";
	cout << examples;
	cout << " examples. ";
	cout << endl;
	examples = examples -1;
	cout << "I have " << examples << " examples." << endl;

	return 0;
}


#endif

#if _MY_DEF_02

#include <iostream>
#include <queue>
#include <string>

int main()
{
	std::priority_queue<std::string> mypq;

	std::string line;

	std::cin >> line;

	mypq.emplace(line);

	std::cout << line << std::endl;


	mypq.emplace("orange");
	mypq.emplace("strawberry");
	mypq.emplace("apple");
	mypq.emplace("pear");

	std::cout << "mypq contains: ";
	while(!mypq.empty())
	{
		std::cout << ' ' << mypq.top();
		mypq.pop();
	}
	std::cout << '\n';

	return 0;
}

#endif

#if _MY_DEF_03
#include <chrono>
#include <iostream>

#include "frames.h"

using namespace std;

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

void print_ms(const TimePoint& point)
{
    using Ms = std::chrono::milliseconds;
    const Clock::duration since_epoch = point.time_since_epoch();
    std::cout << std::chrono::duration_cast<Ms>(since_epoch).count() << " ms\n";
}

#if _MY_DEF_04
int main()
{
    const TimePoint default_value = TimePoint(); // (1)
    print_ms(default_value); // 0 ms

    const Clock::duration duration_4_seconds = std::chrono::seconds(4);
    const TimePoint time_point_4_seconds(duration_4_seconds); // (2)
      // 4 seconds from start of epoch
    print_ms(time_point_4_seconds); // 4000 ms

    const TimePoint time_point_now = Clock::now(); // (3)
    print_ms(time_point_now); // 43098276 ms
}
#endif

#endif

#if _MY_DEF_05
int main() {
    int a = 0;

    List l;

    cout << "Enter New Frame Number : "; cout.flush();
    cin >> a;

    freeFrame(&l, a);

    a = -1;

    a = allocFrame(&l);

    cout << "Frame Number : " << a; cout.flush();
}
#endif

#if _MY_DEF_06

#include <iostream>
#include <utility>
#include <set>

using namespace std;

template <class _A, class _B, class _Compare=less<_A>>
class MMap : public set<pair<_A, _B>, _Compare>
{
public:
	MMap():set<pair<_A, _B>, _Compare>() {};
	~MMap(){};
};

template<typename InPair>
struct MMapComp {
	bool operator()(InPair a, InPair b) {
		if(a.first == b.first) return a.second > b.second;
		else return a.first < b.first;
	}
};

int main(int argc, char **argv) {

	MMap<char, int, MMapComp<pair<char, int>>> test;

	test.insert(make_pair('D', 1));
	test.insert(make_pair('D', 2));
	test.insert(make_pair('B', 3));
	test.insert(make_pair('A', 4));

	for(MMap<char, int>::iterator it = test.begin(); it != test.end(); it++ ) {
		cout << (*it).first << "\t" << (*it).second << endl;
	}

	return EXIT_SUCCESS;
}

#endif


#if _MY_DEF_07

#include <utility>
#include <iostream>


class Test {
  public:
  Test() {
      std::cout << "ctor" << std::endl;
  }
  Test(const Test&) {
      std::cout << "copy ctor" << std::endl;
  }
  Test(Test&&) {
      std::cout << "move ctor" << std::endl;
  }
};

void func(Test const&)
{
    std::cout << "requires lvalue" << std::endl;
}

void func(Test&&)
{
    std::cout << "requires rvalue" << std::endl;
}

template<typename Arg>
void pass(Arg&& arg) {
    // use arg here
    func(std::forward<Arg>(arg));
    return;
}

template<typename Arg, typename ...Args>
void pass(Arg&& arg, Args&&... args)
{
    // use arg here
    return pass(std::forward<Args>(args)...);
}

int main(int, char**)
{
    pass(std::move<Test>(Test()));
    return 0;
}

#endif


#if _MY_DEF_08

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

struct Foo {
	// Foo
	Foo() = default;
	Foo(int i) : m_i(i), m_b(false), m_f(0.f) {}
	Foo(int i, bool b, float f) : m_i(i), m_b(b), m_f(f) {}
	Foo(int i, bool b, vector<int> const& v) : m_i(i), m_b(b), m_v(v) {}
	Foo(int i, bool b, vector<int> && v) : m_i(i), m_b(b), m_v(move(v)) {}

	int m_i;
	bool m_b;
	float m_f;
	vector<int> m_v;
};

struct Bar {

	// variadic template
	template <typename ... Args>
	void AddFoo(Args&& ... args) { // forwarding reference (universal reference)
		v.emplace_back(forward<Args>(args)...);
	}

//	void addFoo(Foo const& foo) {
//		v.emplace_back(foo);
//	}
private:
	vector<Foo> v;

};

int main(int argc, char* argv[]) {

//	vector<Foo> v1;
//	{
//
//		Foo f1(1, true, 2.f);
//		v1.push_back(f1); // make a copy for foo
//		v1.push_back(move(f1)); // move foo
//		v1.push_back(Foo(1, true, 3.f));    // move foo
//
//		v1.emplace_back(2, true, 4.f);
//	}

	Bar b;
	b.AddFoo(1, true, 2.f);;
	b.AddFoo(2.f);

	vector<int> v = {1,2,3};

//	b.AddFoo(1,true, v);
	b.AddFoo(1,true, move(v));

	return EXIT_SUCCESS;
}


#endif


#if _MY_DEF_09

#include <iostream>

enum { kIsSmaller, kIsLarger, kIsSame };

class Data { //from w  ww  .  j a  va2 s  .co  m
public:
    Data(int newVal):value(newVal) {}
    ~Data() {}
    int compare(const Data&);
    void show() { std::cout << value << "\n"; }
private:
    int value;
};

int Data::compare(const Data& otherData)
{
    if (value < otherData.value)
            return kIsSmaller;
    if (value > otherData.value)
            return kIsLarger;
    else
        return kIsSame;
}

// forward class declarations
class Node;
class HeadNode;
class TailNode;
class InternalNode;

class Node {
public:
    Node() {}
    virtual ~Node() {}
    virtual Node* insert(Data* data) = 0;
    virtual void show() = 0;
private:
};

// A node to hold objects of type Data.
class InternalNode : public Node
{
public:
    InternalNode(Data* data, Node* next);
    virtual ~InternalNode() { delete next; delete data; }
    virtual Node* insert(Data* data);
    virtual void show()
          { data->show(); next->show(); } // delegate!

private:
    Data* data;  // the data itself
    Node* next;  // points to next node in the linked list
};

// a simple constructor
InternalNode::InternalNode(Data* newData, Node* newNext):
data(newData), next(newNext)
{
}

Node* InternalNode::insert(Data* otherData) {
    // is the new object bigger or smaller than me?
    int result = data->compare(*otherData);

    switch (result)
    {
    case kIsSame:         // fall through
    case kIsLarger:    // new data comes before me
          {
              InternalNode* dataNode =
                  new InternalNode(otherData, this);
              return dataNode;
          }
     case kIsSmaller:
         next = next->insert(otherData);
          return this;
    }
    return this; // appease the compiler
}

// The last node in the list
class TailNode : public Node
{
public:
    TailNode() {}
    virtual ~TailNode() {}
    virtual Node* insert(Data* data);
    virtual void show() {}
private:
};

Node* TailNode::insert(Data* data)
{
    InternalNode* dataNode = new InternalNode(data, this);
    return dataNode;
}

class HeadNode : public Node {
public:
    HeadNode();
    virtual ~HeadNode() { delete next; }
    virtual Node* insert(Data* data);
    virtual void show() { next->show(); }
private:
    Node* next;
};

// The first node in the list, which creates the tail
HeadNode::HeadNode()
{
    next = new TailNode;
}
Node* HeadNode::insert(Data* data) {
    next = next->insert(data);
    return this;
}
class LinkedList {
public:
    LinkedList();
    ~LinkedList() { delete head; }
    void insert(Data* data);
    void showAll() { head->show(); }
private:
    HeadNode* head;
};

LinkedList::LinkedList() {
    head = new HeadNode;
}

// Delegate to a head node
void LinkedList::insert(Data* pData)
{
    head->insert(pData);
}

// put all these classes to the test
int main()
{
    Data* pData;
    int val;
    LinkedList ll;

    // store user values in a linked list
    while (true)
    {
         std::cout << "What value (0 to stop)? "; std::cout.flush();
         std::cin >> val;
         if (!val)
              break;
         pData = new Data(val);
         ll.insert(pData);
   }

   // display the list
   ll.showAll();
   std::cout.flush();
   return 0;
}

#endif

#if _MY_DEF_10

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class LinkedList {
private:
	struct nodeType {
	public:
		nodeType() : info(""), link(nullptr) {};
		string info;
		nodeType* link;
	};

	nodeType* first, *last, *newNode;
	int count1;

public:

	LinkedList() = default;

	void addElement(string name) {
		newNode = new nodeType;
		newNode->info = name;
		newNode->link = nullptr;
		if(first == nullptr) {
			first = newNode;
			last = newNode;
		}
		else {
			last->link = newNode;
			last = newNode;
		}
		count1++;
	}


	void removeElement(string name) {
		nodeType *current, *bcurrent;

		if(first == nullptr) {
			cout << "Cannot delete from an empty list. " << endl;
		}
		else {
			if(first->info == name) {
				current = first;
				first = first->link;
				if(first == nullptr) {
					last = nullptr;
				}
				delete current;
			}
			else {
				current = first;
				while(current != nullptr) {
					if(current->info == name) {
						bcurrent = current;
						current = current->link;
						if(current==nullptr) {
							last = nullptr;
						}
						delete bcurrent;
					}
					else {
						current = current->link;
					}
				}
			}
		}
	}

	bool isEmptyList() {
		return (first == nullptr);
	}

	int size() const {
		nodeType *current;
		current = first;
		int count = 0;
		while(current != nullptr) {
			current = current->link;
			count++;
		}
		return count;
	}

	void printList(ostream& os) {
		nodeType *current;
		current = first;
		while(current != nullptr) {
			os << current->info << ", ";
			current = current->link;
		}
	}

void sort() {
	nodeType *current, *bcurrent;
	current = first;
	bcurrent = first->link;

	for(int i = count1 -1 ; i >= 0;i--) {
		for(int j = 0; j < count1-1;j++) {
			if(current->info > bcurrent->info) {
				swap(current->info, bcurrent->info);
			}
			current = bcurrent;
			bcurrent = bcurrent->link;
		}
	}
}

void reverse() {
	nodeType *current, *next, *prev;
	current = first;
	next = nullptr;
	prev = nullptr;
	while(current != nullptr) {
		next = current->link;
		current->link = prev;
		prev = current;
		current = next;
	}
}

};

int main(int argc, char* argv[]) {

	string str1; //declaring a variable to store the string
	LinkedList b; // declaring my class object

	if (b.isEmptyList() == true) { //checking if the list is empty
		cout << "The list is empty" << endl;
	}
	do { // do while loop to prompt the user to enter a word and do the functions inside the class
		cout << "Enter a word('exit' or 'quit' to quit): ";
		cin >> str1;
		if (str1 == "exit" || str1 == "quit") { // option to quit the loop
			break;
		}
		else if (str1 == "-1") { // option to quit the loop
			break;
		}
		else {
			b.addElement(str1);
			cout << endl;
			b.printList(cout);
			cout << endl << "The list size is ";
			cout << b.size() << endl;
		}
	} while (str1 != "1"); // option to continue the loop
	cout << "Sorted list: ";
	b.sort();
	cout << endl;
	b.printList(cout);
	cout << endl;
	system("pause");
	return 0;
}

#endif

#if _MY_DEF_11

#include <iostream>

template<typename T>
class List;

template <class TNode>
class Iterator
{
	friend class List<typename TNode::value_type>;
	TNode* pNode;

	Iterator(TNode* _pNode) : pNode(_pNode) {}
public:
	void operator++() { pNode = pNode->_next; }
	void operator++(int) { pNode = pNode->_next; }
	bool operator!=(Iterator<TNode> rval) { return !(pNode == rval.pNode); }
	bool operator==(Iterator<TNode> rval) { return (pNode == rval.pNode); }
	typename TNode::value_type operator*() { return pNode->_data; }
	Iterator<TNode> operator+(int _i) {
		Iterator<TNode> iter = *this;
		for(int i = 0;i < _i;++i) {
			if(iter.pNode) ++iter;
			else break;
		}
		return iter;
	}

};

template <typename T>
class Node {
	friend class List<T>;
	friend class Iterator<Node<T>>;
	Node() : _next(0) {}
	Node(T data) : _data(data), _next(0) {}
	Node(T data, Node<T>* next) : _data(data), _next(next) {}
	Node(Node<T>* next) : _next(next) {}

	T _data;
	Node<T>* _next;
public:
	typedef T value_type;
};

template <typename T>
class List
{
	Node<T>* first;
public:
	typedef Iterator<Node<T>> iterator;
	typedef T				  value_type;

	List() : first(0) {}
	~List()
	{
		if(first) {
			Node<T> *iter = first;
			while(iter != 0) {
				Node<T>* tmp = iter;
				iter = iter->_next;
				delete tmp;
			}
		}
	}

	void push_back(T data) {
		if(first) {
			Node<T> *iter = first;
			for(;iter->_next != 0;iter=iter->_next);
			iter->_next=new Node<T>(data);
		}
		else
			first =new Node<T>(data);
	}

	void push_front(T data) {
		if(first) {
			Node<T>* tmp = new Node<T>(data);
			tmp -> _next = first;
			first = tmp;
		}
		else {
			first = new Node<T>(data);
		}
	}

	iterator begin(){ return iterator(first); }
	iterator end(){ return iterator(0); }

	bool erase(iterator& _iNode) //True for success, vice versa
	{
		/* This is rather inneffecient. Maybe a better way to do this? */
		/* Even then, it's *still* more effecient than a contiguous container */
		if (_iNode.pNode == first)
		{
			first = first->_next;
			delete _iNode.pNode;
			return true;
		}
		else
		{
			for (Node<T>* iter = first; iter->_next; iter = iter->_next)
			{
				if (iter->_next == _iNode.pNode) //Find our node.
				{
					iter->_next = _iNode.pNode->_next;
					delete _iNode.pNode;
					return true;
				}
			}
		}

		return false;
	}
};


int main(void)
{
	List<int> list;
	list.push_back(3);
	list.push_back(4);
	list.push_front(2);
	list.push_front(1);

	/*Print all elements*/
	for (List<int>::iterator iter = list.begin();
	     iter != list.end(); ++iter)
	{
		std::cout << (*iter) << std::endl;
	}

	/*Delete second element and reprint*/
	List<int>::iterator tmp = list.begin() + 1;
	list.erase(tmp);

	for (List<int>::iterator iter = list.begin();
	     iter != list.end(); ++iter)
	{
		std::cout << (*iter) << std::endl;
	}

	/*Now delete first node and print again*/
	tmp = list.begin();
	list.erase(tmp);

	for (List<int>::iterator iter = list.begin();
	     iter != list.end(); ++iter)
	{
		std::cout << (*iter) << std::endl;
	}

	std::cin.ignore();
	//List object takes care of deletion for us.
	return 0;
}



#endif
