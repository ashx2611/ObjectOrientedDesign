#pragma once
/////////////////////////////////////////////////////////////////////
// SCC.h - Finds the Strongly Connected Component		           //
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
/*
*  Package Operations:
*  ===================
*  SCC uses TypeTable, Dependency Table package to find the strongly connected components.
*
*  SCC supports displays for:
*  - a list of all the strongly connected components.
*
*
*  Required Files:
*  ---------------
* - DepTable.h
* - TypeTable.h
* - NoSqlDb.h
* - xmlpersist.h
* - XmlDocument.h
*
*  Package Operations:
*  ---------------
*  - void sccomponent();
*    =>call a recursive function to detect each dependency in the stack
*  - void startgraph();
*    =>get data from NoSqlDb and store it in a list
*  - void addEdge(int v, int w);
*    =>addes an edge between each file and its corresponding dependency
*  - void makeComponent(int u, int discover[], int low[],stack<int> *st, bool stackMember[]);
*    =>recursive funciton to find the strongly connected  components
*  - void displaySCCDb();
*    =>display the SSC Database
*  - void writescctoxml();
*    =>write Db to an XmlFile
*
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#ifndef SCC_H
#define SCC_H

#include"DependencyTable.h"
#include"TypeTable.h"
#include"DepAnal.h"
#include"../NoSqlDb/NoSqlDb.h"
#include"../xmlpersist/xmlpersist.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include <functional>
#include<unordered_map>
#include<vector>
#include<stack>
#include <iostream>
#include <list>
#include <algorithm>
#define NIL -1

using namespace std;
using namespace XmlProcessing;
using SPTr = std::shared_ptr<AbstractXmlElement>;
using StrData = std::string;
using StrData = std::string;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

class SCC
{
private:
	unordered_map<std::string, std::string> typetable;
	std::unordered_map<std::string, int>serialmap;
	DependencyTable dt;
	NoSqlDb<StrData> db;   //database created
	NoSqlDb<StrData> sccdb;  //scc database
	Element<StrData> sccelem; //scc element
	std::unordered_map<int, std::string> intgraph; //int,string graph for storing filenames
	int numberofVertices = 0;   //total number of vertices present
	list<int> *deplist;     //list to store filename and corresponding dependencies

public:
	SCC(int a, NoSqlDb<string> db1)
	{
		numberofVertices = a;
		deplist = new list<int>[a];
		db = db1;
	}
	void sccomponent(); //call a recursive function to detect each dependency in the stack
	void startgraph();  //get data from NoSqlDb and store it in a list 
	void addEdge(int v, int w);//addes an edge between each file and its corresponding dependency
	void makeComponent(int u, int discover[], int low[], stack<int> *st, bool stackMember[]); //recursive funciton to find the strongly connected  components
	void displaySCCDb(); //display the SSC Database
	void writescctoxml(); //write Db to an XmlFile

};


inline void SCC::startgraph()
{
	int numb = 0;
	for (std::string key : db.keys())
	{
		serialmap[key] = numb;
		intgraph[numb] = key;
		numb++;
	}
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::vector<std::string> childvec = db.value(key).childprop;
		for (std::string child : childvec)
		{
			if (!(key == child))
			{
				addEdge(serialmap[key], serialmap[child]);
			}
		}
	}
}


inline void SCC::addEdge(int v, int w)
{
	deplist[v].push_back(w);
}


inline void SCC::displaySCCDb()
{
	cout << "\n==================== \nREQUIREMENT 6 \n====================" << endl;
	cout << "\n\n****************** \nStroing the Strongly Connected Components in my NoSql Database\n**********************\n\n" << "-----------------------------------------------------------------------------";
	Keys keys = sccdb.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << sccdb.value(key).show();
	}
}


//put scc to an XML File
inline void SCC::writescctoxml()
{
	std::ofstream SCCXml;
	SCCXml.open("./SCCXmlFile.xml");
	if (SCCXml.good())
	{
		XmlPersist<string> p;
		string s = p.toxml(db);
		SCCXml << s;
		cout << "\n\n**********\nStoring SCC To SCCXmlFile.xml\n************" << s;
		cout << "\n \n";
	}
	SCCXml.close();
}

//function to find the strongly connected components
inline void SCC::makeComponent(int u, int discover[], int low[], stack<int> *stackobj, bool stackMember[])
{
	vector<string> sccchildren;
	static int time = 0;
	discover[u] = low[u] = ++time; // Initialize discoverovery time and low value
	stackobj->push(u);
	stackMember[u] = true;
	list<int>::iterator i;// Go through all vertices adjacent to this
	for (i = deplist[u].begin(); i != deplist[u].end(); ++i)
	{
		int v = *i;  // v is current adjacent of 'u'		
		if (discover[v] == -1)  // If v is not visited yet, then recur for it
		{
			makeComponent(v, discover, low, stackobj, stackMember);
			low[u] = min(low[u], low[v]);
		}
		else if (stackMember[v] == true)
			low[u] = min(low[u], discover[v]);
	}
	int w = 0;  // To store stack extracted vertices
	if (low[u] == discover[u])
	{
		while (stackobj->top() != u)
		{
			w = (int)stackobj->top();
			std::cout << intgraph[w] << "-> ";
			stackMember[w] = false;
			stackobj->pop();
		}
		w = (int)stackobj->top();
		std::cout << intgraph[w] << "\n";
		stackMember[w] = false;
		stackobj->pop();
	}
}

//display the graph with connected edges
//create a stack to make components
inline void SCC::sccomponent()
{
	cout << "\n==================== \nREQUIREMENT 6 \n====================" << endl;
	cout << "\n*******************\nStrong Component\n*******************\n";
	int *discover = new int[numberofVertices];
	int *low = new int[numberofVertices];
	bool *stackMember = new bool[numberofVertices];
	stack<int> *st = new stack<int>();

	// Initialize discover and low, and stackMember arrays
	for (int i = 0; i < numberofVertices; i++)
	{
		discover[i] = NIL;
		low[i] = NIL;
		stackMember[i] = false;
	}
	// Call the recursive helper function to find strongly
	// connected components in DFS tree with vertex 'i'
	for (int i = 0; i < numberofVertices; i++)
		if (discover[i] == NIL)
			makeComponent(i, discover, low, st, stackMember);
}



#endif