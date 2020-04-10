#pragma once

#include <windows.h>
#include <vector>
#include "singleton.h"

class UserData;

typedef bool (*LPMSGCOMMANDFUNCTION)(UserData * pData, std::vector<char*> comstr);

struct MsgCommand
{
	char	m_szMsgCommand[64];
	BYTE	m_byGmLv;
	LPMSGCOMMANDFUNCTION m_lpFunction;
};

class DebugCmd	: public	CSingleton<DebugCmd>
{
public:
	DebugCmd();
	bool	DealMsg(UserData* pUser, const char* szMsg,BYTE bGmLv);


	static	bool	MsgCommand_SendSystemMail(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddPow(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddRmb(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddGold(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddExp(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddPetExp(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddItem(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddPet(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddHorse(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_Msg(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_Jump(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_Pull(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_Push(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_Full(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_FixEquip(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_ChgSkin(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddHonour(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_CallEnemy(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddSkill(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddState(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddQuest(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_LockNpcCmd(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_Kill(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_RefleshTest(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddAcitve(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_SetActiveOffsetTime(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddHomeCtb(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddHomeRsc(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddFortExp(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_AddSvrExpMult(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_SetWealth(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_ChgWarRankPos(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_KillMapEnemyByID(UserData* pData, std::vector<char*> comstr);
	static	bool	MsgCommand_ForbidPlayer(UserData* pData, std::vector<char*> comstr);
private:
	int m_nCommandSize;
};
