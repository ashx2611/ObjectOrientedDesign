////////////////////////////////////////////////////////////////////////
// XmlWrite.h
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 4              //
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
**/#pragma once
#include "XmlWrite.h"
//Test stub for xmlwrite
#ifdef XML_Test
int main()
{
	xml xml;
	NoSqlDb<string> n;
	xml.writexml(n);

}
#endif