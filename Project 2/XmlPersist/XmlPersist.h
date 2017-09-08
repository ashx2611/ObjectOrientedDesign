//#define _CRT_SECURE_NO_WARNINGS
////////////////////////////////////////////////////////////////////////
// XmlPersist.h
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
* This package provides functions to read and write from xml file
*//*
* Maintenance:
* ------------
* Required packages: CppProperties,XmlDocument,NoSqlDb
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
#include<fstream>
#include <ctime>
using namespace std;
using namespace XmlProcessing;
using SPTr = std::shared_ptr<AbstractXmlElement>;
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
template<typename Data>
class XmlPersist
{
public:
	string toxml(NoSqlDb<Data> & db);
	Element<StrData> fromxml(const string & xml);

};

template<typename Data>
inline string XmlPersist<Data>::toxml(NoSqlDb<Data> & db)
{
	cout << "Writing data to xml" << endl;
	string xml;
	XmlDocument doc;
	//creating the xml structure by defining parent and children
	SPtr pRoot = makeTaggedElement("database");
	doc.docElement() = pRoot;
	Keys allkeys = db.keys();
	for (Key k : allkeys)
	{
		Element<Data> e = db.value(k);
		SPTr pElem = makeTaggedElement("ELement");
		SPTr pNameElem = makeTaggedElement("Name");
		SPTr pNameTextElem = makeTextElement(e.name);
		pNameElem->addChild(pNameTextElem);
		pElem->addChild(pNameElem);
		using Children = vector<string>;
		Children c = e.childprop;
		SPTr pchildElem = makeTaggedElement("Dependent Files");
		for (std::vector<string>::iterator it = c.begin(); it != c.end(); it++)
		{
			SPTr pchildTextElem = makeTextElement(*it);
			pchildElem->addChild(pchildTextElem);
		}
		pElem->addChild(pchildElem);
		pRoot->addChild(pElem);
	}
	//converting data to string
	xml = doc.toString();
	return xml;
}
template<typename Data>
inline Element<StrData> XmlPersist<Data>::fromxml(const string& xml)
{
	try {
		Element<Data> elem;
		XmlDocument doc(xml, XmlDocument::str);
		std::vector<SPTr> desc = doc.elements("database").select();
		for (SPTr elem : desc)
		{
			//retrieving data between the xml tags
			std::cout << "Name :" << elem->children()[0]->children()[0]->value();
			std::cout << "Category :" << elem->children()[1]->children()[0]->value();
			std::cout << "Data :" << elem->children()[2]->children()[0]->value();
			std::cout << "Children : " << elem->children()[4]->children()[0]->value();
			std::cout << "Time Date : " << elem->children()[3]->children()[0]->value();				
			elem->tag();		
		}
		return elem;
	}
	catch (exception e)
	{
		cout << "Cannot read from xml file!!Something is wrong!!" << endl;
		cout << e.what();
	}
}
//Contains method to create a xml file demonstrating the project structure
class ProjStruct
{
public:	
	string xmlstruct();
};
inline std::string ProjStruct:: xmlstruct()
{
	std::ofstream ProjectStruc;
	ProjectStruc.open("../projectstruc.xml"); 
	std::string xml;
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("Project 1");
	doc.docElement() = pRoot;
	SPtr pTestExecElem = makeTaggedElement("Test Executive");
	pRoot->addChild(pTestExecElem);
	SPtr pDependentfiles = makeTaggedElement("Dependencies");
	SPtr pTextElem = makeTextElement("XmlDocument, NoSqlDb,Queries,CppProperties,XmlPersist");
	pDependentfiles->addChild(pTextElem);
	pTestExecElem->addChild(pDependentfiles);
	SPtr pNoSqlDb = makeTaggedElement("NoSqlDb");
	pRoot->addChild(pNoSqlDb);
	SPtr pDependent = makeTaggedElement("Dependencies");
	SPtr pTextElem1 = makeTextElement("XmlDocument, CppProperties");
	pDependent->addChild(pTextElem1);
	pNoSqlDb->addChild(pDependent);
	SPtr pQueries = makeTaggedElement("Queries");
	pRoot->addChild(pQueries);
	SPtr pDependent1 = makeTaggedElement("Dependencies");
	SPtr pTextElem11 = makeTextElement("NoSqlDb,CppProperties");
	pDependent1->addChild(pTextElem11);
	pQueries->addChild(pDependent1);
	SPtr pXmlPersist = makeTaggedElement("XmlPersist");
	pRoot->addChild(pXmlPersist);
	SPtr pDepend = makeTaggedElement("Dependencies");
	SPtr pText = makeTextElement("NoSqlDb,CppProperties");
	pDepend->addChild(pText);
	pXmlPersist->addChild(pDepend);
	xml = doc.toString();
	return xml;
	ProjectStruc << xml;
	ProjectStruc.close();

}
