#pragma once

#include "SkillConfig.h"

class UserData;
class SkillObj
{
public:
	SkillObj(WORD	wID,BYTE	bLv);
	~SkillObj();

	void		SetLv(BYTE	bLv);

	SkillBase*	pSBase;
	SEffLvInfo*	pLvBase;
	BYTE		bySLv;
	BYTE		byPos;
	DWORD		dwCDTick;	//下次CD好的时间点
private:
	
};

class SkillCenter
{
public:
	SkillCenter(UserData* pUD);
	~SkillCenter();

	void		DBInit();
	bool		RequestSkillUp(WORD wSID,bool bJob);
	void		RequestSetSCut(WORD wSID, BYTE bPos);
	SkillObj*	SetSkill(WORD wSID, BYTE bLv);	//更新技能信息
	void		ResetJobSkill();
	void		RemoveSpSkill(WORD wSID);
	SkillObj*	GetSkill(WORD	wSID);
	SkillObj*	GetSkillByPos(BYTE bPos);
	void		SendSyncAllSkill();

	void	_sendClient(SkillObj* pSObj);
	void		_insertDB(SkillObj* pSObj);
	void		_removeDB(SkillObj* pSObj);
	xnList*		m_pListSkill;
	UserData*	m_pUser;
private:
	void	_saveDB(SkillObj* pSObj);
	
	void	_sendDelSkill(WORD wSID);
	void	_syncMObj(SkillObj* pSObj);
};