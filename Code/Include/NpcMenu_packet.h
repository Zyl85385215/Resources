#ifndef _NPCMENU_PACKET_H_
#define _NPCMENU_PACKET_H_

#pragma pack (push)
#pragma pack (1)

//消息类型
enum _NPCMENU
{
	NPCM_SLAVE_RESET,
	NPCM_SLAVE_PHASE,
	NPCM_SLAVE_FUSE,

	NPCM_SLAVE_LSKILL,
	NPCM_SLAVE_TRANSS,

	NPCM_CLOSE = 100,
};

//struct BATTLE_BATTLE_SEND_PLAYER_STATUS_Data
//{
//	dxSignUpPlayerGameStatus status;
//};

#pragma pack (pop)

#endif