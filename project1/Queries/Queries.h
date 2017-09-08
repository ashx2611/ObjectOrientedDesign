////////////////////////////////////////////////////////////////////////
// Queries.h
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
* This package provides functions to query the database to obtain associated values or keys.
*//*
* Maintenance:
* ------------
* Required packages: CppProperties,XmlDocument,NoSqlDb,StrHelper
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
/////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable : 4996)

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
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

//contains all the queries to be performed on database
template<typename Data>
class Queries
{
public:	
	Keys findvaluequery(Keys k);
	Keys findchildrenquery(Keys k);
	Keys findpatternquery(Keys k);
	Keys finstringnamequery(Keys k);
	Keys finstringcategoryquery(Keys k);
	Keys finstringdataquery(Keys k);
	Keys unionofqueries(Keys Query1, Keys Query2);
	Element<Data> objelem;
	NoSqlDb<Data> objnosql;
	template<typename Data>
	friend class TExec;	
};

template <typename Data>
Keys Queries<Data>::findvaluequery(Keys k)
{
	Keys temp;
	//iterate through the entire store
	for(vector<string>::iterator bt=k.begin();bt!=k.end();bt++)
	{ 
	//if key not found
	if (objnosql.store.find(*bt) == objnosql.store.end())
	{
		cout<<"Data not found"<<endl;
	}
 //if given key matches the store
	else
	{	    
			Element<Data> elem = objnosql.store[*bt];
			//printing the value
			cout << "\n    " << std::setw(8) << "name" << " : " << elem.name;
			cout << "\n    " << std::setw(8) << "category" << " : " << elem.category;
			cout << "\n    " << std::setw(8) << "timeDate" << " : " << elem.timeDate;
			cout << "\n    " << std::setw(8) << "data" << " : " << elem.data;
			cout << "\n    " << std::setw(8) << "children" << " : ";
			vector<string> c;
			c = elem.childprop;

			for (vector<string>::iterator it = c.begin(); it != c.end(); it++)
			{
				cout << *bt;
				cout << " ";
			}
			cout << "\n";
 // Inserting all the values associated with keys in temp vector
			temp.push_back(*bt);

	}
	}
	return temp;
}

//function to give children of associated key element
template<typename Data>
Keys Queries<Data>::findchildrenquery(Keys k)
{
	Keys temp;
	//iterating through the entire keys vector
	for (vector<string>::iterator bt = k.begin(); bt != k.end(); bt++)
	{
		//if key not found
		if (objnosql.store.find(*bt) == objnosql.store.end())
		{
			cout << "not found" << endl;
		}
		else
		{
			Element<Data> elem = objnosql.value(*bt);
			cout << "Printing children of "<<elem.name << " : " << endl;
			vector<string> tempchild;
			tempchild = elem.childprop;
			//checking if element has children
			if (!tempchild.empty())
			{
				//iterating over all the children of the element
				for (vector<string>::iterator it = tempchild.begin(); it != tempchild.end(); it++)

					cout << *it << endl;
				temp.push_back(*bt);
			}
			else
				cout << "Element has no child" << endl;
		}
	}
	//returning vector of strings containing children
	return temp;
}
//function to find elements with matching pattern in key..here it  is "elem1"
template <typename Data>
Keys Queries<Data>::findpatternquery(Keys k)
{
	Keys temp;
	Keys default;
	//iterating over entire store to mind the pattern
	for (auto it = objnosql.store.begin(); it != objnosql.store.end(); it++)
	{
		Key tempkey = it->first;
		if (find(k.begin(), k.end(), tempkey) != k.end())
		{
			default.push_back(tempkey);
			//checking for specific pattern "elem:"
			size_t found = tempkey.find("elem1");
			if (found != std::string::npos)
			{
				temp.push_back(tempkey);
			}

		}
	}
	if (!temp.empty())
	{
		//if the answer vector is not empty, print the result.
		for (auto it = temp.begin(); it != temp.end(); it++)
		{
			cout << *it << " " << endl;
		}
		return temp;
	}
	else
		cout << "Element associated to pattern not found" << endl;
	cout << "Returning all default keys present" << endl;
	//printing the default vector in case of patterns dont match
	for (auto it = default.begin(); it != default.end(); it++)
	{
		cout << *it << " " << endl;
	}
	return default;
}

//function to find the pattern in the name of the element ..pattern here is "ele"
template<typename Data>
typename  Keys  Queries<Data>::finstringnamequery(Keys k)
{
	Keys temp;
	for (auto it = k.begin(); it != k.end(); it++)
	{
		Element<Data> ele = objnosql.value(*it);
		string tempname = ele.name;
		size_t found = tempname.find("ele");
		if (found != std::string::npos)
		{
			temp.push_back(*it);
		}
		else
			cout << "Element not found" << endl;
	}
	for (auto it = temp.begin(); it != temp.end(); it++)
	{
		cout << *it << " " << endl;
	}
	return temp;
}

//Function to give elements having similar patttern in their category section
template<typename Data>
typename  Keys  Queries<Data>::finstringcategoryquery(Keys k)
{
	Keys temp;
	
	for (auto it = k.begin(); it != k.end(); it++)
	{
		//storing the value of the associated key in element obj
		Element<Data> ele = objnosql.value(*it);
		//Element<Data> ele = it->second;
		string tempname = ele.category;
		//comparing the two string
		size_t found = tempname.find("category");
		if (found != std::string::npos)
		{
			temp.push_back(*it);
		}
		else
			cout << "Element not found" << endl;
	}
	for (auto it = temp.begin(); it != temp.end(); it++)
	{
		cout << *it << " " << endl;
	}
	return temp;
}

template<typename Data>
typename  Keys  Queries<Data>::finstringdataquery(Keys k)
{
	Keys temp;
	try
	{
		for (auto it = k.begin(); it != k.end(); it++)
		{
			//storing the value of the associated key in element obj
			Element<Data> ele = objnosql.value(*it);
			//Element<Data> ele = it->second;
			string tempname = ele.data;
			//comparing the two string
			size_t found = tempname.find("ata");
			if (found != std::string::npos)
			{
				temp.push_back(*it);
			}
			else
				cout << "Element not found" << endl;
		}
		for (auto it = temp.begin(); it != temp.end(); it++)
		{
			cout << *it << " " << endl;
		}
		return temp;
	}
	catch (exception e)
	{
		cout << "Invalid datatype" << endl;
		cout << " Pattern not found" << endl;
		return temp;
	}
}
//Function to find the union of two queries
template<typename Data>
typename  Keys  Queries<Data>::unionofqueries(Keys q1ans, Keys q2ans)
{
	Keys temp;
	auto i = q1ans.begin();
	temp= q1ans;
	auto j = q2ans.begin();
	while (j != q2ans.end())
	{
		auto itr = temp.begin();
		//checking for duplicate elements
		if (std::find(temp.begin(), temp.end(), *j) != temp.end())
			j++;
		//pushing unique keys in resultant vector
		else
			temp.push_back(*itr);
	}
	//printing the result
	for (auto itrr = temp.begin(); itrr != temp.end(); itrr++)
	{
		cout << *itrr << endl;
	}
	return temp;
	
	}
