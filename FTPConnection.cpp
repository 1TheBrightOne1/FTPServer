#include "FTPConnection.h"

FTPConnection::FTPResponse FTPConnection::ReceiveMessage()
{
	//Get first read and size of read
	auto ptrStream = std::make_unique<std::stringstream>();
	std::stringstream& stream = *ptrStream.get();
	auto streamSize = connection.get()->Receive(stream);

	std::string command;
	stream >> command;

	int length;
	stream >> length;

	int remaining = length;
	stream >> std::ws;

	int headerLength = command.length() + std::to_string(length).length() + 2;

	Log::LogMessage("Processing Message: " + command + " Length: " + std::to_string(length));

	std::ostream* write;
	std::ofstream fileHandle;

	//If we didn't read the whole message and it's a large enough message we'll write it to a file
	if (headerLength > streamSize && length >= 1024 * 100)
	{
		fileHandle.open("temp.bin", std::ios::binary);
		write = &fileHandle;
	}
	else
	{
		write = &stream;
	}

	//Copy remaining message into buffer (excluding the header we have read)
	char buffer[TCPConnection::BUFFER_SIZE];
	memset(buffer, 0, TCPConnection::BUFFER_SIZE);

	stream.read(buffer, streamSize - headerLength);

	//Reset string stream before potentially writing payload to it
	stream.str("");
	stream.clear();

	(*write).write(buffer, streamSize - headerLength);

	remaining -= static_cast<int>((*write).tellp());

	//read remaining message
	auto size = (*write).tellp();
	while (remaining > 0)
	{
		remaining -= connection.get()->Receive(*write);
		Log::LogMessage("Total Read " + std::to_string((*write).tellp()));
	}

	if (command == "ERROR")
	{
		throw "ERROR response received " + stream.str();
	}


	if (stream.str().length() == 0)
		return FTPResponse(command, length, std::make_unique<std::ifstream>("temp.bin", std::ios::binary));
	else
		return FTPResponse(command, length, std::move(ptrStream));
}

void FTPConnection::Trim(std::string & str)
{
	static std::string WHITESPACE = " \n\r\t\f\v";
	auto boundary = str.find_first_not_of(WHITESPACE);
	str = (boundary == std::string::npos) ? "" : str.substr(boundary);

	boundary = str.find_last_not_of(WHITESPACE);
	str = (boundary == std::string::npos) ? "" : str.substr(0, boundary + 1);
}

std::unique_ptr<std::stringstream> FTPConnection::GetDirDetails(std::string requestedPath)
{
	using namespace std::experimental::filesystem;

	std::vector<std::string> dirs;
	std::vector<std::string> files;

	path winPath(requestedPath);

	for (directory_iterator next(winPath), end; next != end; ++next)
	{
		if (next->status().type() == v1::file_type::directory)
		{
			dirs.push_back(next->path().filename().string());
		}
		else
		{
			files.push_back(next->path().filename().string());
		}
	}

	if (dirs.size() == 0 && files.size() == 0)
	{
		throw std::exception("No files exist at specified path");
	}

	auto contents = std::make_unique<std::stringstream>();
	for (const auto& dir : dirs)
		*contents.get() << dir << "\n";
	for (const auto& file : files)
		*contents.get() << file << "\n";

	return contents;
}

void FTPConnection::SendMessage(FTPRequest & request)
{
	Log::LogMessage("Sending Message: " + request.GetHeader());
	connection.get()->Send(*request.payload.get(), request.GetHeader(), request.payloadLength);
}

FTPConnection::FTPResponse::FTPResponse(std::string command, int payloadLength, std::unique_ptr<std::istream> payload) : command(command), payloadLength(payloadLength), payload(std::move(payload))
{
}

void FTPConnection::FTPResponse::ReadResponse(std::ostream & out) const
{
	auto remaining = payloadLength;
	char buffer[TCPConnection::BUFFER_SIZE];

	while (remaining > TCPConnection::BUFFER_SIZE)
	{
		payload.get()->read(buffer, TCPConnection::BUFFER_SIZE);
		out.write(buffer, TCPConnection::BUFFER_SIZE);
		remaining -= TCPConnection::BUFFER_SIZE;
	}

	if (remaining > 0)
	{
		payload.get()->read(buffer, remaining);
		out.write(buffer, remaining);
	}
}

void FTPConnection::FTPResponse::ReadResponse(std::string & out) const
{
	out = "";
	while (!payload.get()->eof())
	{
		char buffer[TCPConnection::BUFFER_SIZE];
		memset(buffer, 0, TCPConnection::BUFFER_SIZE);
		payload.get()->read(buffer, TCPConnection::BUFFER_SIZE - 1);
		out += buffer;
	}
}

FTPConnection::FTPRequest::FTPRequest(std::string command, std::unique_ptr<std::istream> payload, const std::map<char, std::string>& args) : command(command), payload(std::move(payload)), args(args), payloadLength(payloadLength)
{
	this->payload.get()->seekg(0, std::ios::end);
	this->payloadLength = static_cast<int>(this->payload.get()->tellg());
	this->payload.get()->seekg(0);
}

std::string FTPConnection::FTPRequest::GetHeader() const
{
	return command + " " + std::to_string(payloadLength) + " ";
}

void FTPConnection::FTPRequest::ReadPayload(std::string & out) const
{
	out = "";
	payload.get()->clear();
	payload.get()->seekg(0);
	while (!payload.get()->eof())
	{
		char buffer[TCPConnection::BUFFER_SIZE];
		memset(buffer, 0, TCPConnection::BUFFER_SIZE);
		payload.get()->read(buffer, TCPConnection::BUFFER_SIZE - 1);
		out += buffer;
	}
}
