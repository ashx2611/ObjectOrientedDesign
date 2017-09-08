#ifndef typetable_H
#define typetable_H
////////////////////////////////////////////////////////////////////////
// TypeTable.h
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 2              //
// Language:    C++ Visual Studio 2015                                //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10                 //
// Source:      Jim Fawcett, CST 4-187, Syracuse University          //
//              (315) 443-3948, jfawcett@twcny.rr.com                //
// Author:     Ashwini Phadke, Syracuse University                   //
//              aphadke@syr.edu     
//
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
*  - Parser.h,
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
#include<iterator>
#include"../AbstractSyntaxTree/AbstrSynTree.h"
using namespace std;

class TypeTable
{
	using Name = string;
	using Type =string;
	using FileName =string;

public:
	void setName(Name name) {
		_name=name;
	}
	std::string getName() {
		return _name;
	}
	void setType(Type tp) {
		_type = tp;
	}
	Type gettype() {
		return _type;
	}
	void setFileName(FileName f) {
		_filename = f;
	}
	FileName getfileName() {
		return _filename;
	}
	unordered_map < Name, pair<Type, FileName>>& getTypeTable()
	{
		return Type_Table;
	}

	void setTypeTable(unordered_map < Name, pair<Type, FileName>> temp)
	{

	}
	//creates a type table with name, type and filename of associated class,struct,enum and typedef
	void insertintoTTable(CodeAnalysis::ASTNode* pNode)
	{
		
		if (pNode->type_ == "class" || pNode->type_ == "struct" || pNode->type_ == "enum" || pNode->type_ == "typedef" || pNode->type_ == "using" || pNode->type_ == "namespace")
		{
			pair<string, string> temp;
			temp.first = pNode->type_;
			temp.second = pNode->path_;
			//add to table
			getTypeTable().insert(make_pair(pNode->name_, temp));
		}
		
	}
	//displays the typetable to console
	void display()
	{
		//cout << "\n\n" << "**************Requirement 4*********************" << endl;
		cout << "\n\n" << "Displaying TypeTable" << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\n " << setw(20) << "Name" << setw(20) << "Type" << setw(20) << "FileName" << "\n";
		cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		for (auto it = getTypeTable().begin(); it != getTypeTable().end(); it++)
		{
			cout << "\n " << setw(20) << it->first << std::setw(20) << it->second.first << setw(20) << it->second.second << "\n";
		}
	}
private:
	Name _name;
	Type _type;
	FileName _filename;
	unordered_map < Name, pair<Type, FileName>>Type_Table;
};
#endif

