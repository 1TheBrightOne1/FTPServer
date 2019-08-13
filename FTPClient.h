#pragma once

#include <sstream>
#include <map>
#include <functional>
#include "TCPClient.h"
#include "FTPConnection.h"

class FTPClient : public FTPConnection
{
public:
	//Method: FTPClient
	//Purpose: Establishes TCP connection for file transfer. This begins the FTP communication process
	FTPClient(const std::string& address);
private:
	std::map<std::string, std::function<void(Request*)>> commands;
	std::string workingDir;

	//Method: GetUserInput
	//Purpose: Prompts user for command + path
	std::stringstream GetUserInput();

	//Method: PrintCommands
	//Purpose: Displays possible commands to user when prompted for HELP
	void PrintCommands();

	//Method: ParseUserInput
	//Purpose: converts string to valid FTPRequest
	Request ParseUserInput(std::stringstream& input);

	//Method: FileCopy
	//Purpose: Copy file from server
	void FileCopy(const Request& request);

	//Method: PrintResponse
	//Purpose: Prints any response to console
	void PrintResponse(const FTPResponse& request);

	//Method: ShowDirectories
	//Purpose: Shows server and client dirs side by side
	void ShowDirectories(const Request& request);

	//Method: InitCommands
	//Purpose: Initializes map with commands to functions
	void InitCommands();
};