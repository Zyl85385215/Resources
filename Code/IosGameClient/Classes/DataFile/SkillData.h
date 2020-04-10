#pragma once

#include "SkillConfig.h"
#include "ios_mapcmd_packet.h"

class SkillObj
{
public:
	SkillObj(WORD	wID,BYTE	bLv);
	~SkillObj();

	//void		ResetCD();		//重新设置CD
	bool		CheckTargetRange(int	nDis);	//跟目标的距离能否释放
	void		SetLv(BYTE	bLv);

	SkillBase*	pSBase;
	SEffLvInfo*	pLvBase;
	BYTE		bySLv;
	DWORD		dwCDTick;	//下次CD好的时间点
};

class SkillCenter
{
public:
	SkillCenter();
	~SkillCenter();

	SkillObj*	CheckUserSkill(WORD	wSID);		//检测技能是否已CD和可释放条件
	void		SetActSkill(WORD wSID, BYTE bLv,BYTE bPos,int nCD);	//更新技能信息
	SkillObj*	GetActiveSkill(WORD	wSID);
	void		ResetSkillCD(WORD wSID);
	void		ClearActSkill();
	void		ClearSkill();

	void		RemoveSkill(WORD wSID);
	SkillObj*	SetSkill(WORD wSID, BYTE bLv);
	SkillObj*	GetSkill(WORD	wSID);


	xnList*		m_pListSkill;		//自身学会的技能
	xnList*		m_pListSPSkill;

	int		CheckSkillSetting(WORD wSID);
	SkillObj*	m_ppActSkill[MAX_ACTIVE_SKILL];
	//xnList*		m_pMObjSkill;		//控制对象的快捷键技能
};