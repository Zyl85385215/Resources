#pragma once

#include "CtrlObj.h"

class SkillBase;
class SkillSPMenu	: public TabMenuObj
{
public:
	SkillSPMenu();
	~SkillSPMenu();

	virtual bool    CheckCanOpen();
	virtual	void	Open(int nT);

	void	Update();
	void	ViewSkillInfo(SkillBase* pBase,BYTE	bLv);

	SkillBase*	m_pCurSelSkill;

	DForm*	m_pFormSInfo;
private:
	DComponent*	m_ppSkillIco[12];
	DComponent*	m_ppSkillName[12];
	DComponent*	m_ppSkillBack[12];
};