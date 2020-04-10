#pragma once

#include "CtrlObj.h"
class SkillBase;
class SkillMenu	: public TabMenuObj
{
public:
	SkillMenu();
	~SkillMenu();

	virtual	void	Open(int nT);
	void	OpenForm(BYTE	bJob,int nT);
	void	InitTabEvent(int nCnt);

	void	Update();
	void	ViewSkillInfo(SkillBase* pBase,BYTE	bLv);
	void	ViewPetSkillInfo(SkillBase* pBase,BYTE	bLv);
	void	OpenShortCut();
	void	SetShortCut(int nPos);
	void	CloseShortCut();
	void	CancelSettingShortCut();
	void	RequestLvUp();
	void	RequestCZSkillPnt();

	SkillBase*	m_pCurSelSkill;

	DForm*	m_pFormSInfo;
private:
	DForm*	m_pFormJob[4];

	
	DForm*	m_pFormSCut;

	
	DComponent*	m_ppSkillIco[12];
	DComponent*	m_ppSkillName[12];
	DComponent*	m_ppArrow[8];
	BYTE	m_byCurJob;
	DWORD	m_dwLockSkillUp;
};