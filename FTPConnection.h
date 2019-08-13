#pragma once

#include <memory>
#include <sstream>
#include <array>
#include <map>
#include <filesystem>
#include <vector>
#include "TCPConnection.h"

class FTPConnection
{
public:
	struct FTPRequest
	{
		std::string command;
		int payloadLength;
		std::unique_ptr<std::istream> payload;
		std::map<char, std::string> args;

		FTPRequest(std::string command, std::unique_ptr<std::istream> payload, const std::map<char, std::string>& args = std::map<char, std::string>());
		std::string GetHeader() const;
		void ReadPayload(std::string& out) const;
	};

	struct FTPResponse
	{
		std::string command;
		int payloadLength;
		std::unique_ptr<std::istream> payload;

		FTPResponse(std::string command, int payloadLength, std::unique_ptr<std::istream> payload);
		void ReadResponse(std::ostream& out) const;
		void ReadResponse(std::string& string) const;
	};

	using Request = FTPConnection::FTPRequest;

	std::unique_ptr<TCPConnection> connection;
	int destSocket;

	//Method: ReceiveMessage
	//Purpose: Protocol for receiving FTP communications/requests
	//Returns: A request with payload that can be read
	FTPResponse ReceiveMessage();

	//Method: Trim
	//Purpose: Removes white spaces from requests
	static void Trim(std::string& str);

	//Method: GetDirDetails
	//Purpose: Retrieves contents of given directory
	//Parameters: The new path to access
	//Returns: A stringstream of directory information
	//Pre-Condition: Path should not include forward slashes
	std::unique_ptr<std::stringstream> GetDirDetails(std::string path);

	void SendMessage(FTPRequest& request);
};