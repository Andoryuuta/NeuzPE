#include <windows.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "GUI.h"
#include "Net.h"
#include "util.h"
#include "SigScan.h"

DWORD WINAPI MyFunc(LPVOID lpvParam) {
#ifdef NEUZPE_DEBUG_LOG
	Util::OpenConsole();
#endif

	// Sigscan for the global DPObjects which handle networking.
	if (!Net::InitDPObjects()) {
		using namespace nana;
		form fm;
		fm.events().unload(
			msgbox(fm, "Failed sigscan for DP / DirectPlay").icon(msgbox::icon_error)<<"Failed sigscan for DP / DirectPlay"
		);
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
#ifdef NEUZPE_DEBUG_LOG
				std::cout << "Hooking " << Net::GetNameForCClientSock(sock->m_pSock) << "->DoSend at: 0x" << std::hex << sock->m_pSock->vt->DoSend << std::endl;
#endif
				
				// Install vtable hook
				Net::original_dosend = sock->m_pSock->vt->DoSend;
				sock->m_pSock->vt->DoSend = (Net::DoSend_t)Net::DoSendHook;
				return true;
			}

			return false;
		};


		if (test_hook((*Net::dp_auth)->m_pDPSock) || test_hook((*Net::dp_world)->m_pDPSock) || test_hook((*Net::dp_login)->m_pDPSock)) {
			break;
		}

		Sleep(250);
	}

	GUI::Get()->Exec();


	return 1;
}


BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved
){
	switch (fdwReason)
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