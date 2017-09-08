
#pragma once
#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// NoSqlDb.h -provides add,update,delete functionalities in database//
// Ver 1.0                                                         //
// Application: Demonstration for CSE687 OOD Project 1           //
// Language:    C++ Visual Studio 2015                     //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10                //
// Source:      Jim Fawcett, CST 4-187, Syracuse University         //
//              (315) 443-3948, jfawcett@twcny.rr.com               //
// Author:     Ashwini Phadke, Syracuse University                   //
//              aphadke@syr.edu                    //
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides functions to add, update,delete data to database
*//*
* Maintenance:
* ------------
* Required packages: CppProperties,XmlDocument,SteHelper.h
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../StrHelper.h"
#include<fstream>
#include <ctime>
using namespace std;
using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;
/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
template<typename Data>
class Element
{
public:
  using Name = std::string;
  using Category = std::string;
  using TimeDate = std::string;
  using Children = std::vector<string>;
  Property<Name> name;            // metadata
  Property<Category> category;    // metadata
  Property<TimeDate> timeDate;    // metadata
  Property<Children> childprop;   //metadata
  Property<Data> data;            // data
  std::string show();
  string showtime();
};
//Function to display data on console
template<typename Data>
std::string Element<Data>::show()
{
  std::ostringstream out;
  out.setf(std::ios::adjustfield, std::ios::left);
  out << "\n    " << std::setw(8) << "name"     << " : " << name;
  out << "\n    " << std::setw(8) << "category" << " : " << category;
  out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
  out << "\n    " << std::setw(8) << "data"     << " : " << data;
  out << "\n    " << std::setw(8) << "children" << " : " ;
  Children c;//printing children of the element
  c = childprop;
  for(vector<string>::iterator it =c.begin();it!=c.end();it++ )
  {
	  out << *it << " " << endl;
  }  
  return out.str();
}
//function to display the timestamp
template<typename Data>
std::string Element<Data>::showtime()
{
	time_t rawtime;
	time(&rawtime);
	return ctime(&rawtime);
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database            //
// - stores and retrieves elements                                 //
/////////////////////////////////////////////////////////////////////
template<typename Data>
class NoSqlDb
{
public:
  using Key = std::string;
  using Keys = std::vector<Key>;
  Keys keys();
  bool save(Key key, Element<Data> elem);  
  bool update(Key key, string cat, string dat,string t, vector<string> ch);
  void remove(Key key);
  Keys addchild(Keys ch);
  template<typename Data>
  friend class Queries;
  template<typename Data>
  friend class TExec;
  Element<Data> value(Key key);
  size_t count();
private:
  using Item = std::pair<Key, Element<Data>>;
  std::unordered_map<Key,Element<Data>> store; 
};

//function returning set of keys in the database store
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
  Keys keys;
  for (Item item : store)
  {
    keys.push_back(item.first);
  }
  return keys;
}

//function to add data to database
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
  if(store.find(key) != store.end())
    return false;
  store[key] = elem;
  return true;
}


//function to add child to the element
template<typename Data>
typename NoSqlDb<Data>::
Keys NoSqlDb<Data>::addchild(Keys ch)
{
	Keys temp;
	//iterate through the string vector passed and put the values in temp vector to return
	for (vector<string>::iterator it = ch.begin(); it != ch.end(); it++)
	{
		temp.push_back(*it);
	}
	return temp;
}

//function to add value of the associated key
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	//if key is found
  if (store.find(key) != store.end())
    return store[key];//return value of the associated key
  return Element<Data>();
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
  return store.size();
}
//function to update the value of the element in database
template<typename Data>
bool NoSqlDb<Data>::update(Key key, string cat, string dat,string t,vector<string> ch)
{//updating the new values in ehe element
	Element<Data> elem;
	elem.name = key;
	elem.category = cat;
	elem.data = dat;
	elem.timeDate = t;
	elem.childprop = ch;
	//if key is found
	if (store.find(key) != store.end())
	{
		//update the value
		store[key] = elem;
		return true;
	}
	else
		return false;
	}

template<typename Data>								
void NoSqlDb<Data>::remove(Key key)
{
	cout << "Deleting data associated with key and its relationship in other element" << endl;
	std::unordered_map<Key, Element<Data>> tempstore;
	vector<string> tempchildren;
	Key tempkey = key;
	for (Item item : store)
	{
		Element<Data> elem;
		elem= item.second;
		tempchildren = elem.childprop;

		for (auto childiterator = tempchildren.begin(); childiterator != tempchildren.end(); childiterator++)
		{
			if (key == *childiterator)
			{
				tempchildren.erase(childiterator);
				elem.childprop = tempchildren;
				store[elem.name] = elem;
				break;
			}
		}
	}
	store.erase(key);

	for (Item item : store)
	{
		Element<Data> element = value(item.first);
		vector<string> temp = element.childprop;			
	}
}


