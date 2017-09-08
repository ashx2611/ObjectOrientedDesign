////////////////////////////////////////////////////////////////////////
// MsgClient.h- Demonstrates client receive and send message and files
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
* This package implements a client that sends HTTP style messages and
* files to a server and receives files from server.
*  Required Files:
*  ---------------
* * Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
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
*using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages)--sends file & msgs from client to senver
*ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#pragma once
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Analyzer/Executive.h"
#include"../Publisher/Publish.h"
#include <string>
#include <iostream>
#include <thread>
using namespace Logging;
using namespace Async;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;
class ServerHandler
{
public:
	ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;

};
class MsgClient
{
public:
	using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages);

private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
};
