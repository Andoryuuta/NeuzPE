#include "PacketTemplate.h"
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include "util.h"



nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const PacketTemplate& pt)
{
	return ores << PacketTemplate::ServerToString(pt.server) << PacketTemplate::DirToString(pt.dir) << Util::bytes_to_hex_string(pt.data);
}

PacketTemplate::PacketTemplate(Net::CClientSock* sock, std::vector<uint8_t> data, PacketTemplate::Direction dir)
{
	this->server = StringToServer(Net::GetNameForCClientSock(sock));
	this->dir = dir;
	this->data = data;
	this->random_mask.resize(this->data.size(), false);
}

PacketTemplate::PacketTemplate(std::string s)
{
	ParseString(s);
}

void PacketTemplate::ParseString(std::string str) {
	// str example:
	// [WORLD] [SEND] 05 23 12 5E 2C

	try {
		// Erase [ and ]
		str.erase(std::remove(str.begin(), str.end(), '['), str.end());
		str.erase(std::remove(str.begin(), str.end(), ']'), str.end());

		// Tokenize string by spaces.
		std::istringstream iss(str);
		std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

		std::vector<uint8_t> output;
		std::vector<bool> mask;
		Server oserv;
		Direction odir;

		for (auto&& token : tokens) {
			if (token.length() <= 2) {
				// Random byte
				if (token == "?" || token == "??") {
					output.push_back(0);
					mask.push_back(true);
				}
				else {
					output.push_back(std::stoi(token, nullptr, 16));
					mask.push_back(false);
				}
			}
			else {
				auto d = StringToDir(token);
				auto s = StringToServer(token);
				if (d != Direction::UNK) {
					odir = d;
				}
				if (s != Server::UNK) {
					oserv = s;
				}
			}
		}

		this->data = output;
		this->random_mask = mask;
		this->server = oserv;
		this->dir = odir;
	}
	catch (...) {
		throw std::invalid_argument("Improperly formatted packet string!");
	}
}

const std::string PacketTemplate::GenerateString(){

	// str example:
	// [WORLD] [SEND] 05 23 12 5E 2C
	std::stringstream ss;
	ss << "[" << ServerToString(this->server) << "] [" << DirToString(this->dir) << "] ";

	ss << std::hex;
	for (size_t i = 0; i < this->data.size(); i++) {
		if (this->random_mask[i]) {
			ss << "??";
		}
		else {
			ss << std::uppercase << std::setfill('0') << std::setw(2) << (unsigned int)this->data[i];
		}
		ss << " ";
	}

	auto tmp = ss.str();
	tmp.pop_back();
	return tmp;
}


std::vector<uint8_t> PacketTemplate::GenerateBinary() {
	// Create a vector and reserve the space needed
	std::vector<uint8_t> output;
	output.reserve(this->data.size());

	// Random number generator
	std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> rng_dist(0, 255);

	// Go over the data and random mask, generating random bytes as needed.
	for (size_t i = 0; i < this->data.size(); i++) {
		if (this->random_mask[i]) {
			output.push_back((uint8_t)rng_dist(rng));
		}
		else {
			output.push_back(data[i]);
		}
	}

	return output;
}


std::string PacketTemplate::ServerToString(PacketTemplate::Server s) {
	switch (s) {
	case Server::AUTH:
		return "AUTH";
	case Server::LOGIN:
		return "LOGIN";
	case Server::WORLD:
		return "WORLD";
	default:
		return "UNK";
	}
}

PacketTemplate::Server PacketTemplate::StringToServer(std::string s) {
	if (s == "AUTH") {
		return Server::AUTH;
	}
	else if (s == "LOGIN") {
		return Server::LOGIN;
	}
	else if (s == "WORLD") {
		return Server::WORLD;
	}

	return Server::UNK;
}

Net::CDPMng* PacketTemplate::ServerToDPMng(PacketTemplate::Server s) {
	switch (s) {
	case Server::AUTH:
		return *Net::dp_auth;
	case Server::LOGIN:
		return *Net::dp_login;
	case Server::WORLD:
		return *Net::dp_world;
	default:
		return nullptr;
	}
}

PacketTemplate::Server PacketTemplate::DPMngToServer(Net::CDPMng* dp) {
	if (dp == *Net::dp_auth) {
		return Server::AUTH;
	}
	else if (dp == *Net::dp_login) {
		return Server::LOGIN;
	}
	else if (dp == *Net::dp_world) {
		return Server::WORLD;
	}

	// Default to world;
	return Server::UNK;
}

std::string PacketTemplate::DirToString(PacketTemplate::Direction s) {
	switch (s) {
	case Direction::SEND:
		return "SEND";
	case Direction::RECV:
		return "RECV";

	default:
		return "UNK";
	}
}

PacketTemplate::Direction PacketTemplate::StringToDir(std::string s) {
	if (s == "SEND") {
		return Direction::SEND;
	}
	else if (s == "RECV") {
		return Direction::RECV;
	}

	return Direction::UNK;
}