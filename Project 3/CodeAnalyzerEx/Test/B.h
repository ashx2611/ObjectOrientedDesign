#pragma once
#include <iostream>
#include "C.h"

class B {
public:
	void showB();
private:
	C c;

};

void B::showB() {
	std::cout << "\nHello from B!!\n";
	c.showC();
}

