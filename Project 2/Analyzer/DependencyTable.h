#ifndef deptable_H
#define deptypetable_H
////////////////////////////////////////////////////////////////////////
// DependencyTable.h                                                    //
//Ver 1.0                                                               //
// Application: Demonstration for CSE687 OOD Project 2                 //
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
*  DependencyTable package creates a dependency table associated with the typetable and stores dependencies to databse
*
*  Executive supports displays for:
*  - a list of the names,types and filenames of all classes,structs,functions.
*
*
*  Required Files:
*  ---------------
*
*  - Parser.h, TypeTable.h,FileMgr.h,FileSystem.h,NoSqldb.h,Cppproperties.h,xmlpersist.h
*  * Public Interface:
*  ---------------
* void DFS(ASTNode* pNode);----performs recursive DFS on the tree.
	void builddeptable(string filespec);---creates a dependency table	
	void print(string filespec)----prints the dependency table
	void printdatabase();----prints the data from nosql db to console
*
*  	
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
//#include"../Analyzer/TypeTable.h"
//#include"../AbstractSyntaxTree/AbstrSynTree.h"
#include<iterator>
#include"../FileMgr/FileMgr.h"
#include"../FileSystem/FileSystem.h"
#include"../NoSqlDb/NoSqlDb.h"
#include "../CppProperties/CppProperties.h"
#include"../XmlPersist/XmlPersist.h"
#include"DepAnal.h"
#include<set>
using namespace std;
using namespace XmlProcessing;
using dependtabentry = unordered_map<std::string, set<string>>;
using tabcolumns = std::pair<std::string, std::string>;
using tabentry = std::unordered_map<std::string, tabcolumns>;
using SPTr = std::shared_ptr<AbstractXmlElement>;
using StrData = std::string;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
class DependencyTable
{
public:
	DependencyTable();
	void printdependencytable();
	void printdatabase();
	NoSqlDb<string> getdb();
	void getTypetable(unordered_map<string, pair<string,string>> typetable_);
	AbstrSynTree& ASTref_;
	void DFS();
	void doDFS();
	void DFS(ASTNode* pNode);
	void builddeptable(string filespec);	
	unordered_map<string, set<string>> getdeptable();
	void print(string filespec);
private:
	Element<StrData> elem;
	NoSqlDb<string> db;
	TypeTable temp;
	unordered_map<string, pair<string, string>> Table;
	unordered_map<string, string> dependencyTable;
	dependtabentry deptableentry;
	set<string> vecstr;
};
inline DependencyTable::DependencyTable() :ASTref_(Repository::getInstance()->AST())
{
}
//function to return database which contains dependency details
inline NoSqlDb<string> DependencyTable::getdb()
{
	return db;
}

//function to return typetable
inline void DependencyTable::getTypetable(unordered_map<string, pair<string,string>> typetable)
{
	Table = typetable;
}
//function to recursively traverse on proot
inline void DependencyTable::DFS()
{
	std::cout << "\nMaking a dependency table.....please wait" << endl;;
	ASTNode* pRoot = ASTref_.root();
	DFS(pRoot);
}
inline void DependencyTable::DFS(ASTNode* pNode)
{
	static std::string path = "";
	if (pNode->path_ != path)
	{
		path = pNode->path_;
	}
	if (doDisplay(pNode))
	{
		std::cout << "\n  " << pNode->name_;
		std::cout << ", " << pNode->type_;
	}
	vecstr.insert(pNode->path_);
	for (auto pChild : pNode->children_)
		DFS(pChild);
}

inline void DependencyTable::doDFS()
{
	for (auto iter = vecstr.begin(); iter != vecstr.end(); ++iter)
	{
		builddeptable(*iter);
	}
}

inline void DependencyTable::builddeptable(string filespec)
{
	if (filespec != "")
	{
		static string path = "";
		ifstream in(filespec);
		if (!in.good())
		{
			cout << "cant open";
		}
		else
		{
			Scanner::Toker toker;
			toker.returnComments();
			toker.attach(&in);
			set<string> tempvector;
			vector<string>child;
			do
			{
				string tok = toker.getTok();
				for (auto tt : Table)
				{
					if (tok == tt.first)
					{
						//add type
						tempvector.insert(tt.second.second);
						child.push_back(tt.second.second);
					}
				}

			} while (in.good());
			//put data to database elements
			elem.name = filespec;
			elem.childprop = db.addchild(child);
			db.save(elem.name, elem);
			deptableentry.insert(make_pair(filespec, tempvector));
		}
		print(filespec);

		cout << "\n";
	}
}
inline unordered_map<string, set<string>> DependencyTable::getdeptable()
{
	return deptableentry;
}
//function to print deptable on console
inline void DependencyTable::print(string filespec)
{
	cout << "\n" << setw(40) << left << filespec << setw(40) << "depends on";
	for (auto set_it = deptableentry[filespec].begin(); set_it != deptableentry[filespec].end(); ++set_it)
	{
		cout << "\n" << endl;
		cout << setw(70) << " " << *set_it << "\n";
	}
}



inline void DependencyTable::printdependencytable()
{
	cout << "\n\n---------------------- Dependency table -------------------\n\n";
	std::cout << left << std::setw(50) << "FileName" << left << std::setw(50) << "Depends on" << endl;
	cout << "----------------------------------------------------------------";
	for (auto i = dependencyTable.begin(); i != dependencyTable.end(); i++)
	{
		std::cout << "\n" << left << std::setw(50) << i->first << left << std::setw(50) << i->second;
	}
}

inline void DependencyTable::printdatabase()
{
	cout << "\n\n---------------------- Putting data in the NoSql Database-------------------\n\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();	
	}
}
#endif





