#pragma once

#include "GlobalDefine.h"
#include "xnlist.h"
#include "Interface.h"

#include "ios_user_packet.h"
#include "ItemData.h"
#include "MapAIObj.h"

class GuideObj
{
public:
	GuideObj();
	~GuideObj();
	int		nID;
	char*	szForm;
	char*	szComp;

	int		nX,nY,nW,nH;
	
	int     nCX;
	int     nCY;

	char*   mapId;
	//第二图片
	char*   map2;
	int     freeDian;
	int     nSprX;
	int		nSprY;
	int		nNextID;
	int		nLimitLv;
	int		nEffID;
	int		nEffID2;
	int     eff2X;
	int     eff2Y;
	//bool	ifGift;
	char*	szTxt;	
	DWORD     txtX;
	DWORD     txtY;
	DForm*	pForm;
};

class GuideCenter
{
public:
	GuideCenter();
	~GuideCenter();

	void		SetGuide(int nID);
	void		Update();
	GuideObj*	m_pCurGuide;
	void		NextGuide();

	void        SetGameSceneGuide();
	void        SetUserLvGuide();

	void        SetStrikeGuide();

	void        SaveQuitGuide(int nID);

	void        SetAttackGuide(MapAIObj*	pAObj);
	void        SetGatherGuide(MapAIObj*	pAObj);
	void        SetEnemyDeadGuide(MapAIObj*	pAObj);
	void        SetQuestInfoGuide(USER_S_QUEST_INFO_Data*  pData);
	void        SetQuestRemoveGuide(USER_S_REMOVE_QUEST_Data*  pData);
	void        SetBagMenuGuide();
	void        SetActivityMenuGuide();

	void        SetActivityGuide(int nGuideID, int nActivityID);
	/*void        SaveEndGuideIDFile( const char* szFile );
	int         LoadEndGuideIDFile( const char* szFile );*/
	void        UpdateMapCtrl();
private:
	GuideObj*	_GetGuide(int nID);

public:
	xnList*		m_pList;
	//当前Guide  ID
public:
	int         nCurID;
	int         nStartID;	
	int         nQuitID;

	int			nLvUpID;

	int         nAwardItemPos;
	bool        bGatherQuest;	

	bool        bAtk;	
	BYTE        bCountGuide[100];
	bool        bStrikeEff;	
public:
	int         nActivityPos;
	int         nActivityID;

public:
	void		GetItemInBagPos(int nItemID);

	void		SetGuideInFirstFB(int nQuestID, int nGuideID);
	void		SetInlayBagMenuGuide();
	int			m_nAwardInlayPos;
	void		SetEatBagMenuGuide();
	int			m_nAwardEatPos;
private:
	bool		m_bGuideInFB;
	bool		m_bGuideInOtherFB;

	bool		m_bGatherAuto;

	bool		m_bStoneGuide;
	int			m_nRunMIndex;
	bool		m_bInlayGuide;
	bool		m_bEatGuide;
	bool		m_bCatchFishGuide;
	//bool		m_bActivityFishGuide;
};

extern	GuideCenter*	pGuideCenter;