//////////////////////////////////////////////////////////////////////////
//// DependencyTable.cpp                                                  //
////Ver 1.0                                                               //
//// Application: Demonstration for CSE687 OOD Project 4                //
//// Language:    C++ Visual Studio 2015                                //
//// Platform:    Dell Inspiron 13, Core-i7, Windows 10                 //
//// Source:      Jim Fawcett, CST 4-187, Syracuse University          //
////              (315) 443-3948, jfawcett@twcny.rr.com                //
//// Author:     Ashwini Phadke, Syracuse University                   //
////              aphadke@syr.edu                                      //
////////////////////////////////////////////////////////////////////////
///*
//*  Package Operations:
//*  ===================
//*  DependencyTable package creates a dependency table associated with the typetable and stores dependencies to databse
//*
//*  Executive supports displays for:
//*  - a list of the names,types and filenames of all classes,structs,functions.
//*
//*
//*  Required Files:
//*  ---------------
//*
//*  - Parser.h, TypeTable.h,FileMgr.h,FileSystem.h,NoSqldb.h,Cppproperties.h,xmlpersist.h
//*  * Public Interface:
//*  ---------------
//* void DFS(ASTNode* pNode);----performs recursive DFS on the tree.
//void builddeptable(string filespec);---creates a dependency table
//void print(string filespec)----prints the dependency table
//void printdatabase();----prints the data from nosql db to console
//*
//*
//*
//*  Maintanence History:
//*  --------------------
//*  ver 1.0
//*  - first release
//**/
#pragma once

#include"DependencyTable.h"
#ifdef DT_Test
int main()
{
	DependencyTable dt;
	dt.getdb();
	dt.DFS();
	dt.doDFS();
	dt.printdatabase();
	return 0;
}
#endif