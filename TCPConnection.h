#pragma once

#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Ws2ipdef.h>
#include "Log.h"

class TCPConnection
{
public:
	static const unsigned int BUFFER_SIZE = 1024 * 20; //Used by higher level application layer

	//Method: TCPConnection
	//Purpose: Initializes (but does not start) a TCPConnection
	TCPConnection(unsigned short port = 5050);

	//Method: Send
	//Purpose: Sends a stream of data through connection
	//Parameters: The stream to send and a header (if needed). Header will be prepended to stream
	void Send(std::istream& in, const std::string& header, int streamLength) const;

	//Method: Send
	//Purpose: Sends a short message through connection
	void Send(const char* message, int length) const;

	//Method: Receive
	//Purpose: Receives a message of Max Len BUFFER_SIZE. Multiple calls may be needed
	//Parameters: The stream to write to
	//Returns: bool, if the connection encountered an error/terminated
	int Receive(std::ostream& out) const;
	virtual ~TCPConnection();

protected:
	struct sockaddr_in destination;
	int destSocket;
	int destLength = sizeof(destination);
	unsigned short port;

private:
	static WSADATA wsaData;
	static bool initialized; //So that WSADATA is only init once
};