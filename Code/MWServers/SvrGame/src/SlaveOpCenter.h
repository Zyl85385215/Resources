#pragma once
#include "singleton.h"
#include <Windows.h>

class SlaveCenter;
class SlaveBaseData;
class UserData;
class UserFlag;
class SlaveOperation : public CSingleton<SlaveOperation>
{
public:
	void	LearnSlaveSkill(SlaveCenter* pSC,int nID, int nItemID);
	void	StrengthenSlave(SlaveCenter* pSC,int nID,BYTE byPos);		//强化
	void	ExchangeSlave(SlaveCenter* pSC,int nID);					//进化
	void	VariatSlave(SlaveCenter* pSC,int nID,int* szID,int nCnt);	//变异
	void	SetActiveSlave(SlaveCenter* pSC,int nID);					//宠物出战 
	void	ChangeSlaveName(SlaveCenter* pSC,int nID,char* szName);		//改名
	void	ReleaseSlave(SlaveCenter* pSC,int nID);						//释放宠物
	void	TradeSlave(UserData* pSeller,UserData* pBuyer,int nID){}	//交易
	void	GetSlave(SlaveCenter* pSC,int nGetType);                    //抽宠物
	void	QuickGetSlave(SlaveCenter* pSC,int nGetType);               //快速抽宠物
	void	ReviveSlave(SlaveCenter* pSC,int nID,bool bItem = true);						//功能改为加宠物寿命,每颗加1~5点
private:
	int		CheckPetSkill(SlaveBaseData* pPet,int nID,int nLv);
	bool	CheckEnoughPet(SlaveCenter* pSC,SlaveBaseData* pPet,int* szID,int& nCnt);
};

class HorseCenter;
class HorseOperation : public CSingleton<HorseOperation>
{
public:
	void	EquipHorse(HorseCenter* pSC,int nID);
	void	FeedHorse(HorseCenter* pSC,int nID,bool isSenior,int nCnt);			//喂食马草
	void	CultivateHorse(HorseCenter* pSC,int nID,BYTE  byType);				//坐骑培养
	void	CultivateHorseEx(HorseCenter* pSC,int nID,BYTE  byType);				//坐骑培养10
	void	CultivateHorseRlt(HorseCenter* pSC,int nID,bool ifSure);
	void	EatHorse(HorseCenter* pSC,int nID,int* szID,int nCnt);
	void	ReleaseHorse(HorseCenter* pSC,int nID);						//放生坐骑
	void	RideHorse(HorseCenter* pSC,bool ifOn);						//骑马
};