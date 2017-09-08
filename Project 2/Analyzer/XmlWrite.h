#ifndef xml_H
#define xml_H
////////////////////////////////////////////////////////////////////////
// XmlWrite.h
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 2              //
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
#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>
#include"TypeTable.h"
#include<iterator>
#include"../FileMgr/FileMgr.h"
#include"../FileSystem/FileSystem.h"
#include"../NoSqlDb/NoSqlDb.h"
#include "../CppProperties/CppProperties.h"
#include"../XmlPersist/XmlPersist.h"
using namespace std;
using namespace XmlProcessing;
using SPTr = std::shared_ptr<AbstractXmlElement>;
using StrData = std::string;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
class xml
{
public:void writexml(NoSqlDb<string> db,std::string arg);
};

inline void xml::writexml(NoSqlDb<string> db,std::string args)
{
	
	std::ofstream Project1Xml;
	Project1Xml.open(args);
	if (Project1Xml.good())
	{
		XmlPersist<string> p;
		string s = p.toxml(db);
		Project1Xml << s;
		cout << "To xml" << s;
		cout << "\n \n";
	}
	Project1Xml.close();
}
#endif
