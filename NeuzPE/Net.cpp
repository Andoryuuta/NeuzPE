#include "Net.h"
#include "SigScan.h"
#include "GUI.h"
#include "Util.h"
#include <iostream>
#include <vector>
#include <sstream>

namespace Net {
	extern DoSend_t original_dosend = nullptr;

	extern CDPMng** dp_auth = nullptr;
	extern CDPMng** dp_world = nullptr;
	extern CDPMng** dp_login = nullptr;

	std::string GetNameForCClientSock(CClientSock* sock) {
		auto test_mng = [sock](CDPMng** mng) {
			if (mng != nullptr && (*mng)->m_pDPSock != nullptr && (*mng)->m_pDPSock->m_pSock == sock) {
				return true;
			}
			return false;
		};

		if (test_mng(dp_auth)) {
			return "AUTH";
		}
		else if (test_mng(dp_world)) {
			return "WORLD";
		}
		else if (test_mng(dp_login)) {
			return "LOGIN";
		}

		return "UNK";
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
			return try_get(Net::dp_auth);
		}
		else if (name == "LOGIN") {
			return try_get(Net::dp_login);
		}
		else if (name == "WORLD") {
			return try_get(Net::dp_world);
		}
		return nullptr;
	}



	BOOL __fastcall DoSendHook(CClientSock * pThis, void* EDX, uint8_t* data, uint32_t data_size, uint32_t unk) {
		std::vector<uint8_t> v(data, data + data_size);

#ifdef NEUZPE_DEBUG_LOG
		std::cout << "[Send] -> " << Util::bytes_to_hex_string(v) << std::endl;
#endif

		GUI::Get()->LogPacket(pThis, v);

		return original_dosend(pThis, data, data_size, unk);
	}

	// Does sigscans for the direct play ("dp") globals.
	// Returns false on failure.
	bool InitDPObjects() {
		auto shared_dp_area = SigScan::Scan(0x401000, "B9 ?? ?? ?? ?? C7 ?? ?? ?? ?? ?? FF FF FF FF E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 3D FF");
		if (shared_dp_area == 0) {
#ifdef NEUZPE_DEBUG_LOG
			std::cout << "[ERROR] shared_dp_area scan failed!" << std::endl;
#endif
			return false;
		}

		dp_auth  = (CDPMng**)(shared_dp_area + 0x01);
		dp_login = (CDPMng**)(shared_dp_area + 0x15);
		dp_world = (CDPMng**)(shared_dp_area + 0x1F);

		return true;
	}
};

