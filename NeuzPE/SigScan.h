// Taken from https://gist.github.com/Andoryuuta/3636c0c6da5044fcd76ebc167deb429f

#pragma once
#include <cstdint>
#include <string>

class SigScan
{
public:
	static uint32_t Scan(uint32_t start_address, std::string sig);
};
