#include "Log.h"

std::ostream* Log::log = nullptr;
int Log::level = -1;

void Log::LogMessage(const std::string & message, LogLevel level)
{
	if (log == nullptr)
	{
		InitLog();
	}

	if (level <= Log::level)
		(*log) << message << std::endl;
}

void Log::InitLog()
{
	std::ifstream config("log_level.txt");
	std::string value;
	std::getline(config, value);

	if (value == "file")
		log = new std::ofstream("log.txt");
	else
		log = &std::cout;

	config >> level;
}
