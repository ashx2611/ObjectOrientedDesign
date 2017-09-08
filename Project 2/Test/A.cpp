#pragma once
#include<iostream>
#include"A.h"
#include"B.h"
#include"C.h"
using namespace std;
#ifdef  TEST
#define TEST
int main()
{
	A a;
	B b;
	b.blah();
	a.dosomething();
	return 0;
}
#endif TEST

void A::dosomething()
{
	cout << "do something" << endl;
}
