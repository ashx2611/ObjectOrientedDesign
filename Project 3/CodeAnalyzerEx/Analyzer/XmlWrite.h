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
*  xml write provides function to write data to xml file
* - Parser.h
*  - AbstrSynTree.h
*
* Public Interface:
*  ---------------
*void writexml(NoSqlDb<string> db,std::string arg)----function to write data fron database to xml
*
**  Maintanence History:
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
//#include"../XmlPersist/XmlPersist.h"
using namespace std;
//using namespace XmlProcessing;
//using SPTr = std::shared_ptr<AbstractXmlElement>;
using StrData = std::string;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
class xml
{
public:void writexml(NoSqlDb<string> db,std::string arg);
};
//writes data from database to xml file
//takes args from command line
inline void xml::writexml(NoSqlDb<string> db,std::string args)
{
	
	std::ofstream oodprojXml;
	oodprojXml.open(args);
	if (oodprojXml.good())
	{
		//XmlPersist<string> p;
		//string s = p.toxml(db);
	//	oodprojXml << s;
		cout << "\n" << "***********Requirement 7***********" << endl;
//		cout << "To xml" << s;
		cout << "\n \n";
	}
	oodprojXml.close();
}
#endif
