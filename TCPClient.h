#pragma once

#include "TCPConnection.h"

class TCPClient : public TCPConnection
{
public:
	//Method: TCPClient
	//Purpose: Initializes TCP connection (but does not start)
	TCPClient(unsigned short port = 5050);

	//Method: Connect
	//Purpose: Begins TCP session with server
	//Parameters: An IPv4 address
	void Connect(const std::string& address);
};