#include "GUI.h"

#include <iostream>
#include "util.h"
#include "Net.h"
#include "PacketTemplate.h"

GUI::GUI()
{
	/*
	fm.caption("FlyFF Packet Editor prototype");
	list.create(fm);
	//list.enable_single(true, false);
	list.show_header(true);
	list.append_header("Server");
	list.append_header("Packet");

	text.create(fm);
	text.line_wrapped(false);
	text.multi_lines(false);

	btn.create(fm);
	btn.caption("Send");

	layout.bind(fm);
	layout.div("vert<lb weight=75%><vert<textbox><sendbtn max=100>>");
	layout.field("lb") << list;
	layout.field("textbox") << text;
	layout.field("sendbtn") << btn;
	layout.collocate();
	
	// Select packet from list
	list.events().selected([this](const arg_listbox& arg) {
		this->text.reset(arg.item.text(1));
	});

	// Send
	btn.events().click([this]
	{
		auto optstr = this->text.getline(0);
		if (optstr.has_value()) {
			auto pkt_to_send = Util::hex_string_to_bytes(optstr.value());
			std::cout << "Gonna send: " << Util::bytes_to_hex_string(pkt_to_send) << std::endl;
			Net::original_dosend(Net::last_used_CClientSock, pkt_to_send.data(), pkt_to_send.size(), 1);
		}
		//Todo: insert
	});
	fm.show();
	*/




	// New GUI:
	// Select packet from list
;	pfm.packet_listbox.events().selected([this](const arg_listbox& arg) {
		const PacketTemplate* ptr = arg.item.value_ptr<PacketTemplate>();
		this->pfm.packet_textbox.reset(const_cast<PacketTemplate*>(ptr)->GenerateString());
	});

	// Clear button
	pfm.clear_button.events().click([this] {
		this->pfm.packet_listbox.clear();
	});

	// Send
	pfm.inject_button.events().click([this]
	{
		auto optstr = this->pfm.packet_textbox.getline(0);
		if (optstr.has_value()) {
			auto pt = PacketTemplate(optstr.value());
			auto pkt_to_send = pt.GenerateBinary();
			auto sock = Net::GetCClientSockForName(PacketTemplate::ServerToString(pt.server));
			if (sock != nullptr) {
				std::cout << "NEWGUI: Gonna send: " << Util::bytes_to_hex_string(pkt_to_send) << std::endl;
				Net::original_dosend(sock, pkt_to_send.data(), pkt_to_send.size(), 1);
			}

			/*
			auto pkt_to_send = Util::hex_string_to_bytes(optstr.value());
			std::cout << "Gonna send: " << Util::bytes_to_hex_string(pkt_to_send) << std::endl;
			Net::original_dosend(Net::last_used_CClientSock, pkt_to_send.data(), pkt_to_send.size(), 1);
			*/
		}
		//Todo: insert
	});


	pfm.show();
}

GUI* GUI::instance = nullptr;
GUI* GUI::Get()
{
	if(instance == nullptr){
		instance = new GUI();
	}
	return instance;
}


void GUI::Exec() {
	exec();
}

// Checks if a nana::listbox is scrolled to the bottom
// From "Error Flynn" @ http://nanapro.org/en-us/forum/index.php?u=/topic/1098/ggget-listbox-current-scroll
bool scroll_at_bottom(const nana::listbox &lb)
{
	auto cat_proxy = lb.at(lb.size_categ() - 1);
	if (cat_proxy.size() > 0)
	{
		return cat_proxy.back().displayed();
	}
	else
	{
		return true;
	}
}

void GUI::LogPacket(Net::CClientSock * sock, std::vector<uint8_t> pkt) {


	auto pfm = &GUI::Get()->pfm;

	// Check if we should log this packet, early return if not.
	auto servType = PacketTemplate::StringToServer(Net::GetNameForCClientSock(sock));
	switch (servType) {
	case PacketTemplate::Server::AUTH:
		if (!pfm->log_server_auth.checked()) {
			return;
		}
		break;
	case PacketTemplate::Server::LOGIN:
		if (!pfm->log_server_login.checked()) {
			return;
		}
		break;
	case PacketTemplate::Server::WORLD:
		if (!pfm->log_server_world.checked()) {
			return;
		}
		break;
	}

	bool previously_at_bottom = scroll_at_bottom(pfm->packet_listbox);

	// Make my object
	PacketTemplate pt = PacketTemplate(sock, pkt, PacketTemplate::Direction::SEND);

	// Append object to the listbox
	auto cat = pfm->packet_listbox.at(0);
	cat.append(pt, true);

	// Scroll to bottom only if we were previously at the bottom
	if (previously_at_bottom) {
		pfm->packet_listbox.scroll(true);
	}

}