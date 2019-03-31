// Taken from https://gist.github.com/Andoryuuta/3636c0c6da5044fcd76ebc167deb429f

#include "SigScan.h"
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

std::vector<std::string> SpaceSplit(std::string text) {

	std::istringstream iss(text);
	std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
	return results;
}

// Base-16 single char to int.
int char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

uint64_t parseHex(std::string s) {
	uint64_t total = 0;
	uint64_t place_value = 1;
	std::for_each(s.rbegin(), s.rend(), [&total, &place_value](char const& c) {
		total += char2int(c) * place_value;
		place_value *= 16;
	});
	return total;
}

uint32_t SigScan::Scan(uint32_t start_address, const std::string& sig) {
	auto split = SpaceSplit(sig);
	std::vector<uint8_t> sig_bytes;
	std::vector<char> sig_wildcards;

	for (auto&& s : split) {
		if (s == "?" || s == "??") {
			sig_wildcards.push_back('?');
			sig_bytes.push_back(0);
		}
		else {
			sig_wildcards.push_back('x');
			sig_bytes.push_back((uint8_t)parseHex(s));
		}
	}

	// Get the region information.
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)start_address, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	uint8_t* region_end = (uint8_t*)((uint32_t)mbi.BaseAddress + (uint32_t)mbi.RegionSize);

	// HEAVY performance optimization (7x speedup locally)
	auto sbd = sig_bytes.data();
	auto swd = sig_wildcards.data();

	// The 
	auto dataCompare = [](const uint8_t* data, const uint8_t* search_bytes, const char* wildcard_mask)
	{
		for (; *wildcard_mask; ++wildcard_mask, ++data, ++search_bytes)
			if (*wildcard_mask == 'x' && *data != *search_bytes)
				return false;
		return (*wildcard_mask) == NULL;
	};

	for (uint8_t* p = (uint8_t*)start_address; p < region_end; p++) {
		// Try to match at this addr.
		if (dataCompare(p, sbd, swd)) {
			return (uint32_t)p;
		}

	}

	// No match.
	return 0;
}


/*

bool IsGuarded(DWORD protect) {
	return (protect & PAGE_NOACCESS) || (protect & PAGE_GUARD);
}


bool IsExecScannable(DWORD protect) {
	return (protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE));// && !IsGuarded(protect);

}

void SigScan::PrintRegions()
{
	MEMORY_BASIC_INFORMATION mbi;
	LPVOID lpMem = (void*)0;
	while (VirtualQuery(lpMem, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
		lpMem = (LPVOID)((DWORD)mbi.BaseAddress + (DWORD)mbi.RegionSize);
		std::cout << "BaseAddress: 0x" << std::hex << mbi.BaseAddress << ", protect: 0x" << mbi.Protect << std::endl;
	}
}
*/