// Sigscan bytes based off of this function:
/*
seg000:004E1F30                                     unknown_maybe_good_for_aob_scan proc near
seg000:004E1F30                                                                             ; DATA XREF: seg002:00842898↓o
seg000:004E1F30
seg000:004E1F30                                     var_10          = byte ptr -10h
seg000:004E1F30
seg000:004E1F30 83 EC 10                                            sub     esp, 10h
seg000:004E1F33 56                                                  push    esi
seg000:004E1F34 8B F1                                               mov     esi, ecx
seg000:004E1F36 FF 15 B0 A2 82 00                                   call    ds:GetTickCount
seg000:004E1F3C 39 05 50 B9 9B 00                                   cmp     ds:dword_9BB950, eax
seg000:004E1F42 0F 83 B0 00 00 00                                   jnb     loc_4E1FF8
seg000:004E1F48 B9 D0 2E AD 00                                      mov     ecx, offset g_dp_auth ; AOB scan starts here. // B9 ?? ?? ?? ?? C7 ?? ?? ?? ?? ?? FF FF FF FF E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 3D FF
seg000:004E1F4D C7 05 50 B9 9B 00 FF FF FF FF                       mov     ds:dword_9BB950, 0FFFFFFFFh
seg000:004E1F57 E8 54 A2 31 00                                      call    sub_7FC1B0
seg000:004E1F5C B9 40 58 AD 00                                      mov     ecx, offset g_dp_login
seg000:004E1F61 E8 4A A2 31 00                                      call    sub_7FC1B0
seg000:004E1F66 B9 70 2F AD 00                                      mov     ecx, offset g_dp_world
seg000:004E1F6B E8 40 A2 31 00                                      call    sub_7FC1B0
seg000:004E1F70 A1 94 EE 9B 00                                      mov     eax, ds:dword_9BEE94
seg000:004E1F75 3D FF FF FF 7F                                      cmp     eax, 7FFFFFFFh
seg000:004E1F7A 77 1D                                               ja      short loc_4E1F99
seg000:004E1F7C 3D FA 07 00 00                                      cmp     eax, 7FAh
seg000:004E1F81 7E 18                                               jle     short loc_4E1F9B
seg000:004E1F83 A1 90 EE 9B 00                                      mov     eax, ds:dword_9BEE90
seg000:004E1F88 8B 80 E8 1F 00 00                                   mov     eax, [eax+1FE8h]
seg000:004E1F8E 85 C0                                               test    eax, eax
seg000:004E1F90 75 4A                                               jnz     short loc_4E1FDC
seg000:004E1F92 B8 6A B0 82 00                                      mov     eax, offset Source
seg000:004E1F97 EB 46                                               jmp     short loc_4E1FDF
seg000:004E1F99                                     ; ---------------------------------------------------------------------------
seg000:004E1F99
seg000:004E1F99                                     loc_4E1F99:                             ; CODE XREF: unknown_maybe_good_for_aob_scan+4A↑j
seg000:004E1F99 33 C0                                               xor     eax, eax
seg000:004E1F9B
seg000:004E1F9B                                     loc_4E1F9B:                             ; CODE XREF: unknown_maybe_good_for_aob_scan+51↑j
seg000:004E1F9B 68 FA 07 00 00                                      push    7FAh
seg000:004E1FA0 50                                                  push    eax
seg000:004E1FA1 6A 00                                               push    0
seg000:004E1FA3 68 4C C8 82 00                                      push    offset aGettextRangeEr ; "GetText range_error"
seg000:004E1FA8 68 50 C6 82 00                                      push    offset aSMinDMaxDIndex ; "%s min:%d, max:%d, index:%d"
seg000:004E1FAD 68 28 82 8E 00                                      push    offset aNeuz    ; "Neuz"
seg000:004E1FB2 68 50 02 00 00                                      push    250h
seg000:004E1FB7 68 28 C6 82 00                                      push    offset aCFlyffpcProgra_0 ; "c:\\FlyffPC\\Program\\_Common\\Project."...
seg000:004E1FBC 68 1C C6 82 00                                      push    offset aLogRange ; "LOG_RANGE"
seg000:004E1FC1 6A 05                                               push    5
seg000:004E1FC3 8D 4C 24 2C                                         lea     ecx, [esp+3Ch+var_10]
seg000:004E1FC7 E8 B4 19 31 00                                      call    sub_7F3980
seg000:004E1FCC 50                                                  push    eax             ; int
seg000:004E1FCD E8 EE 19 31 00                                      call    sub_7F39C0
seg000:004E1FD2 83 C4 1C                                            add     esp, 1Ch
seg000:004E1FD5 B8 44 C8 82 00                                      mov     eax, offset aError ; "error"
seg000:004E1FDA EB 03                                               jmp     short loc_4E1FDF
seg000:004E1FDC                                     ; ---------------------------------------------------------------------------
seg000:004E1FDC
seg000:004E1FDC                                     loc_4E1FDC:                             ; CODE XREF: unknown_maybe_good_for_aob_scan+60↑j
seg000:004E1FDC 8B 40 04                                            mov     eax, [eax+4]
seg000:004E1FDF
seg000:004E1FDF                                     loc_4E1FDF:                             ; CODE XREF: unknown_maybe_good_for_aob_scan+67↑j
seg000:004E1FDF                                                                             ; unknown_maybe_good_for_aob_scan+AA↑j
seg000:004E1FDF 6A 00                                               push    0
seg000:004E1FE1 6A 00                                               push    0
seg000:004E1FE3 50                                                  push    eax
seg000:004E1FE4 B9 D8 AD 8F 00                                      mov     ecx, offset off_8FADD8
seg000:004E1FE9 E8 72 7D 0A 00                                      call    sub_589D60
seg000:004E1FEE C7 86 2C 11 00 00 01 00 00 00                       mov     dword ptr [esi+112Ch], 1
seg000:004E1FF8
seg000:004E1FF8                                     loc_4E1FF8:                             ; CODE XREF: unknown_maybe_good_for_aob_scan+12↑j
seg000:004E1FF8 83 3D FC B1 8F 00 00                                cmp     ds:dword_8FB1FC, 0
seg000:004E1FFF 75 34                                               jnz     short loc_4E2035
seg000:004E2001 83 BE 2C 11 00 00 00                                cmp     dword ptr [esi+112Ch], 0
seg000:004E2008 74 2B                                               jz      short loc_4E2035
seg000:004E200A B9 D8 AD 8F 00                                      mov     ecx, offset off_8FADD8
seg000:004E200F C7 86 2C 11 00 00 00 00 00 00                       mov     dword ptr [esi+112Ch], 0
seg000:004E2019 E8 A2 7B 0A 00                                      call    sub_589BC0
seg000:004E201E B9 D0 2E AD 00                                      mov     ecx, offset g_dp_auth
seg000:004E2023 E8 88 A1 31 00                                      call    sub_7FC1B0
seg000:004E2028 6A 66                                               push    66h
seg000:004E202A 8B CE                                               mov     ecx, esi
seg000:004E202C E8 FF 7E F3 FF                                      call    sub_419F30
seg000:004E2031 C6 40 36 01                                         mov     byte ptr [eax+36h], 1
seg000:004E2035
seg000:004E2035                                     loc_4E2035:                             ; CODE XREF: unknown_maybe_good_for_aob_scan+CF↑j
seg000:004E2035                                                                             ; unknown_maybe_good_for_aob_scan+D8↑j
seg000:004E2035 B8 01 00 00 00                                      mov     eax, 1
seg000:004E203A 5E                                                  pop     esi
seg000:004E203B 83 C4 10                                            add     esp, 10h
seg000:004E203E C3                                                  retn
seg000:004E203E                                     unknown_maybe_good_for_aob_scan endp
*/
