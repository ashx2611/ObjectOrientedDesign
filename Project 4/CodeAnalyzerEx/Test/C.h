#pragma once
#include "D.h"
#include "A.h"	
	class C {
	public:
		void showC();
	private:
		A a;
	    D d;
	};

	void C::showC()
	{
		a.showA();
		std::cout << "\nHello from C\n";
	}



