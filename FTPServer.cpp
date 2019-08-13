#include "FTPServer.h"

FTPServer::FTPServer(const std::string& root) : root(root)
{
	connection = std::make_unique<TCPServer>();
	TCPServer* server = reinterpret_cast<TCPServer*>(connection.get());
	server->Begin([this]() {this->ProcessClient(); });
}

void FTPServer::ProcessClient()
{
	this->destSocket = destSocket;
	cursor = root;

	SendMessage(Request("DIR", GetDirDetails("")));
	while (true)
	{
		Log::LogMessage("Waiting for request");
		auto clientRequest = ReceiveMessage();
		std::string payload;
		clientRequest.ReadResponse(payload);
		try
		{
			if (clientRequest.command == "EXPLORE")
			{
				SendMessage(Request("DIR", GetDirDetails(cursor + payload)));
				cursor += '/' + payload;
			}
			else if (clientRequest.command == "GET")
			{
				SendMessage(Request("FILE", GetFile(payload)));
			}
		}
		catch (std::exception& e)
		{
			SendMessage(Request("ERROR", std::make_unique<std::stringstream>(e.what())));
		}
		Log::LogMessage("Finished Responding");
	}
}

std::unique_ptr<std::ifstream> FTPServer::GetFile(const std::string & path)
{
	return std::make_unique<std::ifstream>(cursor + "/" + path, std::ios::binary | std::ios::ate);
}
