/*
 * SMI.hpp
 *
 *  Created on: 2018. 12. 22.
 *      Author: finegs
 */

#ifndef INCLUDE_SMI_HPP_
#define INCLUDE_SMI_HPP_

#include <iostream>
#include <cstdlib>

class MyClass
{
	//...
};

int f01();
int f02();
int f03();

namespace smi
{

using namespace std;

class String
{
private:
	char* s;
public:
	String() : s(nullptr) {}
	explicit
	String(const char* _src);
	String(const String& _o);
	String(String&&);
	const char* getS() const;

	String& operator=(const  String&);
	String& operator+=(const  String&);
	bool operator==(const String&);
	String operator+(const String&) const;
	friend ostream& operator<<(ostream&, const  String&);
//	friend istream& operator>>(const istream&,  smi::String&);
};

}

#endif /* INCLUDE_SMI_HPP_ */
