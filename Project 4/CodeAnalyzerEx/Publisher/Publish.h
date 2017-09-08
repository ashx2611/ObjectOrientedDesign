
#pragma once
#ifndef PP_H
#define PP_H
/////////////////////////////////////////////////////////////////////
// Publish.h - Publishes  source code in the form of html pages	    //
// ver 1.0    
// Application: Demonstration for CSE687 OOD Project 4 
// Language:    C++ Visual Studio 2015                             //
// Platform:    Dell Inspiron 13, Core-i7, Windows 10              //
// Source:      Jim Fawcett, CST 4-187, Syracuse University        //
//              (315) 443-3948, jfawcett@twcny.rr.com              //
// Author:     Ashwini Phadke, Syracuse University                 //
//              aphadke@syr.edu           						   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
* void Publishcode();--creates html pages publishing source code and the dependent files of the files retrieved from the dependency table
string getfilename(const string& s);--/function to get just the files name from string path containing absoulute filepath
void generatecss();--function to create style sheet for the code publisher application
void generatehtmlfile(string pathof);--/function to create files with htm extension to publish.All files are created in HtmFolder.
void generateresultfile(vector<string>);--function to create result page containing index of all other files
*
*  Required Files:
*  ---------------
*
*  Public Interface:
*  ---------------
*
* *  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/

#include <iostream>
#include <functional>
#include<unordered_map>
#include<vector>
#include<set>
#include <direct.h>
#include<fstream>
#include <iterator>
#include<string>
#include"../Analyzer/Executive.h"

