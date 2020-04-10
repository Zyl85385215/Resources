#pragma once

#include "CtrlObj.h"


class MissionMenu
{
public:
	MissionMenu();
	~MissionMenu();

	void		Open();
	DForm*		m_pForm;

	DForm*		m_pFormTrack;
	DComponent*	m_pTFCompBtn;
	DComponent*	m_pDCompModel;

	DComponent*	m_pCompType[4];
	DComponent*	m_pCompAccepted[2];
	DComponent*	m_pCompTrackQuest;
	CCtrlFrame*	m_pTrackQuest;

	void	Update();
	void	SetViewQuest(BYTE bType, bool bAccept);
	void	SetSelQuest(int nPos);
	void	UpViewQuestComp(DForm* pForm, QuestObj* pQBase, ClientQuest* pClientQ);
	

	bool	CheckNpcQ(int nNpcID);

	BYTE	m_byCurType;
	BYTE	m_byCurAcpt;

	QuestObj*	m_pCurSelQuest;
private:
	void	_updateQuickTrack();

	void	_trackQuest(int nPos, ClientQuest* pCQuest);
	void	_trackQBase(int nPos, QuestObj* pQBase);
};