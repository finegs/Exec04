/*
 * example_dll.cpp
 *
 *  Created on: 2018. 9. 23.
 *      Author: finegs
 */

#include <cstdio>
#include "example_dll.hpp"

__stdcall void hello(const char *s)
{
        printf("Hello %s\n", s);
}
int Double(int x)
{
        return 2 * x;
}
void CppFunc(void)
{
        puts("CppFunc");
}
void MyClass::func(void)
{
        puts("MyClass.func()");
}
