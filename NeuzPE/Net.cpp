#include "Net.h"
#include "SigScan.h"
#include "GUI.h"
#include "Util.h"
#include <iostream>
#include <vector>
#include <sstream>

namespace Net {
	extern DoSend_t original_dosend = nullptr;
	extern CClientSock* last_used_CClientSock = nullptr;

	extern CDPMng** g_dpCertified = nullptr;
	extern CDPMng** g_DPlay = nullptr;
	extern CDPMng** g_dpLoginClient = nullptr;


	// Does sigscans for the g_DPlay and g_dpLoginClient globals.
	// Returns false on failure.
	bool InitDPObjects() {
		//uint32_t image_base = (uint32_t)GetModuleHandle(NULL);
		auto g_dpCertified_addr = SigScan::Scan(0x401000, "B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 E8 03 00 00 FF 15 ?? ?? ?? ?? EB ??");
		if (g_dpCertified_addr == 0) {
			std::cout << "[ERROR] g_dpCertified scan failed!" << std::endl;
			return false;
		}
		g_dpCertified = (CDPMng**)(g_dpCertified_addr + 1);

		auto g_DPlay_addr = SigScan::Scan(0x401000, "B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D ?? ?? ?? C7 ?? ?? ?? FF FF FF FF FF 15 ?? ?? ?? ?? 6A 01");
		if (g_DPlay_addr == 0) {
			std::cout << "[ERROR] g_DPlay scan failed!" << std::endl;
			return false;
		}
		g_DPlay = (CDPMng**)(g_DPlay_addr + 1);

		auto g_dpLoginClient_addr = SigScan::Scan(0x401000, "B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 3D FF");
		if (g_dpLoginClient_addr == 0) {
			std::cout << "[ERROR] g_dpLoginClient scan failed!" << std::endl;
			return false;
		}
		g_dpLoginClient = (CDPMng**)(g_dpLoginClient_addr + 1);

		return true;
	}

	std::string GetNameForCClientSock(CClientSock* sock) {
		auto test_mng = [sock](CDPMng** mng) {
			if (mng != nullptr && (*mng)->m_pDPSock != nullptr && (*mng)->m_pDPSock->m_pSock == sock) {
				return true;
			}
			return false;
		};

		if (test_mng(g_dpCertified)) {
			return "AUTH";
		}
		else if (test_mng(g_DPlay)) {
			return "WORLD";
		}
		else if (test_mng(g_dpLoginClient)) {
			return "LOGIN";
		}
		else {
			return "UNK";
		}
	}

	CClientSock* GetCClientSockForName(std::string name) {
		auto try_get = [](CDPMng** mng) -> CClientSock* {
			try {
				if (mng != nullptr && (*mng)->m_pDPSock != nullptr && (*mng)->m_pDPSock->m_pSock != nullptr) {
					return (*mng)->m_pDPSock->m_pSock;
				}
				return nullptr;
			}
			catch (...) {
				return nullptr;
			}
		};

		if (name == "AUTH") {
			return try_get(Net::g_dpCertified);
		}
		else if (name == "LOGIN") {
			return try_get(Net::g_dpLoginClient);
		}
		else if (name == "WORLD") {
			return try_get(Net::g_DPlay);
		}
		return nullptr;
	}



	BOOL __fastcall DoSendHook(CClientSock * pThis, void* EDX, uint8_t* data, uint32_t data_size, uint32_t unk) {

		last_used_CClientSock = pThis;
		std::vector<uint8_t> v(data, data + data_size);
		std::cout << "[Send] -> " << Util::bytes_to_hex_string(v) << std::endl;


		/*
		// Old GUI
		{
			nana::internal_scope_guard lock;
			//GUI::Get()->list.at(0).append(bytes_to_hexw_string(v));

			auto cat = GUI::Get()->list.at(0);
			auto pos = cat.size();

			//std::stringstream ss;
			//ss << std::hex << (uint32_t)pThis;
			//cat.push_back(ss.str());
			cat.push_back(GetNameForCClientSock(pThis));
			cat.at(pos).text(1, Util::bytes_to_hex_string(v));
			GUI::Get()->list.scroll(true);
		}
		*/

		// New GUI
		{
			GUI::Get()->LogPacket(pThis, v);
		}

		//std::cout << "DoSendHook sending " << data_size << "bytes!" << std::endl;
		return original_dosend(pThis, data, data_size, unk);
	}
};