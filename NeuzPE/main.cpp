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


//JMP* FLSendJmp = nullptr;

bool firstCall = true;
/*
BOOL __fastcall FLSendHook(CDPSock *pThis, void* EDX, uint8_t*data, uint32_t data_size, uint32_t unk) {
	std::cout << "FLSendHook2: CDPSock @0x" << std::hex << (uint32_t)pThis << std::endl;

	// Install VTable hook
	original_dosend = pThis->m_pSock->vt->DoSend;
	pThis->m_pSock->vt->DoSend = (DoSend_t)DoSendHook;

	// Restore native bytes so that gameguard doesn't detect us.
	FLSendJmp->Restore();
	delete FLSendJmp;

	// Perform the original native functionality.
	return pThis->m_pSock && pThis->m_pSock->vt->DoSend(pThis->m_pSock, data, data_size, unk) == 1;
}
*/

DWORD WINAPI MyFunc(LPVOID lpvParam) {
	Util::OpenConsole();

	

	//FLSendJmp = new JMP((void*)0x007FA6F0, (void*)&FLSendHook);
	//FLSendJmp->Activate();


	if (!Net::InitDPObjects()) {
		using namespace nana;
		form fm;
		msgbox(fm, "Failed sigscan for DP / DirectPlay");
		fm.show();
		exec();
	}

	// Find whichever DPObject is active and hook it
	// Net::g_DPlay == world
	// Net::g_dpLoginClient == login

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

		/*
		std::cout << "(*Net::g_dpCertified)->m_pDPSock" << ": 0x" << std::hex << (*Net::g_dpCertified)->m_pDPSock << std::endl;
		print_send((*Net::g_dpCertified)->m_pDPSock);
		std::cout << "(*Net::g_DPlay)->m_pDPSock" << ": 0x" << std::hex << (*Net::g_DPlay)->m_pDPSock << std::endl;
		print_send((*Net::g_DPlay)->m_pDPSock);
		std::cout << "(*Net::g_dpLoginClient)->m_pDPSock" << ": 0x" << std::hex << (*Net::g_dpLoginClient)->m_pDPSock << std::endl;
		print_send((*Net::g_dpLoginClient)->m_pDPSock);
		
		*/
		Sleep(250);
	}


	
	


	//Net::original_dosend = (*Net::g_DPlay)->m_pDPSock->m_pSock->vt->DoSend;
	//(*Net::g_DPlay)->m_pDPSock->m_pSock->vt->DoSend = (Net::DoSend_t)Net::DoSendHook;

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