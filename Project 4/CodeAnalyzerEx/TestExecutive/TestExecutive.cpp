#include "TestExecutive.h"
#include<iostream>
using namespace std;

void TestExecutive::demonstrateRequirements()
{
	cout << "******************Requirement 2*******************" << endl;
	cout << "Used C++ standard library's streams for all console I/O and new and delete for all heap-based memory management." << endl;
	cout << "******************Requirement 3*******************" << endl;
	cout << "Demonstrated functionality to publish, as linked web pages, the contents of a set of C++ source code files."
		<< "The html files published could be found in client's folder that is 'Test files'"<< endl;
	cout << "******************Requirement 4*******************" << endl;
	cout << "Demonstrated Publishing using CodePublisher developed in Project 3" << endl;
	cout << "******************Requirement 5*******************" << endl;
	cout << "Demonstrated client program that can upload files, and view Repository contents" << endl
		<< "Here client sends file from TestFiles to ServerFiles folder"<<endl;
	cout << "******************Requirement 6*******************" << endl;
	cout << "Demonstrated  message-passing communication system, based on Sockets, used to access the serverFiles(Repository) folder from another process or machine" << endl;
	cout << "******************Requirement 7*******************" << endl;
	cout << "Demonstrated communication system to provide support for passing HTTP style messages using either synchronous request/response or asynchronous one-way messaging" << endl;
	cout << "******************Requirement 8*******************" << endl;
	cout << "Demonstrated communication system that supports sending and receiving of file streams " << endl;
	cout << "******************Requirement 9*******************" << endl;
	cout << "Demonstrated the requirements met by the project in the test executive"<<endl;


}

int main() {
	TestExecutive exec;
	exec.demonstrateRequirements();
}

