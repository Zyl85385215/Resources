#pragma once


#include "CtrlObj.h"
#include "ItemData.h"
enum EQUIP_INFO_OPENSTYLE{
	FORM_BAG,
	FORM_BANK,
	FORM_EQUIP_INTENSIFY,
	FORM_EQUIP_INTENSIFYEX,
	FORM_MALL,
	FORM_SHOP,
	FORM_NULL,
	FORM_SHOW,
	FORM_MANOR,
	FORM_EX,
	FORM_PETSKILL,
};
class EquipInfoMenu{
public:
	EquipInfoMenu();
	~EquipInfoMenu();
	void	Open(ItemObj* pObj,int nOpenStyle = FORM_EX);
	void	Open(ItemExBase* pObj,int nOpenStyle = FORM_EX,void* param = NULL);
	void	Close();
	void	Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	ItemObj*	m_pItemObj;
	ItemExBase*	m_pItemBase;
	int		m_nOpenStyle;
	void*		m_pParam;
	DForm*		m_pFormContrast;
	int		m_nPX1;
	int		m_nPX2;

	void	(*fCallBackEvent)(void* pParam,BYTE byType);
	void	SetCallBackEvent(void (*fnCallBack)(void* pParam,BYTE byType));
	void	SetButtonSpr(const char* szSpr1,const char* szSpr2 = NULL);

private:
	void	_setExtParam(DComponent* pCom1,DComponent* pCom2,ItemExBase*	m_pItemBase);
	void	_setGroupInfo(DComponent* pCom1,int nGroup);
	
};

class ItemInfoMenu{
public:
	ItemInfoMenu();
	~ItemInfoMenu();
	void	Open(ItemObj* pObj,int nOpenStyle = FORM_EX);
	void	Open(ItemExBase* pObj,int nOpenStyle = FORM_EX,void* param = NULL);
	void	Close();
	void	Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	ItemObj*	m_pItemObj;
	ItemExBase*	m_pItemBase;
	int		m_nOpenStyle;
	void*		m_pParam;
	int		m_nPX1;
	int		m_nPX2;

	void	(*fCallBackEvent)(void* pParam,BYTE byType);
	void	SetCallBackEvent(void (*fnCallBack)(void* pParam,BYTE byType));
	void	SetButtonSpr(const char* szSpr1,const char* szSpr2 = NULL);
};

class SlaveInfoMenu{
public:
	SlaveInfoMenu();
	~SlaveInfoMenu();
	void	OpenByID(int nSID);

	DForm*		m_pForm;
};


class FastEquipMenu{
public:
	FastEquipMenu();
	~FastEquipMenu();

	xnList*			m_pListTips;
	void			ResetTipsEquip(); 
	void			GetEquip(ItemObj* pObj);
	void			OpenTipEquip(ItemObj* pObj); 

	void	    Close();
	void            CheckListFormClose();

	void			NewHomeTips();

	void		Update();
	void		CheckMentoringTip();
	void		CheckFriendTip();
	void		UpdateActivityIcon();

	DWORD		m_dwViewIndex;
public:
	DForm*		m_pTipForm;
	DForm*		m_pTipListForm;	

	DComponent*	m_pHomeTip;
	DComponent*	m_pFightTip;
	DComponent*	m_pMentoringTip;
	DComponent*	m_pTeamInviteTip;
	DComponent*	m_pMarryRingTip;
	DComponent*	m_pFriendGiftTip;
	DComponent*	m_pFriendRequestTip;
	DComponent*	m_pMentoringAwardTip;
	DComponent*	m_pCurTimeActiveTip;
	DComponent*	m_pCatchFishTip;
};