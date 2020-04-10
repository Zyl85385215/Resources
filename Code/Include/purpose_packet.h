#pragma  once

#pragma pack (push)
#pragma pack (1)

enum
{	
	PUR_START,
	PUR_QUEST_2S,
	PUR_CHECK,
	PUR_QUEST_2C,
	PUR_GETONLINE,
	PUR_GETOUTLINE,
	PUR_FINISH,
};

struct PURPOSESYS_PUR_START_Data {
	DWORD	DaojiLixian;
	DWORD	Zaixian;
	DWORD	DaojiZaixian;
	BYTE	byLiXian;
	bool	New;
};

struct PURPOSESYS_PUR_CHECK_Data {
	DWORD	DaojiLixian;
	DWORD	Zaixian;
	DWORD	DaojiZaixian;
	BYTE	byLiXian;
};

struct PURPOSESYS_PUR_QUEST_2C_Data {
	int		QusetId[5];
	BYTE	Finish;
};

struct PURPOSESYS_PUR_QUEST_2S_Data {
	int		QusetId[5];
};

struct PURPOSESYS_PUR_FINISH_Data {
	BYTE	Finish;
};

#pragma pack (pop)
