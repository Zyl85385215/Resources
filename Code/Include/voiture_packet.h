#ifndef _VOITURE_PACKET_H_
#define _VOITURE_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum {
	SYNCVOITURE,
	VOITURE_DISAPPEAR,
	GET_VOITUREINFO,
	VOITUREINFO,
	VOITUREMOVE,
	VOITURE_PICKUP,	//¼ñ³µ	
};

struct VOITURE_VOITURE_PICKUP_Data {
	DWORD uin;
};

struct VOITURE_SYNCVOITURE_Data {
	DWORD uin;
	WORD x;
	WORD y;
	BYTE direction;
	DWORD	masterUin;	
	BYTE	status;
};

struct VOITURE_VOITURE_DISAPPEAR_Data {
	DWORD uin;
};

struct VOITURE_GET_VOITUREINFO_Data {
	DWORD uin;
};

struct VOITURE_VOITUREINFO_Data {
	DWORD uin;
	DWORD items[6];
	char name[1];
};

struct VOITURE_VOITUREMOVE_Data {
	DWORD uin;
	WORD sx,sy;
	WORD tx,ty;
	BYTE direction;
};

#pragma pack (pop)
#endif