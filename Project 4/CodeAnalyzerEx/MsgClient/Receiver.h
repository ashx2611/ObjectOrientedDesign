#pragma once

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Analyzer/Executive.h"
#include<direct.h>
//#include"../Publisher/Publish.h"

#include <string>
#include <iostream>
using namespace Logging;
using namespace Async;
using Show = StaticLogger<1>;
using namespace std;
using namespace Utilities;

class ClientReceiver
{
public:
	ClientReceiver(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	void processcmdline(int argc, char *argv[]);

private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	

};
