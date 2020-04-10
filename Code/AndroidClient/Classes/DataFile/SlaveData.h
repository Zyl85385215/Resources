#pragma once
#include "PacketCenter.h"
#include "SlaveConfig.h"
#include "ios_yjgslave_packet.h"
#include "SkinConfig.h"


class SlaveCData{
public:
	SlaveCData();
	~SlaveCData();
	void	RecalParam();
	void	AddExp(int nExp);
	void	SetExp(BYTE byLevel,int nExp);


	int			m_nParam[SLAVE_PARAM_MAX];
	int			m_nSkill[SLAVE_SKILL_MAX];
	int			m_nStrengthen[SLAVE_STRENGTHEN_MAX];
	int			m_nSkillLv[SLAVE_SKILL_MAX];
	DWORD		m_dwUin;
	DWORD		m_dwID;
	char*		m_pName;
	int			m_nType;
	BYTE		m_bySlot;
	BYTE		m_byLevel;
	BYTE		m_byVariatlv;
	DWORD		m_nExp;
	BYTE		m_byState;
	bool		m_ifActive;
	DWORD		m_dwMapID;
	int			m_nHp;
	int			m_nLifePoint;
	SlaveBasic* m_pBasic;
	ObjSkinInfo* m_pSkinObj;
	SlaveVariatBasic* m_pVariat;
};

class SlaveCCenter : public PacketListener
{
public:
	SlaveCCenter();
	~SlaveCCenter();
//操作
	bool		AddSlave(SlaveSendInfo* pInfo);
	bool		UpdateSlave(SlaveSendInfo* pInfo);
	void		RemoveSlave(DWORD id);
	SlaveCData* GetSlave(DWORD id);
	void		ActiveSlave(DWORD id,DWORD mapid);
	int			GetSlaveCnt(int nType);
	bool		CheckExcEnough(SlaveCData* pSlave,int* szRlt);
	bool		CheckCount();
//发包
	void		RequestActiveSlave(int byPos);
	void		RequestChangeName(int byPos,char* pName);
	void		RequestRelease(int byPos);
	void		RequestLearnSkill(int byPos,int nItemID);
	void		RequestStrengthen(int byPos,BYTE bySel);
	void		RequestExchange(int byPos);
	void		RequestVariat(int byPos,int* szID,int nCnt);
	void        RequestGetPet(int nGetType);
	void        RequestQuickGetPet(int nGetType);
	void		RequestRivive(int byPos);
//管理
	void		Update();
	bool		PacketProcess(ZPacket* pPacket);
	void		ClearList();

	xnList*		m_pSlaveList;
	BYTE		m_byUnlockCnt;
	bool		m_ifNumChange;
	SlaveCData*	m_pActiveSlave;
	DWORD		m_dwReviveTick;
};