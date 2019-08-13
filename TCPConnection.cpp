#include "TCPConnection.h"

bool TCPConnection::initialized = false;
WSADATA TCPConnection::wsaData = WSADATA();

TCPConnection::TCPConnection(unsigned short port) : port(port)
{
	memset(&destination, 0, sizeof(destination));
	destination.sin_family = AF_INET;
	destination.sin_port = htons(port);

	if (!initialized)
	{
		WSAStartup(MAKEWORD(2, 0), &wsaData);
		initialized = true;
	}
}

void TCPConnection::Send(std::istream & in, const std::string& header, int streamLength) const
{
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	memcpy(buffer, header.c_str(), header.length());

	in.read(buffer + header.length(), BUFFER_SIZE - header.length());
	auto amtRead = (static_cast<int>(in.tellg()) == -1) ? streamLength : in.tellg();
	Send(buffer, header.length() + amtRead);

	while (streamLength - in.tellg() > BUFFER_SIZE)
	{
		memset(buffer, 0, BUFFER_SIZE);
		in.read(buffer, BUFFER_SIZE);

		Send(buffer, BUFFER_SIZE);

		Log::LogMessage("Total Sent: " + std::to_string(header.length() + in.tellg()));
	}

	//Send last message
	if (!in.eof())
	{
		auto remaining = streamLength - in.tellg();
		in.read(buffer, remaining);
		Send(buffer, remaining);
	}
	Log::LogMessage("Sent final message " + std::to_string(header.length() + in.tellg()));
}

void TCPConnection::Send(const char * message, int length) const
{
	while (length > 0)
	{
		int bytesSent = send(destSocket, message, length, 0);
		Log::LogMessage("Sent " + std::to_string(bytesSent) + " bytes", LogLevel::VERBOSE);

		if (bytesSent < 0)
			break;

		message += bytesSent;
		length -= bytesSent;
	}
}

int TCPConnection::Receive(std::ostream& out) const
{
	char buffer[BUFFER_SIZE];
	int bytesRead;

	bytesRead = recv(destSocket, buffer, BUFFER_SIZE, 0);
	Log::LogMessage("Received " + std::to_string(bytesRead) + " bytes", LogLevel::VERBOSE);

	out.write(buffer, bytesRead);

	return bytesRead;
}

TCPConnection::~TCPConnection()
{
	closesocket(destSocket);
	WSACleanup();
}
