/*****************************************************
 *	C++ code generated with Nana Creator (0.17.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef PEFORM_H
#define PEFORM_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

//<*includes

//*>


class PEform
	: public nana::form
{
public:
	PEform(nana::window wd = nullptr, const ::nana::size& sz = {640, 480}, const nana::appearance& apr = {true, true, false, false, false, false, false})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor
		caption("Packet Editor");
		notebook1_tbb.activated(0);
		log_type_send_checkbox.check(true);
		log_type_recv_checkbox.enabled(false);
		log_server_auth.check(true);
		log_server_login.check(true);
		log_server_world.check(true);

		packet_listbox.append_header("Server", 70U);
		packet_listbox.append_header("Dir", 50U);
		packet_listbox.append_header("Packet", 320U);

		packet_listbox.sortable(false);


		//*>
	}

	~PEform()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		_place.div("margin=5 gap=2 _field_");
		caption("Form");
		// notebook1
		notebook1_pnl.create(*this);
		notebook1_tbb.create(notebook1_pnl);
		notebook1_plc.bind(notebook1_pnl);
		notebook1_plc.div("vert margin=1 <vert weight=30 tabs><pages>");
		notebook1_plc["tabs"] << notebook1_tbb;
		_place["_field_"] << notebook1_pnl;
		// log_page
		log_page.create(notebook1_pnl);
		log_page_place.bind(log_page);
		log_page_place.div("margin=5 gap=2 arrange=[variable,80] _field_");
		notebook1_tbb.push_back("Log");
		notebook1_tbb.tab_bgcolor(notebook1_tbb.length()-1, log_page.bgcolor());
		notebook1_tbb.tab_fgcolor(notebook1_tbb.length()-1, log_page.fgcolor());
		notebook1_tbb.attach(notebook1_tbb.length()-1, log_page);
		notebook1_plc["pages"].fasten(log_page);
		// log_packet_panel
		log_packet_panel.create(log_page);
		log_packet_panel_place.bind(log_packet_panel);
		log_packet_panel_place.div("vert margin=5 gap=2 arrange=[variable,28] _field_");
		log_page_place["_field_"] << log_packet_panel;
		// packet_listbox
		packet_listbox.create(log_packet_panel);
		log_packet_panel_place["_field_"] << packet_listbox;
		// packet_textbox
		packet_textbox.create(log_packet_panel);
		log_packet_panel_place["_field_"] << packet_textbox;
		packet_textbox.multi_lines(false);
		// log_options_panel
		log_options_panel.create(log_page);
		log_options_panel_place.bind(log_options_panel);
		log_options_panel_place.div("vert weight=80 margin=5 gap=2 arrange=[15%,19%,25%,variable,28,28] _field_");
		log_page_place["_field_"] << log_options_panel;
		// log_type_group
		log_type_group.create(log_options_panel);
		log_type_group.caption_align(static_cast<nana::align>(0));
		log_type_group.div("vert margin=3 gap=2 _field_");
		log_type_group.caption("Type");
		log_options_panel_place["_field_"] << log_type_group;
		// log_type_send_checkbox
		log_type_send_checkbox.create(log_type_group);
		log_type_group["_field_"] << log_type_send_checkbox;
		log_type_send_checkbox.bgcolor(log_type_group.bgcolor());
		log_type_send_checkbox.caption("Send");
		// log_type_recv_checkbox
		log_type_recv_checkbox.create(log_type_group);
		log_type_group["_field_"] << log_type_recv_checkbox;
		log_type_recv_checkbox.bgcolor(log_type_group.bgcolor());
		log_type_recv_checkbox.caption("Recv");
		// log_server_group
		log_server_group.create(log_options_panel);
		log_server_group.caption_align(static_cast<nana::align>(0));
		log_server_group.div("vert margin=3 gap=2 _field_");
		log_server_group.caption("Server");
		log_options_panel_place["_field_"] << log_server_group;
		// log_server_auth
		log_server_auth.create(log_server_group);
		log_server_group["_field_"] << log_server_auth;
		log_server_auth.bgcolor(log_server_group.bgcolor());
		log_server_auth.caption("Auth");
		// log_server_login
		log_server_login.create(log_server_group);
		log_server_group["_field_"] << log_server_login;
		log_server_login.bgcolor(log_server_group.bgcolor());
		log_server_login.caption("Login");
		// log_server_world
		log_server_world.create(log_server_group);
		log_server_group["_field_"] << log_server_world;
		log_server_world.bgcolor(log_server_group.bgcolor());
		log_server_world.caption("World");
		// spam_group
		spam_group.create(log_options_panel);
		spam_group.caption_align(static_cast<nana::align>(0));
		spam_group.div("vert margin=5 gap=2 arrange=[18,20,28] _field_");
		spam_group.caption("Spam");
		log_options_panel_place["_field_"] << spam_group;
		// spam_checkbox
		spam_checkbox.create(spam_group);
		spam_group["_field_"] << spam_checkbox;
		spam_checkbox.bgcolor(spam_group.bgcolor());
		spam_checkbox.caption("Spam");
		// spam_delay_label
		spam_delay_label.create(spam_group);
		spam_group["_field_"] << spam_delay_label;
		spam_delay_label.bgcolor(spam_group.bgcolor());
		spam_delay_label.caption("Delay (ms):");
		// spam_speed_textbox11
		spam_speed_textbox11.create(spam_group);
		spam_group["_field_"] << spam_speed_textbox11;
		spam_speed_textbox11.multi_lines(false);
		// log_options_padding_panel
		log_options_padding_panel.create(log_options_panel);
		log_options_panel_place["_field_"] << log_options_padding_panel;
		// clear_button
		clear_button.create(log_options_panel);
		log_options_panel_place["_field_"] << clear_button;
		clear_button.caption("Clear");
		// inject_button
		inject_button.create(log_options_panel);
		log_options_panel_place["_field_"] << inject_button;
		inject_button.caption("Inject");
		// sendlist_page
		sendlist_page.create(notebook1_pnl);
		notebook1_tbb.push_back("Send list");
		notebook1_tbb.tab_bgcolor(notebook1_tbb.length()-1, sendlist_page.bgcolor());
		notebook1_tbb.tab_fgcolor(notebook1_tbb.length()-1, sendlist_page.fgcolor());
		notebook1_tbb.attach(notebook1_tbb.length()-1, sendlist_page);
		notebook1_plc["pages"].fasten(sendlist_page);

		_place.collocate();
		notebook1_plc.collocate();
		log_page_place.collocate();
		log_packet_panel_place.collocate();
		log_options_panel_place.collocate();
		log_type_group.collocate();
		log_server_group.collocate();
		spam_group.collocate();
	}

// Welp
//protected:
public:
	nana::place _place{ *this };
	nana::panel<true> notebook1_pnl;
	nana::tabbar<size_t> notebook1_tbb;
	nana::place notebook1_plc;
	nana::panel<true> log_page;
	nana::place log_page_place;
	nana::panel<true> log_packet_panel;
	nana::place log_packet_panel_place;
	nana::listbox packet_listbox;
	nana::textbox packet_textbox;
	nana::panel<true> log_options_panel;
	nana::place log_options_panel_place;
	nana::group log_type_group;
	nana::checkbox log_type_send_checkbox;
	nana::checkbox log_type_recv_checkbox;
	nana::group log_server_group;
	nana::checkbox log_server_auth;
	nana::checkbox log_server_login;
	nana::checkbox log_server_world;
	nana::group spam_group;
	nana::checkbox spam_checkbox;
	nana::label spam_delay_label;
	nana::textbox spam_speed_textbox11;
	nana::panel<true> log_options_padding_panel;
	nana::button clear_button;
	nana::button inject_button;
	nana::panel<true> sendlist_page;


	//<*declarations

	//*>
};

#endif //PEFORM_H

