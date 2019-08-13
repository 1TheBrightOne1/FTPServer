#include "TCPServer.h"

TCPServer::TCPServer(unsigned short port) : TCPConnection(port)
{
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr = in4addr_any;

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		Log::LogMessage("Unable to obtain socket " + std::to_string(WSAGetLastError()), LogLevel::ERR);
		throw;
	}

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
		(char *)&reuse, sizeof(reuse)) < 0)
	{
		Log::LogMessage("Error setting reuse " + std::to_string(WSAGetLastError()), LogLevel::ERR);
		throw;
	}
}

void TCPServer::Begin(std::function<void()> callback)
{
	this->callback = callback;

	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		Log::LogMessage("Error on bind " + std::to_string(WSAGetLastError()), LogLevel::ERR);
		throw;
	}

	if (listen(serverSocket, MAX_QUEUE) < 0)
	{
		Log::LogMessage("Error on listen " + std::to_string(WSAGetLastError()), LogLevel::ERR);
		throw;
	}

	while (true)
	{
		Log::LogMessage("Server listening");
		destSocket = accept(serverSocket, (struct sockaddr*)&destination, &destLength);

		callback();

		closesocket(destSocket);
	}
}
