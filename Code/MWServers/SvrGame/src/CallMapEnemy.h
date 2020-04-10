#pragma once

#include <Windows.h>
#include <map>
const char* const szLuaFunctionOnEnemyBeKill = "OnEnemyBeKill";
class ZPacket;
class CallEnemyCenter
{
public:
	CallEnemyCenter();
	~CallEnemyCenter();

	DWORD		CallEnemyByCfg(WORD wBase,int nMIndex, WORD wX, WORD wY,BYTE bForce,WORD wPowEx=100,char* szTitle = NULL,int nScale = 0);
	void		SetEnemyAct(DWORD dwID,int nMapIndex, int nType, DWORD dwTarget,int nTagType);
	DWORD		LockEnemyIDByMap(DWORD	nMIndex);
	bool		RemoveEnemy(DWORD dwID, int nMIndex);
	bool		KillEnemy(DWORD	nMIndex, WORD wX, WORD wY, WORD wRange, DWORD	dwUin);
	bool		KillMapEnemyByID(DWORD	nMIndex,WORD wEnemyID);
	bool		RecvPacket(ZPacket* pPacket);


	std::map<DWORD,DWORD>	m_mapEnemyID;	//各个地图刷怪的ID
	std::map<DWORD,WORD>	m_mapCopyEnemyID;	//远征副本 当前刷怪ID

	bool        CheckCopyEnemyRepeat(DWORD dwMIndex, WORD wEnemyID);
	void        RefreshmapCopyEnemyID(DWORD dwMIndex);

	void		RunLuaFunctionForEnemyBeKill(int nEnemyID,int nMapID,const char* const szFunctionName);
};