/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
////////////////////////////////////////////////////////////////////////
// Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 1                //
// Language:    C++ Visual Studio 2015                                //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10                 //
// Source:      Jim Fawcett, CST 4-187, Syracuse University          //
//              (315) 443-3948, jfawcett@twcny.rr.com                //
// Author:     Ashwini Phadke, Syracuse University                   //
//              aphadke@syr.edu                                      //
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides functions to add, update,delete data to database
*//*
* Maintenance:
* ------------
* Required packages: CppProperties,XmlDocument,SteHelper.h
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
/////////////////////////////////////////////////////////////////////
#pragma once
#include "NoSqlDb.h"
#include <iostream>
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
using namespace std;
int main()
{
	std::cout << "\n  Demonstrating NoSql Helper Code";
	std::cout << "\n =================================\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
	NoSqlDb<StrData> db;
	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.timeDate = elem1.showtime();
	elem1.data = "elem1's StrData";
	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.timeDate = elem2.showtime();
	elem2.data = "elem2's StrData";
	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.timeDate = elem3.showtime();
	elem3.data = "elem3's StrData";
	vector<string> childof;
	childof = {elem3.name };
	elem1.childprop = db.addchild(childof);
	vector<string> childof2;
	childof2 = { elem1.name,elem3.name };
	elem2.childprop = db.addchild(childof2);
	//saving all the database elements
	db.save(elem1.name, elem1);
	db.save(elem2.name, elem2);
	db.save(elem3.name, elem3);
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";
	cout << "Removing element 3 from data base" << endl;
	db.remove("elem3");
	cout<< "current number of items in database : " <<db.count()<<endl;
	Keys keys1 = db.keys();
	for (Key key2 : keys1)
	{
		std::cout << db.value(key2).show();
	}
	
}

	


  