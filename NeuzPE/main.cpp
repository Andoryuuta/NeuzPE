#include <windows.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "JMPManager.h"
#include "GUI.h"
#include "Net.h"
#include "util.h"
#include "SigScan.h"

DWORD WINAPI MyFunc(LPVOID lpvParam) {
	Util::OpenConsole();

	if (!Net::InitDPObjects()) {
		using namespace nana;
		form fm;
		msgbox(fm, "Failed sigscan for DP / DirectPlay");
		fm.show();
		exec();
		return 0;
	}

	// Find whichever DPObject is active and hook it, as they all share the same vtable.
	while(true) {
		auto test_hook = [](Net::CDPSock* sock) {
			if (sock != nullptr &&
				sock->m_pSock != nullptr &&
				sock->m_pSock->vt != nullptr && 
				sock->m_pSock->vt->DoSend != nullptr)
			{
				std::cout << "Hooking DoSend at: 0x" << std::hex << sock->m_pSock->vt->DoSend << std::endl;
				
				// Install vtable hook
				Net::original_dosend = sock->m_pSock->vt->DoSend;
				sock->m_pSock->vt->DoSend = (Net::DoSend_t)Net::DoSendHook;
				return true;
			}

			return false;
		};


		if (test_hook((*Net::g_dpCertified)->m_pDPSock) || test_hook((*Net::g_DPlay)->m_pDPSock) || test_hook((*Net::g_dpLoginClient)->m_pDPSock)) {
			break;
		}

		Sleep(250);
	}

	GUI::Get()->Exec();


	return 1;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, MyFunc, 0, 0, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}