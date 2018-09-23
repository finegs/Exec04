/*
 * example.cpp
 *
 *  Created on: 2018. 9. 23.
 *      Author: finegs
 */

#include <cstdio>
#include "example_dll.hpp"

int main(void)
{
        hello("World");
        printf("%d\n", Double(333));
        CppFunc();

        MyClass a;
        a.func();

        return 0;
}

