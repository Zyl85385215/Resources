#pragma once

#include "singleton.h"
#include "windows.h"
#include "xnlist.h"

class FIValType
{
public:
	BYTE	bType;	//1=银两，2=道具，3=金元宝，4=绑定金元宝，5=绑定金元宝等
	int		nVal1;
	int		nVal2;
};

class FriendCInfo
{
public:
	FriendCInfo();
	~FriendCInfo();
	WORD	wID;

	WORD	wStateID;
	WORD	wSkillID[5];

	xnList*	pListCost;
	xnList*	pListSchool;
	xnList* pListTitles;
	xnList* pListFriends;
	BYTE	bLvMin,bLvMax;
	BYTE	bHLvMin,bHLvMax;
	WORD	wPkMin,wPKMax;


#ifndef _ISSERVER
	char	szName[16];
	char	szAge[5];
	char	szJob[16];
	char	szDesc[255];
	char	szStatetxt[4][20];
#endif
};

class FriendsConfig : public CSingleton<FriendsConfig>
{
public:
	FriendsConfig();
	~FriendsConfig();

	void	LoadFile(char* szFile);
	FriendCInfo*	GetFInfoByID(WORD wID);

private:
	xnList*		m_pListFriends;
};