#pragma once
#include <string>
#include <cstdint>
#include <vector>

namespace Util {

	// Open new console for c(in/out/err)
	void OpenConsole();

	std::string bytes_to_hex_string(std::vector<uint8_t> data);
	std::wstring bytes_to_hex_wstring(std::vector<uint8_t> data);

	std::vector<uint8_t> hex_string_to_bytes(std::string str);
};