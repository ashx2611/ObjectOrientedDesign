////////////////////////////////////////////////////////////////////////
// Typetable.cpp                                                      //
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 2                //
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
*  TypeTablemakes use of AbstrSynTree package to create a  type table depicting details of the file processed.
*
*  Executive supports displays for:
*  - a list of the names,types and filenames of all classes,structs,functions.
*
*
*  Required Files:
*  ---------------
*
*  - Parser.h, Parser.cpp
*  - AbstrSynTree.h
*
* Public Interface:
*  ---------------
*
*  	std::string getName() --getter function for Name in typetable
*  Type gettype()---getter function to return type from type table
*  unordered_map<std::string, std::pair<std::string, std::string>> &getTypeTable() --returns populated TypeTable that can be used by non class members
void insertintoTTable(CodeAnalysis::ASTNode* pNode) --inserts the name and type and filename associated with each class,struct,enum and using
*  void display();  --displays the type table
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
**/
#pragma once
#include"TypeTable.h"
using namespace std;
#ifdef TT
int main()
{

	TypeTable t;
	t.setName("abc.h");
	t.getName();
	t.setFileName("file 2");
	t.getfileName();
	t.setType("class");
	t.gettype();
	return 0;

}
#endif