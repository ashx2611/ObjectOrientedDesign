////////////////////////////////////////////////////////////////////////
// XmlPersist.cpp
//Ver 1.0                                                             //
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
* This package demonstrates functions to read and write from xml file
*//*
* Maintenance:
* ------------
* Required packages: CppProperties,XmlDocument,NoSqlDb,StrHelper
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
/////////////////////////////////////////////////////////////////////
#pragma once
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../StrHelper.h"
#include "XmlPersist.h"
#include<fstream>
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
using namespace std;
#if XMLPERSIST
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
	//making children of elements
	vector<string> childof;
	childof = { elem2.name };
	elem1.childprop = db.addchild(childof);
	vector<string> childof2;
	childof2 = { elem3.name };
	elem2.childprop = db.addchild(childof2);
	vector<string> childof3;
	childof3 = { elem1.name };
	elem3.childprop = db.addchild(childof3);
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
	std::ofstream Project1Xml;
	Project1Xml.open("../XmlFile.xml");

	XmlPersist<StrData> p;
	string s = p.toxml(db);
	Project1Xml << s;
	cout << "To xml" << s;
	cout << "\n \n";
	Project1Xml.close();

	XmlPersist<StrData> p1;
	cout << "**********FROM XML***********" << endl;
	if (!(Project1Xml.eof()))
	{
		Element<StrData> db12;
		std::ifstream ifs("../XmlFile.xml");
		std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		db12 = p1.fromxml(content);

	}
}
#endif

	
	


