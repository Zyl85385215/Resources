#pragma once

#include "SkillConfig.h"
#include "ios_mapcmd_packet.h"

class MapAIObj;
class SkillObj
{
public:
	SkillObj(WORD	wID,BYTE	bLv);
	~SkillObj();

	void		ResetCD(MapAIObj* pParent);		//重新设置CD
	bool		CheckTargetRange(int	nDis);	//跟目标的距离能否释放
	void		SetLv(BYTE	bLv);

	SkillBase*	pSBase;
	SEffLvInfo*	pLvBase;
	BYTE		bySLv;
	BYTE		byPos;
	DWORD		dwCDTick;	//下次CD好的时间点

	WORD		wUserTCnt;	//技能使用次数
};

class SkillCenter
{
public:
	SkillCenter(MapAIObj* pObj);
	~SkillCenter();

	SkillObj*	CheckUserSkill(WORD	wSID);		//检测技能是否已CD和可释放条件
	SkillObj*	AutoUserSkill();
	void		SetSkill(WORD wSID, BYTE bLv,BYTE	bPos, DWORD	dwCD);	//更新技能信息
	void		ResetJobSkill();
	SkillObj*	GetSkill(WORD	wSID);

	xnList*		m_pListSkill;
	SkillObj*	m_ppActSkill[MAX_ACTIVE_SKILL];
	MapAIObj*	m_pParent;

	void		StrikeSkill(WORD wSkill);
	WORD		m_wStrikeSkill;
};