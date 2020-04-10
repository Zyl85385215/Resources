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
	void	StrengthenSlave(SlaveCenter* pSC,int nID,BYTE byPos);		//ǿ��
	void	ExchangeSlave(SlaveCenter* pSC,int nID);					//����
	void	VariatSlave(SlaveCenter* pSC,int nID,int* szID,int nCnt);	//����
	void	SetActiveSlave(SlaveCenter* pSC,int nID);					//�����ս 
	void	ChangeSlaveName(SlaveCenter* pSC,int nID,char* szName);		//����
	void	ReleaseSlave(SlaveCenter* pSC,int nID);						//�ͷų���
	void	TradeSlave(UserData* pSeller,UserData* pBuyer,int nID){}	//����
	void	GetSlave(SlaveCenter* pSC,int nGetType);                    //�����
	void	QuickGetSlave(SlaveCenter* pSC,int nGetType);               //���ٳ����
	void	ReviveSlave(SlaveCenter* pSC,int nID,bool bItem = true);						//���ܸ�Ϊ�ӳ�������,ÿ�ż�1~5��
private:
	int		CheckPetSkill(SlaveBaseData* pPet,int nID,int nLv);
	bool	CheckEnoughPet(SlaveCenter* pSC,SlaveBaseData* pPet,int* szID,int& nCnt);
};

class HorseCenter;
class HorseOperation : public CSingleton<HorseOperation>
{
public:
	void	EquipHorse(HorseCenter* pSC,int nID);
	void	FeedHorse(HorseCenter* pSC,int nID,bool isSenior,int nCnt);			//ιʳ���
	void	CultivateHorse(HorseCenter* pSC,int nID,BYTE  byType);				//��������
	void	CultivateHorseEx(HorseCenter* pSC,int nID,BYTE  byType);				//��������10
	void	CultivateHorseRlt(HorseCenter* pSC,int nID,bool ifSure);
	void	EatHorse(HorseCenter* pSC,int nID,int* szID,int nCnt);
	void	ReleaseHorse(HorseCenter* pSC,int nID);						//��������
	void	RideHorse(HorseCenter* pSC,bool ifOn);						//����
};