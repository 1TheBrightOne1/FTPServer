#pragma once

#include <functional>
#include "TCPConnection.h"

class TCPServer : public TCPConnection
{
public:
	//Method: TCPServer
	//Purpose: Prepares (but does not start) TCP connection
	TCPServer(unsigned short port = 5050);

	//Method: Begin
	//Purpose: Tell server to start listening
	//Parameters: A callback to the application to handle connections
	void Begin(std::function<void()> callback);
private:
	int serverSocket;
	int reuse = 1; //For SETSOCKOPT
	std::function<void()> callback;
	struct sockaddr_in serverAddress;
	static const unsigned int MAX_QUEUE = 10;
};