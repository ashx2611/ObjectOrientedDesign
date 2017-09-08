#pragma once
/////////////////////////////////////////////////////////////////////
// StrongComponents.h - Finds the Strongly Connected Component		           //
// ver 1.0      
//Application: Demonstration for CSE687 OOD Project 4    
// Language:    C++ Visual Studio 2015                             //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10              //
// Source:      Jim Fawcett, CST 4-187, Syracuse University        //
//              (315) 443-3948, jfawcett@twcny.rr.com               //
// Author:     Ashwini Phadke, Syracuse University                 //
//              aphadke@syr.edu           						   	//

/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  Strong Components makes use of  TypeTable, Dependency Table package to find the strongly connected components.SCC supports displays for a list of all the strongly connected components.
*  (reference for strongly connected algorithm is wikipedia,com and geeksforgeeks)
*
*  Required Files:
*  ---------------
* - DependencyTable.h,DepAnal.h
* - TypeTable.h NoSqlDb.h
* - XmlPersist XmlDocument.h
*
*  Public Interface:
*  ---------------
*  - void sccomponent()--------call a recursive function to detect each dependency in the stack
*  - void startgraph();--------get data from NoSqlDb and store it in a list
*  - void addEdge(int v, int w)----addes an edge between each file and its corresponding dependency
*  - void makeComponent(int u, int discover[], int low[],stack<int> *st, bool stackMember[])----recursive funciton to find the strongly connected  components
*  - void displaySCCDb();------display the SSC Database
*  - void writescctoxml()-------write Db to an XmlFile
*
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*/
#include"StrongComponents.h"
#ifdef SC_TEST

int main()
{
	TypeAnal ta; //object of typetable
	DependencyTable dt;	//object of dependency table
	unordered_map<std::string, std::pair<std::string, std::string>> typetable;
	std::unordered_map<std::string, std::set<std::string>> temp;
	unordered_map<std::string, std::string> tempdeptable;
	typetable = ta.doTypeAnal();
	dt.getTypetable(typetable);//get populated typetable
	dt.DFS();
	dt.doDFS();
	dt.printdatabase();
	xml showxml;
	NoSqlDb<string> tempdb = dt.getdb();
	showxml.writexml(tempdb, argv[argc - 1]);

	temp = dt.getdeptable();
	NoSqlDb<string> db1 = dt.getdb();			//gets the dependency database
	SCC scc(temp.size(), db1);  //constructor for strongly connected components
	scc.buildgraph();           //makes a graph for scc
	scc.strongcomponent();
	

	return 0;
}
#endif 