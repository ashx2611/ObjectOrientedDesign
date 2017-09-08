
#pragma once
#include<iostream>
#include"B.h"
#include"A.h"

using namespace std;
#ifdef BLAH
#define BLAH
int main()
{
	A a;
	a.dosomething();
	B b;
	b.blah();
}
#endif

void B::blah()
{
	std::cout << "blahblah" << endl;
}