using namespace std;

	class PublishFiles
	{
	public:
		unordered_map<string, set<string>> deptabledata;

		PublishFiles(unordered_map<string, set<string>> getDepTable)
		{
			deptabledata = getDepTable;
		//	_mkdir(directoryname.c_str());
		}
		void Publishcode();
		string getfilename(const string& s);
		void generatecss();
		void generatehtmlfile(string pathof);
		void generateresultfile(vector<string>);

	private:
		string directoryname = "../ServerFiles";
		vector<string> dependentfilesvec;
		vector<string> resultvector;
		string pfirst;
		string fileName;
	};
	//creates html pages publishing source code and the dependent files of the files retrieved from the dependency table
	void PublishFiles::Publishcode()
	{
	//	_mkdir(directoryname.c_str());//creating directory to put the files
		for (auto it : deptabledata)
		{
			pfirst = it.first; //getting filepath from the dep table
			for (auto DTit = deptabledata[it.first].begin(); DTit != deptabledata[it.first].end(); DTit++)
			{
				string dependencypath = *DTit;
				string appendname = getfilename(dependencypath);
				fileName = "";
				appendname = "/" + appendname + ".htm";//adding htm extension
				fileName.append(directoryname);
				fileName.append(appendname);
				dependentfilesvec.push_back(fileName);//vector containing paths of the corresponding dependent files
			}
			generatehtmlfile(pfirst);
			resultvector.push_back(pfirst);
			dependentfilesvec.clear();
		}
		generateresultfile(resultvector);
	}

	//function to get just the files name from string path containing absoulute filepath
	inline string PublishFiles::getfilename(const string& s)
	{
		char separator = '/';
#ifdef _WIN32
		separator = '\\';
#endif
		size_t it = s.rfind(separator, s.length());
		if (it != string::npos) {
			return(s.substr(it + 1, s.length() - it));
		}
		return("");
	}


	//function to create style sheet for the code publisher application
	inline void PublishFiles::generatecss()
	{
		ofstream mystyle;
		mystyle.open("../ServerFiles/mystylesheet.css");
		mystyle << "h3{ font - color:black; border-bottom-style:solid #008B8B; }" << endl;
		mystyle << "body{" << endl;
		mystyle << "margin:20px; color:#008080;background-color:#FFF0F5;" << endl;
		mystyle << "font - family:Comic Sans; font - weight:600; font - size:120 % ;" << endl;
		mystyle << "}" << endl;
		mystyle << ".indent{ margin - left:20px; margin - right:20px; }" << endl;
		mystyle << "h4{ margin - bottom: 3px; margin - top:3px; }" << endl;
		mystyle << "#myDIV{";
		mystyle << "width: 100 % ;";
		mystyle << "padding: 50px 0;";
		mystyle << "text - align: center;";
		mystyle << "background - color: lightblue;";
		mystyle << "margin - top:20px;";
		mystyle << "}";
		mystyle.close();
	}

	//function to create result page containing index of all other files
	inline void PublishFiles::generateresultfile(vector<string> result)
	{
		ofstream myfile;
		myfile.open("../ServerFiles/result.html");
		myfile << "<html>";
		myfile << "<h3>" << "<b>" << "File Index published by Code Publisher!!<br>" << "</b>" << "</h3>";
		for (auto i = result.begin(); i != result.end(); i++)//iterating over vector<string> containing file path/names
		{
			string filename = *i;
			string filenamewohtm = getfilename(filename);
			string link = "";
			link.append("../ServerFiles/");
			link.append(filenamewohtm);
			link.append(".htm");
			myfile << "<head>";
			myfile << "<link rel=" << "stylesheet" << " type=" << "text/css" << " href=" << "../ServerFiles/mystylesheet.css" << ">";
			myfile << "<a href =" << link << ">" << filenamewohtm << " </a>";
			myfile << "<br>";
			myfile << "</head>";
		}
		myfile << "</html>";
		myfile.close();
		string temppath = "../ServerFiles/result.html";
		string command("start \"\" \"" + temppath + "\"");
		system(command.c_str());
	}

	//function to create files with htm extension to publish.All files are created in HtmFolder.
	void PublishFiles::generatehtmlfile(string pathof) {
		string filename = ""; string str;	string path = ""; path = pathof;
		ifstream inputFile(pathof);
		string name = getfilename(path);
		name = "/" + name + ".htm";
		filename.append(directoryname);
		filename.append(name);
		ofstream file(filename);//creating files in HtmFolder
		file << "<html>" << "<head>";
		file << "<link rel=" << "stylesheet" << " type=" << "text/css" << " href=" << "../ServerFiles/mystylesheet.css" << ">";
		file << "<h3>" << "<b>" << "Dependent Files" << "</b>" << "</h3>";	file << "<hr>";
		file << "<script type=\"text/javascript\" src=\"../ServerFiles/HideUnhide.js\"></script>" << std::endl << "</head>";
		//file << "<script type=\"text/javascript\" src=\"../HideUnhide.js\"></script>" << std::endl << "</head>";
		for (auto i = dependentfilesvec.begin(); i != dependentfilesvec.end(); i++) {
			string tempfilename = *i; string name = *i; string path; string tempath;
			size_t pathfound = name.find_last_of("/\\"); string displayfile = "";//removing backslash from path
			tempath.append(name.substr(pathfound + 1));
			size_t found = tempath.find_last_of("."); //removing .htm
			path.append(tempath.substr(0, found));
			file << "<a href =" << tempfilename << ">" << path << " </a>" << "<br>";
		}
		file << "<pre>" << "<body>";
		char next; int count = 0; int id = 0;
		while (inputFile.get(next)) {
			if (next == '<')
				file << "&lt;"; //replacing < with &lt
			else if (next == '>')
				file << "&gt;"; //replacing > with &gt
			else if (next == '{') { //searching for open braces,if found a toggle button is added for the scope
				file << next;
				file << "<button onclick = \"myFunction(" << id << ")\">+</button>";
				file << "<div id =" << id << ">"; id++;
			} //Adding unique div id to each button
			else if (next == '}')
				file << next << "</div>"; //Closing the div tag at close brace
			else
				file << next;
		}
		file << "</body>" << "</pre>" << "</html>";
		inputFile.close(); file.close();
	}

#endif