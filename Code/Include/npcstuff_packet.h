#ifndef _NPCSTUFF_PACKET_H_
#define _NPCSTUFF_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum {
	NPCSTUFF_OPEN,
	NPCSTUFF_LIST,
	NPCSTUFF_DATA,		
};

struct NPCSTUFF_NPCSTUFF_OPEN_Data {
	BYTE	skillIndex;		
};
struct NPCSTUFF_NPCSTUFF_LIST_Data {
	BYTE count;
};

struct NPCSTUFF_NPCSTUFF_DATA_Data {
	WORD itemID;	
	int mapIndex;
	int price;
	int amount;
};

#pragma pack (pop)
#endif