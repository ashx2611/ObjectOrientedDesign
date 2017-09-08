#pragma once
#pragma warning (disable : 4101) 
#ifndef depanal_H
#define depanal_H
////////////////////////////////////////////////////////////////////////
// Depanal.h
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
*  Conducts type analysis and displays data to console
*
*  Executive supports displays for:
*  - a list of the names,types and filenames of all classes,structs,using and enums.
*
*
*  Required Files:
*  ---------------
*  - Typetable.h
*
* Public Interfaces:
*  ---------------
*unordered_map<std::string, std::pair<std::string, std::string>> doTypeAnal();-----creates a typetable and prints to console
*
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#include <iostream>
#include <functional>
#include"TypeTable.h"
#include<iterator>
using namespace std;
using namespace CodeAnalysis;
	
		class TypeAnal
		{
		public:
			using SPtr = std::shared_ptr<ASTNode*>;
			TypeAnal();
			unordered_map<std::string, std::pair<std::string, std::string>> doTypeAnal();
		private:
			std::unordered_map<std::string, std::string> depTable;
			void DFS(ASTNode* pNode);
			AbstrSynTree& ASTref_;
			ScopeStack<ASTNode*> scopeStack_;
			Scanner::Toker& toker_;
			TypeTable tt;
		};

		inline TypeAnal::TypeAnal() :
			ASTref_(Repository::getInstance()->AST()),
			scopeStack_(Repository::getInstance()->scopeStack()),
			toker_(*(Repository::getInstance()->Toker()))
		{
			std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
		}                                              // It doesn't do anything useful for dep anal.

		inline bool doDisplay(ASTNode* pNode)
		{
			static std::string toDisplay[] = {
			  "function", "lambda", "class", "struct", "enum", "alias", "typedef","namespace"
			};
			for (std::string type : toDisplay)
			{
				if (pNode->type_ == type)
					return true;
			}
			return false;
		}

		inline void TypeAnal::DFS(ASTNode* pNode)
		{
			static std::string path = "";
			if (pNode->path_ != path)
			{
				std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
				path = pNode->path_;
			}
			if (doDisplay(pNode))
			{
				//std::cout << "\n  " << pNode->name_;
				//std::cout << ", " << pNode->type_;
			}
			tt.insertintoTTable(pNode);

			for (auto pChild : pNode->children_)
				DFS(pChild);
		}

		//creates typetable displaying name type and filename of associated files
		inline unordered_map<std::string, std::pair<std::string, std::string>> TypeAnal::doTypeAnal()
		{
			std::cout << "\n  starting type analysis:\n";
			std::cout << "\n  scanning AST and displaying important things:";
			std::cout << "\n -----------------------------------------------";
			ASTNode* pRoot = ASTref_.root();
			//performing DFS on the files
			DFS(pRoot);
			//printing the type table
			tt.display();
			return tt.getTypeTable();
		}

#endif

