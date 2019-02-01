#pragma once
#include <string>
#include <vector>
#include "Net.h"

#include <nana/gui/widgets/listbox.hpp>
class PacketTemplate
{
public:
	enum class Server {
		WORLD,
		LOGIN,
		AUTH,

		UNK,
	};

	enum class Direction {
		SEND,
		RECV,

		UNK,
	};
	static std::string ServerToString(Server s);
	static Server StringToServer(std::string s);
	static Net::CDPMng* ServerToDPMng(Server s);
	static Server DPMngToServer(Net::CDPMng* dp);
	static std::string DirToString(Direction s);
	static Direction StringToDir(std::string s);


	Server server;
	Direction dir;
	std::vector<uint8_t> data;
	std::vector<bool> random_mask;

	PacketTemplate(Net::CClientSock* sock, std::vector<uint8_t> data, Direction dir);
	PacketTemplate(std::string);
	void ParseString(std::string s);
	const std::string GenerateString();
	std::vector<uint8_t> GenerateBinary();


};

nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const PacketTemplate& pt);