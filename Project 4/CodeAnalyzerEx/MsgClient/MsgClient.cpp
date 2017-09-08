
////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates client receive and send message and files
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
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
*
*/
#pragma once
#pragma warning (disable : 4367)
#include "MsgClient.h"
using namespace Logging;
using namespace Async;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
class ClientCounter
{
public:
  ClientCounter() { ++clientCount; }
  size_t count() { return clientCount; }
private:
  static size_t clientCount;
};

size_t ClientCounter::clientCount = 0;
//Defines msg received and msg processing from server
HttpMessage ServerHandler::readMessage(Socket& socket)
{
	connectionClosed_ = false;
	HttpMessage msg;

	// read message attributes

	while (true)
	{
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1)
		{
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
		{
			break;
		}
	}
	// If client is done, connection breaks and recvString returns empty string

	if (msg.attributes().size() == 0)
	{
		connectionClosed_ = true;
		return msg;
	}
	// read body if POST - all messages in this demo are POSTs

	if (msg.attributes()[0].first == "GET")
	{
		std::string filename = msg.findValue("file");
		//if client wants to download files then download
		if (msg.attributes()[0].second == "DOWNLOAD")		{
			
			if (filename != "")
			{
				size_t contentSize;
				std::string sizeString = msg.findValue("content-length");
				if (sizeString != "")
					contentSize = Converter<size_t>::toValue(sizeString);
				else
					return msg;
				readFile(filename, contentSize, socket);
			}

			if (filename != "")
			{
				msg.removeAttribute("content-length");
				std::string bodyString = "<file>" + filename + "</file>";
				std::string sizeString = Converter<size_t>::toString(bodyString.size());
				msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
				msg.addBody(bodyString);
			}
		}
			if (filename == "")
			{
				
				size_t numBytes = 0;
				size_t pos = msg.findAttribute("content-length");
				if (pos < msg.attributes().size())
				{
					numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
					Socket::byte* buffer = new Socket::byte[numBytes + 1];
					socket.recv(numBytes, buffer);
					buffer[numBytes] = '\0';
					std::string msgBody(buffer);
					msg.addBody(msgBody);
					delete[] buffer;
				}			
			}		
	}

	return msg;
}

bool ServerHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::string fqname = "../TestFiles/" + filename;// +".snt";

	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		Show::write("\n\n  can't open file " + fqname);
		return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return true;
}
//----< receiver functionality is defined by this function >---------

void ServerHandler::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "QUIT")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 */
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
  HttpMessage msg;
  HttpMessage::Attribute attrib;
  EndPoint myEndPoint = "localhost:8081";  
  switch (n)
  {
  case 1:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("POST", "UPLOAD"));
    msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
    msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
    msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

    msg.addBody(body);
    if (body.size() > 0)
    {
      attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
      msg.addAttribute(attrib);
    }
    break;
//Defining publish msg
  case 2:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("POST", "PUBLISH"));
	  msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
	  msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	  msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

	  msg.addBody(body);
	  if (body.size() > 0)
	  {
		  attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
		  msg.addAttribute(attrib);
	  }
	  break;
  case 3://defining download msg
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("POST", "DOWNLOAD"));
	  msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
	  msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	  msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

	  msg.addBody(body);
	  if (body.size() > 0)
	  {
		  attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
		  msg.addAttribute(attrib);
	  }
	  break;
  case 4://defining quit msg
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("POST", "QUIT"));
	  msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
	  msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	  msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

	  msg.addBody(body);
	  if (body.size() > 0)
	  {
		  attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
		  msg.addAttribute(attrib);
	  }
	  break;	   
  default:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	break;
  }
  return msg;
}
//----< send message using socket >----------------------------------

void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
  std::string msgString = msg.toString();
  socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent*/
bool MsgClient::sendFile(const std::string& filename, Socket& socket)
{
  // assumes that socket is connected
  std::string fqname = "../TestFiles/" + filename;
  FileSystem::FileInfo fi(fqname);
  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);
  FileSystem::File file(fqname);
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;  
  HttpMessage msg = makeMessage(1, "", "localhost::8080");
  msg.addAttribute(HttpMessage::Attribute("file", filename));
  msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
  sendMessage(msg, socket);
  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];
  while (true)
  {
    FileSystem::Block blk = file.getBlock(BlockSize);
    if (blk.size() == 0)
      break;
    for (size_t i = 0; i < blk.size(); ++i)
      buffer[i] = blk[i];
    socket.send(blk.size(), buffer);
    if (!file.isGood())
      break;
  }
  file.close();
  return true;
}
//----< this defines the behavior of the client >--------------------
void MsgClient::execute(const size_t TimeBetweenMessages, const size_t NumMessages)
{
   ClientCounter counter;
  size_t myCount = counter.count();
  std::string myCountString = Utilities::Converter<size_t>::toString(myCount);
  Show::attach(&std::cout);
  Show::start();
  Show::title(
    "Starting HttpMessage client" + myCountString + 
    " on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
  );
  try
  {
    SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 8080))
    {
      Show::write("\n client waiting to connect");
      ::Sleep(100);
    }    
     HttpMessage msg;
	for (size_t i = 1; i <= NumMessages; ++i)
	{
		std::string msgBody =
			"<msg>Message #" + Converter<size_t>::toString(i) +
			" from client #" + myCountString + "</msg>";
		msg = makeMessage(i, msgBody, "localhost:8080");
		sendMessage(msg, si);
		Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
		::Sleep(TimeBetweenMessages);
		//  send files only if client wants to upload
		if (msg.attributes()[0].second == "UPLOAD")
		{//get all files from client
			std::vector<std::string> files = FileSystem::Directory::getFiles("../TestFiles", "*.*");
			for (size_t i = 0; i < files.size(); ++i)
			{
				Show::write("\n\n  sending file " + files[i]);
				sendFile(files[i], si);
			}
		}
	}

    // shut down server's client handler
    msg = makeMessage(4, "QUIT", "toAddr:localhost:8080");
    sendMessage(msg, si);
    Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
    Show::write("\n");
    Show::write("\n  All done folks");
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}

//----< entry point - runs two clients each on its own thread >------
int main()
{
  ::SetConsoleTitle(L"Clients Running on Threads");
  Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
  MsgClient c1;
  std::thread t1(
    [&]() { c1.execute(100, 4); } // 4 messages 100 millisec apart
  );
  MsgClient c2;
  std::thread t2(
   [&]() { c2.execute(200, 4); } // 4 messages 200 millisec apart
  );
  t1.join();
  t2.join();
  BlockingQueue<HttpMessage> msgQ;
  try
  {
	  SocketSystem ss;
	  SocketListener sl(8081, Socket::IP6);
	  ServerHandler cp(msgQ);
	  sl.start(cp);
	  while (true)
	  {
		  HttpMessage msg = msgQ.deQ();
		  Show::write("\n\n  client recvd message with body contents:\n" + msg.bodyString());
	  }
  }
  catch (std::exception& exc)
  {
	  Show::write("\n  Exeception caught: ");
	  std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
	  Show::write(exMsg);
  }
}

