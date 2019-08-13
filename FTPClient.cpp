#include "FTPClient.h"

FTPClient::FTPClient(const std::string& address)
{
	connection = std::make_unique<TCPClient>();
	TCPClient* client = reinterpret_cast<TCPClient*>(connection.get());
	client->Connect(address);

	//Receive initial directory
	auto response = ReceiveMessage();

	PrintResponse(response);
	while (true)
	{
		auto request = ParseUserInput(GetUserInput());
		SendMessage(request);

		if (request.command == "GET")
		{
			FileCopy(request);
			//auto response = ReceiveMessage();
			//response.get()->ReadPayload(std::ofstream("C:/users/kevin.bright/downloads/newfile.html"));
		}
		else
		{
			auto response = ReceiveMessage();
			PrintResponse(response);
		}
	}
}

std::stringstream FTPClient::GetUserInput()
{
	std::string input;
	std::cout << "Please enter a command: ";
	std::getline(std::cin, input);

	return std::stringstream(input);
}

void FTPClient::PrintCommands()
{
	std::cout << "EXPLORE [path]\nGET [file]\n";
}

FTPConnection::FTPRequest FTPClient::ParseUserInput(std::stringstream & input)
{
	std::string command;
	input >> command;

	std::string payload;
	std::getline(input, payload, '-');
	Trim(payload);

	std::map<char, std::string> args;
	while (!input.eof())
	{
		char arg;
		input >> arg;

		std::string param;
		std::getline(input, param, '-');
		Trim(param);
	}
	
	return Request(command, std::make_unique<std::stringstream>(payload), args);
}

void FTPClient::FileCopy(const Request& request)
{
	std::string saveLocation;
	request.ReadPayload(saveLocation);
	auto response = ReceiveMessage();
	if (request.args.find('l') != request.args.cend())
	{
		saveLocation = request.args.at('l');
	}

	std::ofstream out(saveLocation, std::ios::binary);
	response.ReadResponse(out);
}

void FTPClient::PrintResponse(const FTPResponse& response)
{
	std::string message;
	response.ReadResponse(message);
	std::cout << message;
}

void FTPClient::ShowDirectories(const Request & request)
{
}

void FTPClient::InitCommands()
{

}
