////////////////////////////////////////////////////////////////////////
// MsgServer.cpp- Demonstrates server side to receive and send message and files
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
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.it also sends html type files to clients repository. It also calls publisher if the 
client asks to publish a file.

/*
*using EndPoint = std::string;
void execute(const size_t TimeBetweenMessages, const size_t NumMessages)--sends file & msgs from server to client
*ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
*
*  Maintanence History:
*  --------------------
*  ver 1.0
*  - first release
* Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp 
*   Utilities.h, Utilities.cpp
*/
#pragma once
#pragma warning (disable : 4367)
#include "MsgServer.h"

///////////////////////////////////////////////////////////////////
///Code for sending from server to client

//----< factory for creating messages >------------------------------
/*
* This function only creates one type of message for this demo.
* - To do that the first argument is 1, e.g., index for the type of message to create.
* - The body may be an empty string.
* - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
*   expects the receiver EndPoint for the toAddr attribute.
*/
HttpMessage ServerSender::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080";
switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("GET", "DOWNLOAD"));
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
	case 2:
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

void ServerSender::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
* - Sends a message to tell receiver a file is coming.
* - Then sends a stream of bytes until the entire file
*   has been sent.
* - Sends in binary mode which works for either text or binary.
*/
bool ServerSender::sendFile(const std::string& filename, Socket& socket)
{
	// assumes that socket is connected

	std::string fqname = "../ServerFiles/" + filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;
	HttpMessage msg = makeMessage(1, "", "localhost::8081");
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
			break;	}
	file.close();
	return true;
}
//----< this defines the behavior of the client >--------------------

void ServerSender::execute(const size_t TimeBetweenMessages, const size_t NumMessages)
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8081))
		{
			Show::write("\n Server waiting to connect");
			::Sleep(100);
		}
		// send a set of messages

		HttpMessage msg;
		std::string msgBody =
				"<msg>Message #" + Converter<size_t>::toString(1) +
				" from server #" + "</msg>";
			msg = makeMessage(1, msgBody, "localhost:8081");
			//
			//* Sender class will nee to accept messages from an input queue
			//* and examine the toAddr attribute to see if a new connection
			//* is needed.  If so, it would either close the existing connection
			//* or save it in a map[url] = socket, then open a new connection.
			//*/
			sendMessage(msg, si);
			Show::write("\n\n  Server sent\n" + msg.toIndentedString());
			::Sleep(TimeBetweenMessages);
			//}
			//  send all *.cpp files from TestFiles folder
			cout << "\n" << "SENDING FILE" << endl;
			std::vector<std::string> files = FileSystem::Directory::getFiles("../ServerFiles", "*.htm");
			for (size_t i = 0; i < files.size(); ++i)
			{
				Show::write("\n\n  sending file " + files[i]);
				sendFile(files[i], si);
			}
			std::vector<std::string> file1 = FileSystem::Directory::getFiles("../ServerFiles", "*.css");
			for (size_t i = 0; i < file1.size(); ++i)
			{
				Show::write("\n\n  sending file " + files[i]);
				sendFile(file1[i], si);
			}
			std::vector<std::string> file2 = FileSystem::Directory::getFiles("../ServerFiles", "*.js");
			for (size_t i = 0; i < file2.size(); ++i)
			{
				Show::write("\n\n  sending file " + files[i]);
				sendFile(file2[i], si);
			}
			// shut down server's client handler

			msg = makeMessage(2, "QUIT", "toAddr:localhost:8081");
			sendMessage(msg, si);
			Show::write("\n\n  Server sent\n" + msg.toIndentedString());

			Show::write("\n");
			Show::write("\n  All done folks");
		//}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//----< this defines processing to frame messages >------------------
HttpMessage ClientHandler::readMessage(Socket& socket)
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
  }  // If client is done, connection breaks and recvString returns empty string
  if (msg.attributes().size() == 0)
  {
    connectionClosed_ = true;
    return msg;
  }
  // read body if POST - all messages in this demo are POSTs
  if (msg.attributes()[0].first == "POST")
  {
	  std::string filename = msg.findValue("file");
		  if (msg.attributes()[0].second == "UPLOAD")
	  {
	  cout << "" << endl;
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
	  else if (msg.attributes()[0].second == "PUBLISH")
	  {
		  cout << "\n" << "Converting  files sent by client to html format at server" << endl;
		  CodeAnalysisExecutive exec;
		  exec.integrate(pargc, pargv);
	  }
	  else if(msg.attributes()[0].second == "DOWNLOAD"){
		  Show::title("Server Sending Files");
		  ServerSender c1;
		  std::thread t1(
		  	[&]() { c1.execute(100, 1); }
		  );
		  t1.join();
	  }
	  else if (msg.attributes()[0].second == "QUIT")
	  {
		  cout << "\n"<<"QUIT" << endl;
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
//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{	std::string fqname = "../ServerFiles/" + filename;	
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
void ClientHandler::operator()(Socket socket)
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

void ClientHandler::processcmdline()
{	
		pargc = 4;
		pargv[1] = "..\\ServerFiles";
		pargv[2] = "*.h";
		pargv[3] = "*.cpp";
}

//----< test stub >--------------------------------------------------
int main()
{
	string directoryname = "../ServerFiles";
	_mkdir(directoryname.c_str());
	::SetConsoleTitle(L"HttpMessage Server - Runs Forever"); 
  Show::attach(&std::cout);
  Show::start();
  Show::title("\n  HttpMessage Server started");
  BlockingQueue<HttpMessage> msgQ;  
  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    ClientHandler cp(msgQ);
	cp.processcmdline();
    sl.start(cp);		
    while (true)
    {
      HttpMessage msg = msgQ.deQ();
      Show::write("\n\n  Server recvd message with body contents:\n" + msg.bodyString());  	 
    }	
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}




