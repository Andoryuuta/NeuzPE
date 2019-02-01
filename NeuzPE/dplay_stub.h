// Stubs a bunch of direct play types.
// All the the types are taken from ReactOS / wine.

#pragma once
#include <Windows.h>


/* NOTE: DPMSG_HOST and DPMSG_GENERIC share the same format */
typedef struct tagDPMSG_GENERIC
{
	DWORD       dwType; /* Use message type as described above */
} DPMSG_GENERIC, *LPDPMSG_GENERIC,
DPMSG_HOST, *LPDPMSG_HOST,
DPMSG_SESSIONLOST, *LPDPMSG_SESSIONLOST;
typedef struct tagDPMSG_GENERIC * LPDPMSG_GENERIC;

/* DPID - DirectPlay player and group ID */
typedef DWORD DPID, *LPDPID;

#define 	DPID_SYSMSG   0 /* DPID of system */
#define 	DPID_ALLPLAYERS   0 /* DPID of all players */
#define 	DPID_SERVERPLAYER   1 /* DPID of the server player */
#define 	DPID_UNKNOWN   0xFFFFFFFF /* Player ID is unknown */