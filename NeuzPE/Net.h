#pragma once

#include <Windows.h>
#include <cstdint>
#include "dplay_stub.h"
#include <string>

namespace Net {

	struct CDPMng;
	struct CDPMng_vtable;
	struct CDPSock;
	struct CClientSock;
	struct CClientSock_vtable;

	typedef BOOL(__thiscall *DoSend_t)(CClientSock *, uint8_t*, uint32_t, uint32_t);
	typedef void(__thiscall *MessageHandler_t)(CDPMng*, LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom);

	struct CDPMng {
		CDPMng_vtable* vtable;
		CDPSock* m_pDPSock;
		HANDLE m_hRecvThread;
		HANDLE m_hClose;
	};

	struct CDPMng_vtable {
		void* virtual_dtor;
		MessageHandler_t SysMessageHandler;
		MessageHandler_t UserMessageHandler;
	};

	struct CDPSock
	{
		void* vtable;
		CClientSock *m_pSock;
		u_short	m_uPort;
		char	m_lpAddr[32];
	};

	struct CClientSock
	{
		CClientSock_vtable *vt;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
	};

	struct CClientSock_vtable
	{
		int field_0;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		DoSend_t DoSend;
		int field_24;
		int field_28;
	};

	extern DoSend_t original_dosend;

	extern CDPMng** dp_auth;
	extern CDPMng** dp_world;
	extern CDPMng** dp_login;

	std::string GetNameForCClientSock(CClientSock* sock);
	CClientSock* GetCClientSockForName(std::string name);

	BOOL __fastcall DoSendHook(CClientSock * pThis, void* EDX, uint8_t* data, uint32_t data_size, uint32_t unk);

	// Does sigscans for the direct play ("dp") globals.
	// Returns false on failure.
	bool InitDPObjects();
};