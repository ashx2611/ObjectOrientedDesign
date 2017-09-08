////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp                                                  //
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 1                //
// Language:    C++ Visual Studio 2015                                //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10                 //
// Source:      Jim Fawcett, CST 4-187, Syracuse University           //
//              (315) 443-3948, jfawcett@twcny.rr.com                 //
// Author:     Ashwini Phadke, Syracuse University                    //
//              aphadke@syr.edu                                       //
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package demonstrates all the requirements of project 1
*//*
* Maintenance:
* ------------
* Required packages: CppProperties,XmlDocument,NoSqlDb,StrHelper,Queries,XmlPersist
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
/////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable : 4996)

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include"../XmlPersist/XmlPersist.h"
#include "../NoSqlDb/NoSqlDb.h"
#include"../Queries/Queries.h"
#include "../StrHelper.h"
#include<fstream>
#include <ctime>
using namespace std;
using namespace XmlProcessing;
using SPTr = std::shared_ptr<AbstractXmlElement>;
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main()
{
	std::cout << "\n  Demonstrating NoSql Helper Code";
	std::cout << "\n =================================\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
	cout << "Adding data to database" << endl;
	Queries<StrData> query;
	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "elem1category";
	elem1.timeDate = elem1.showtime();
	elem1.data = "elem1's StrData";
	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "elem2category";
	elem2.timeDate = elem2.showtime();
	elem2.data = "elem2's StrData";
	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "elem3category";
	elem3.timeDate = elem3.showtime();
	elem3.data = "elem3's StrData";
	vector<string> childof;
	childof = { elem2.name};
	elem1.childprop = query.objnosql.addchild(childof);
	vector<string> childof2;
	childof2 = { elem3.name };
	elem2.childprop = query.objnosql.addchild(childof2);
	vector<string> childof3;
	childof3 = { elem1.name };
	elem3.childprop = query.objnosql.addchild(childof3);
	query.objnosql.save(elem1.name, elem1);
	query.objnosql.save(elem2.name, elem2);
	query.objnosql.save(elem3.name, elem3);
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	std::cout << "\n  size of db = " << query.objnosql.count() << "\n";
	Keys keys = query.objnosql.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << query.objnosql.value(key).show();
	}
	std::cout << "\n\n";	
	std::ofstream Project1Xml;
	Project1Xml.open("../XmlFile.xml");
	XmlPersist<StrData> p;
	string s = p.toxml(query.objnosql);
	Project1Xml << s;
	cout << "***Persisting data to xml file***" << s<<endl;
	cout << "\n \n";
	Project1Xml.close();
	XmlPersist<StrData> p1;
	cout << "***Retrieving data from xml file***" << endl;
	if (!(Project1Xml.eof()))
	{
		Element<StrData> db12;
		std::ifstream ifs("../XmlFile.xml");
		std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		db12 = p1.fromxml(content);
	}
	cout << "****Displaying results of query which gives value of associated keys****" << endl;
	Keys testvector = { "elem1","elem2","elem3" };
	Keys k = query.findvaluequery(testvector);
	cout << "****Displaying result of query which gives children of associated keys****" << endl;
	Keys k1 = query.findchildrenquery(testvector);
	cout << "****Displaying result of query which gives keys associated to particular pattern****" << endl;
	Keys k2 = query.findpatternquery(testvector);
	cout << "****Displaying result of query which gives keys that contain specified string in item name****" << endl;
	Keys k3 = query.finstringnamequery(testvector);
	cout << "****Displaying result of query which gives keys that contain specified string in item category****" << endl;
	Keys k4 = query.finstringcategoryquery(testvector);
	cout << "****Displaying result of query which gives keys that contain specified string in item data***" << endl;
	Keys k5 = query.finstringdataquery(testvector);
	cout << "****Displaying requirement 8" << endl;
	cout << "Passing result of findstringnamequery to findvaluequery and displaying corresponding output" << endl;
	Keys k6 = query.findvaluequery(k3);
	cout << "****Displaying requirement 9****" << endl;
	cout << "****Displaying union of keys****" << endl;
	Keys k7 = query.unionofqueries(k3,k4);
	cout << "***Updating value of element 1 in database***" << endl;
	vector<string> st = { "elem3" };
	query.objnosql.update(elem1.name, "edited category","edited data", "Thu Feb 9 00:56:08 2017",st);
	cout << "***Displaying database after updating value***" << endl;
	for (Key key : query.objnosql.keys())
	{
	std::cout << "\n  " << key << ":";
	std::cout << query.objnosql.value(key).show();
	}
	cout << "Removing element 3 from data base" << endl;
	query.objnosql.remove("elem3");
	cout << query.objnosql.count();
	Keys keys1 = query.objnosql.keys();
	for (Key key2 : keys1)
	{
		std::cout << query.objnosql.value(key2).show();
	}
	ProjStruct x;
	std::string xml = x.xmlstruct();
	std::cout << "\n Demonstrating structure of the project. Requirement 10" << xml;
}



