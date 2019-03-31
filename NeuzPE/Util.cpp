#include "util.h"

#include <Windows.h>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <iterator>

namespace Util {
	// Open new console for c(in/out/err)
	bool OpenConsole()
	{
		if(!AllocConsole())
		{
			return false;
		}

		FILE* cinStream;
		FILE* coutStream;
		FILE* cerrStream;
		if (freopen_s(&cinStream, "CONIN$", "r", stdin) != 0) {
			return false;
		}
		if (freopen_s(&coutStream, "CONOUT$", "w", stdout) != 0) {
			return false;
		}
		if (freopen_s(&cerrStream, "CONOUT$", "w", stderr) != 0) {
			return false;
		}

		return true;
	}

	std::string bytes_to_hex_string(std::vector<uint8_t> data) {
		std::stringstream ss;
		ss << std::hex;
		for (auto&& b : data) {
			ss << std::uppercase << std::setfill('0') << std::setw(2) << (unsigned int)b << " ";
		}
		auto tmp = ss.str();
		tmp.pop_back();
		return tmp;
	}

	std::wstring bytes_to_hex_wstring(std::vector<uint8_t> data) {
		std::wstringstream ss;
		ss << std::hex;
		for (auto&& b : data) {
			ss << std::uppercase << std::setfill(L'0') << std::setw(2) << (unsigned int)b << " ";
		}
		auto tmp = ss.str();
		tmp.pop_back();
		return tmp;
	}

	std::vector<uint8_t> hex_string_to_bytes(std::string str) {
		std::vector<uint8_t> output;
		output.reserve(str.length() / 3); // 2 char hex digit + 1 char space.

		// Tokenize string by spaces.
		std::istringstream iss(str);
		std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

		for (auto&& token : tokens) {
			output.push_back(std::stoi(token, nullptr, 16));
		}

		return output;
	}
};
