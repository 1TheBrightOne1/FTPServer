#pragma once

#include <fstream>
#include <sstream>
#include <functional>
#include <future>
#include "TCPServer.h"
#include "FTPConnection.h"

class FTPServer : public FTPConnection
{
public:
	//Method: FTPServer
	//Purpose: Establishes a TCP connection and BLOCKS until client connects
	FTPServer(const std::string& root = "C:/users/kevin.bright/documents");

	//Method: ProcessClient
	//Purpose: Called by TCPConnection when client connects. Handles directory and file transfer requests
	void ProcessClient();
private:
	const std::string root;//For security, cannot access files not encompassed by root
	std::string cursor; //The current directory

	//Method: GetFile
	//Purpose: Loads requested file
	//Parameters: The path to access
	//Return: a file handle to requested file
	//Pre-Condition: Path should not include forward slashes
	std::unique_ptr<std::ifstream> GetFile(const std::string& path);
};