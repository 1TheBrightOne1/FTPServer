#pragma once

#include <fstream>
#include <iostream>
#include <string>

enum LogLevel
{
	ERR,
	WARNING,
	INFO,
	VERBOSE
};

class Log
{
public:
	//Method: LogMessage
	//Purpose: Adds message if it is within log level
	static void LogMessage(const std::string& message, LogLevel level = LogLevel::INFO);
private:
	static std::ostream* log;
	static int level;

	//Method: InitLog
	//Purpose: Reads config file and opens log stream
	static void InitLog();
};