#include "TCPClient.h"

TCPClient::TCPClient(unsigned short port) : TCPConnection(port)
{
	destSocket = socket(destination.sin_family, SOCK_STREAM, IPPROTO_TCP);
}

void TCPClient::Connect(const std::string& address)
{
	destination.sin_family = AF_INET;
	inet_pton(destination.sin_family, address.c_str(), &destination.sin_addr);

	int result;
	if ((result = connect(destSocket, (struct sockaddr*)&destination, sizeof(destination))) < 0)
	{
		result = WSAGetLastError();
		Log::LogMessage("Unable to connect", LogLevel::ERR);
		throw;
	}
}
