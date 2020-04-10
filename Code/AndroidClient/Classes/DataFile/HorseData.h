#pragma once
#include "PacketCenter.h"
#include "HorseConfig.h"
#include "ios_yjgslave_packet.h"
#include "SkinConfig.h"




class HorseCData{
public:
	HorseCData();
	~HorseCData();
	void	RecalParam();
	int		AddExp(int nExp,bool ifReal = true);
	void	SetExp(BYTE byLevel,int nExp);


	int			m_nParam[HORSE_PARAM_MAX];
	int			m_nPotential[HORSE_CULTIVATE_MAX];
	int			m_szTempCul[HORSE_CULTIVATE_MAX];
	DWORD		m_dwUin;
	DWORD		m_dwID;
	char*		m_pName;
	int			m_nType;
	int			m_nSkin;
	BYTE		m_bySlot;
	BYTE		m_byLevel;
	DWORD		m_nExp;
	int			m_nEatLimt;
	BYTE		m_byState;
	bool		m_ifEquip;
	HorseBasic*	m_pBasic;
	ObjSkinInfo* m_pSkinObj;
	ObjSkinInfo* m_pSkinObjNext;
};

class HorseCCenter : public PacketListener
{
public:
	HorseCCenter();
	~HorseCCenter();
//操作
	bool		AddHorse(HorseSendInfo* pInfo);
	bool		UpdateHorse(HorseSendInfo* pInfo);
	void		RemoveHorse(DWORD id);
	HorseCData* GetHorse(DWORD id);
	void		EquipHorse(DWORD id);
//发包
	void		RequestEquipHorse(int byPos);
	void		RequestFeedHorse(int byPos,bool ifSenior,int nCnt);
	void		RequestCultivateHorse(int byPos,BYTE  byType);
	void		RequestCultivateRlt(int byPos,bool ifSure);
	void		RequestEatHorse(int byPos,int* szID,int nCnt);
	void		RequestReleaseHorse(int byPos);
	void		RequestRide(bool ifOn, bool bWarn = true);
//管理
	void		Update();
	bool		PacketProcess(ZPacket* pPacket);
	void		ClearList();

	xnList*		m_pHorseList;
	bool		m_ifNumChange;
	HorseCData*	m_pEquipHorse;
	bool		m_ifRide;
	ZPacket*	m_pFPacket;
};