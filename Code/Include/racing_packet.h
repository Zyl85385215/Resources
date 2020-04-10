#ifndef _RACING_PACKET_H_
#define _RACING_PACKET_H_

#include "player_packet.h"
#pragma pack (push)
#pragma pack (1)


enum _RACING{
	RACING_START,
	RACING_STOP_SIGNUP,
	RACING_WIN,
};


struct RACING_RACING_START_Data {
	char name[20];
};

#pragma pack (pop)
#endif