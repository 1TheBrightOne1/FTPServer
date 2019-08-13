#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <filesystem>

#include "FTPClient.h"
#include "FTPServer.h"

bool CompareFiles(std::ifstream& a, std::ifstream& b)
{
	const int BUFF_SIZE = 128;
	char bufferA[BUFF_SIZE];
	char bufferB[BUFF_SIZE];

	int count = 0;
	while (!a.eof())
	{
		a.read(bufferA, BUFF_SIZE);
		b.read(bufferB, BUFF_SIZE);

		if (memcmp(bufferA, bufferB, BUFF_SIZE) != 0)
		{
			for (int iii = 0; iii < BUFF_SIZE; ++iii)
			{
				if (bufferA[iii] != bufferB[iii])
					std::cout << "Mismatch at " << count * BUFF_SIZE + iii << " " << static_cast<int>(bufferA[iii]) << " " << static_cast<int>(bufferB[iii]) << "\n";
			}
		}

		++count;
	}

	return true;
}

void PrintBytes()
{
	std::string file;
	
	while (true)
	{
		std::cout << "ENTER FILE: ";
		std::cin >> file;

		std::ifstream out(file, std::ios::binary);

		if (out.fail())
			continue;

		while (true)
		{
			std::cout << "WHERE TO? ";

			int pos;
			std::cin >> pos;

			if (pos < 0)
				break;

			out.seekg(pos);

			char buff[128];
			out.read(buff, 128);
			for (int iii = 0; iii < 128; ++iii)
			{
				if (iii % 8 == 0)
					std::cout << std::endl;
				std::cout << static_cast<int>(buff[iii]) << ' ';
			}
			std::cout << std::endl;
		}
	}
}

int main(int argc, char** argv)
{
	std::map<std::string, std::string> args;
	args["-t"] = "client";
	args["-p"] = "5050";

	for (int iii = 1; iii < argc; iii += 2)
	{
		args[argv[iii]] = argv[iii + 1];
	}

	unsigned short port = atoi(args["-p"].c_str());
	if (args["-t"] == "client")
	{
		FTPClient client(args["-a"]);
	}
	else
	{
		FTPServer server(args["-s"]);
	}

	return 0;
}