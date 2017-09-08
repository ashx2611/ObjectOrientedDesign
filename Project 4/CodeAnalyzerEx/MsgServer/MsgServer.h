////////////////////////////////////////////////////////////////////////
// MsgServer.h- Demonstrates client receive and send message and files
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
/*
* This package implements a server that sends HTTP style 
* files to a client and receives files and messages from client.
*  Required Files:
*  ---------------
* * Required Files:
*   HttpMessage.h, 
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*../Analyzer/Executive.h"
../Publisher/Publish.h"
*
* Public Interfaces:
*  ---------------
*using EndPoint = std::string;--port number of client.
void execute(const size_t TimeBetweenMessages, const size_t NumMessages)--sends file & msgs from server to client
*ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#pragma once
#pragma warning (disable : 4367)
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Analyzer/Executive.h"
#include<direct.h>
#include <string>
#include <iostream>
using namespace Logging;
using namespace Async;
using Show = StaticLogger<1>;
using namespace std;
using namespace Utilities;
//specifies methods to send files and messages from client to server
class ServerSender
{
public:
	using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages);

private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
};
//Specifies functionalities to rreceive files and messsages at server
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	void processcmdline();

private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	int pargc = 0;
	char *pargv[1000];

};