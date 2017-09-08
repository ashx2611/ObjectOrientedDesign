#pragma once
#include"A.h"
#include"B.h"
#include"C.h"
#include<iostream>
int main()
{
	A a;
	B b;
	C c;
	b.blah();
	a.dosomething();
	c.blahblah();
	return 0;

}

void C::blahblah()
{
	cout << "this is C" << endl;
}
