#pragma once

#include <atomic>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>

#include "PEform.h"
#include "net.h"

using namespace nana;
class GUI
{

private:
	static GUI* instance;
	GUI();

public:
	static GUI* Get();
	void Exec();
	void LogPacket(Net::CClientSock * sock, std::vector<uint8_t> pkt);

public:
	form fm;
	listbox list;
	button btn;
	textbox text;
	place layout;

	PEform pfm;

	//
	std::atomic<bool> spamming_packet;

};

