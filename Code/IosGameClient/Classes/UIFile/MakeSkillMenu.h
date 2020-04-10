#pragma once


#include "CtrlObj.h"
#include "cocos2d.h"
#include "MakeLayer.h"
class MakeSkillTag;
class MakeSkillMenu: public TabMenuObj,public cocos2d::CCObject{
public:
	MakeSkillMenu();
	~MakeSkillMenu();
	DForm*		m_pFormCollect;
	DForm*		m_pFormCollectEx;
	DForm*		m_pFormMake;

	BYTE		m_byType;
	int			m_nSelect;
	void		SetMakeList();
	void		SetMakeType(BYTE byType);
	void		Select(int nSelect);
	void		SelectItem(int nSelect);
	int			m_nMakeSelect;

	int			m_nSkillType;
	virtual		void	Close();

	void		Update();
	//void		Open();
	virtual bool CheckCanOpen();

	virtual	void	Open(int nT);

	void		OpenMenu(int nSkill);

	DComponent*	m_pSwitch[2];
	
	void		SetMakeItem(MakeSkillTag* pTag);

	void		StartMake(MakeSkillTag* pTag);

	int			m_nMake1;
	int			m_nMake2;
	MakeSkillTag* m_pNowTag;

	int			GetPowerMax();

	void		OpenCombo(BYTE byType);
	
	DForm*		m_pSelectForm;
	int			m_nSelectFilterType;
	tempstring	m_szFilter;
private:
	void		_afterMove();
	static	int	m_nMoveCount;
	int			m_nChgPos;
	MakeLayer*	m_pMakeLayer;
};