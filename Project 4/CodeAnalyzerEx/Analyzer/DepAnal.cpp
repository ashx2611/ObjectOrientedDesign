////////////////////////////////////////////////////////////////////////
// Depanal.cpp---implementation of depanal.h
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 4             //
// Language:    C++ Visual Studio 2015                                //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10                 //
// Source:      Jim Fawcett, CST 4-187, Syracuse University          //
//              (315) 443-3948, jfawcett@twcny.rr.com                //
// Author:     Ashwini Phadke, Syracuse University                   //
//              aphadke@syr.edu                                      //
//////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  Conducts type analysis and displays data to console
*
*  Executive supports displays for:
*  - a list of the names,types and filenames of all classes,structs,using and enums.
*
*
*  Required Files:
*  ---------------
*  - Typetable.h
*
* Public Interfaces:
*  ---------------
*unordered_map<std::string, std::pair<std::string, std::string>> doTypeAnal();-----creates a typetable and prints to console
*
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#include "DepAnal.h"
#ifdef TEST_DEPANAL

int main()
{
	TypeAnal t;
	t.doTypeAnal();
}
#endif
