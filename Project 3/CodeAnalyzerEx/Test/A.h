#pragma once
#include "B.h"
#include "C.h"
#include <iostream>

struct  x
{

};


	class A
	{
	public:
		void showA();
	private:
		B b;
		
	};

	void A::showA() {
		std::cout << "\nHello from A\n";
		file2.show2();
	}


