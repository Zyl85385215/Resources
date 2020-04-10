#ifndef _QUEST_PACKET_H_
#define _QUEST_PACKET_H_

#pragma pack (push)
#pragma pack (1)
#define SendQuestAnswer(a)								\
{														\
	BYTE buffer[1024];									\
	Packet * pPacket;									\
	BuildPacket(pPacket, QUEST, QUEST_ANSWER, buffer);	\
	SET_DATA(pAns, QUEST, QUEST_ANSWER, pPacket);		\
	pAns->ansIndex = a;									\
	pClient->sendPacket(pPacket);						\
}

enum {
	QUEST_QUESTION, 
	QUEST_ANSWER,
	QUEST_END,
	
	QUEST_GET_STRING,
	QUEST_ANS_STRING,
	QUEST_ADDLIST,
};

struct QUEST_QUEST_QUESTION_Data {
	BYTE code;
	bool noclose;
	char npcname[32];
	int  size;
	char question[1];
};

struct QUEST_QUEST_ANSWER_Data {
	char ansIndex;
};

struct QUEST_QUEST_GET_STRING_Data {
	char message[1];
};

struct QUEST_QUEST_ANS_STRING_Data {
	char answer[1];
};

struct QUEST_QUEST_ADDLIST_Data {
	char Questlog[1];
};

#pragma pack (pop)

#endif