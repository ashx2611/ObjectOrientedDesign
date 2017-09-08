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
*  It finds files for analysis using AbstrSynTree and, for each file found,uses TypeTable to check if the relationship exists
*  and builds dependency table
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
* void DFS(ASTNode* pNode);----get all components of file path
	void builddeptable(string filespec);---recursive call to add files and its dependencies into the dependency table	
	void print(string filespec)----prints the dependency table
	void printdatabase();----prints the data from nosql db to console
	unordered_map<string, set<string>> getdeptable();---returns dependency table
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
#include<iterator>
#include"../FileMgr/FileMgr.h"
#include"../FileSystem/FileSystem.h"
#include"../NoSqlDb/NoSqlDb.h"
#include "../CppProperties/CppProperties.h"
//#include"../XmlPersist/XmlPersist.h"
#include"DepAnal.h"
#include<set>
using namespace std;
//using namespace XmlProcessing;
using dependtabentry = unordered_map<string, set<string>>;
using tabcolumns = pair<string, string>;
using tabentry = unordered_map<string, tabcolumns>;
//using SPTr = shared_ptr<AbstractXmlElement>;
using StrData = string;
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
	void printDA(string filespec);
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
////getting the root node of the filepath and by recursive call, get each component
inline void DependencyTable::DFS()
{
	std::cout << "\nMaking a dependency table.....please wait" << endl;
	ASTNode* pRoot = ASTref_.root();
	DFS(pRoot);
}
//function to get directory & files from filepath
inline void DependencyTable::DFS(ASTNode* pNode)
{
	static std::string path = "";
	if (pNode->path_ != path)
	{
		path = pNode->path_;
	}
	if (doDisplay(pNode))
	{
		//std::cout << "\n  " << pNode->name_;
		//std::cout << ", " << pNode->type_;
	}
	vecstr.insert(pNode->path_);
	for (auto pChild : pNode->children_)
		DFS(pChild);
}
//create dependency table
inline void DependencyTable::doDFS()
{
///	cout << "\n******************Requirement 5*********************" << endl;
	cout << "\n****************** \nDisplaying Dependency Table\n******************\n" << "----------------------------------------------------\n";
	cout << std::left << std::setw(30) << "File Name" << "Dependent Files \n" << "--------------------------------------------------------------\n";
	for (auto iter = vecstr.begin(); iter != vecstr.end(); ++iter)
	{
		builddeptable(*iter);
	}
}

//function to store dependencies to database and file
inline void DependencyTable::builddeptable(string filespec)
{
	string filename="";
	if (filespec != "")
	{
		static string path = "";
		try {
			ifstream in(filespec);

			if (in.good())
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
							filename = tt.first;//putting file name
							string dependency = tt.second.second;   //get each dependency of the filename
							tempvector.insert(dependency); //store dependencies in vector
						}
					}

				} while (in.good());
				//put data to database elements
				elem.name = filespec;
				for (auto vecit = tempvector.begin(); vecit != tempvector.end(); ++vecit) {
					child.push_back(*vecit);
				}
				elem.childprop = db.addchild(child);    //save dependencies as children of each element into the database
				db.save(elem.name, elem);
				deptableentry.insert(make_pair(filespec, tempvector));
			}
			else {
				cout << "Cannot open file!";
				exit(0);
			}
			printDA(filespec);
			cout << endl;
			cout << "\n";
		}
		catch(logic_error &ex)
		{
			cout << "\n" << ex.what();
		}
	
	}
}


inline unordered_map<string, set<string>> DependencyTable::getdeptable()
{
	return deptableentry;
}
//function to print deptable on console
inline void DependencyTable::printDA(string filename)
{
	cout << "\n" << setw(80) << left << filename << setw(40) << "Depends On";
	for (auto set_it = deptableentry[filename].begin(); set_it != deptableentry[filename].end(); ++set_it)
	{
		cout << "\n" << endl;
		cout << setw(80) << " " << *set_it << "\n";
	}
}

inline void DependencyTable::printdependencytable()
{
	cout << "\n\n--------------------------------------------------------------Dependency table --------------------------------------------------------------------------------------------------------\n\n";
	std::cout << left << std::setw(80) << "FileName" << left << std::setw(80) << "Depends on" << endl;
	cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
	for (auto i = dependencyTable.begin(); i != dependencyTable.end(); i++)
	{
		std::cout << "\n" << left << std::setw(80) << i->first << left << std::setw(80) << i->second;
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





