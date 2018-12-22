/*
 * AppSMI.cpp
 *
 *  Created on: 2018. 12. 22.
 *      Author: finegs
 */

#include <SMI.hpp>


int main(int argc, char* argv[])
{
//	return f01();
//	return f02();
//	return f03();

	using namespace std;
	using namespace smi;

	String s1("aaa");

	String s2 = s1;

	cout << s1 << endl;
	cout << s2 << endl;
	cout << s1+s2 << endl;
}

