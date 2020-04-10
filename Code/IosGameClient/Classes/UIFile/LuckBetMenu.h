#pragma once

#include "CtrlObj.h"


class LuckBetMenu
{
public:
	LuckBetMenu();
	~LuckBetMenu();

	void		OpenForm();
	void		Update();
	void		RqStart();
	void		GetResult(BYTE	byEndPos, int nGetVal);
	void		GetResultEx(BYTE	byRtlVal, int nGetVal);
	void		SetBetVal(BYTE byType, int nVal);

	void		SetExBetView(int nVal);

	DForm*		m_pForm;
	DForm*		m_pFormRtl;
	DComponent*	m_ppCompBox[24];

	DComponent*	m_ppCompBetNum[8][3];
	DComponent*	m_ppCompBetRtl[8];
	DComponent*	m_ppCompMyCoin[8];

	int			m_sznPutVal[8];

	CCtrlRadio*	m_pCRadioSel;

	DComponent*	m_pCompIco;
	DComponent*	m_pCompNum;

	DWORD		m_dwTickBetHold;
	BYTE		m_bySelBet;		//当前压的对象
	BYTE		m_byBetType;	//币种

	DWORD		m_dwRunTick;
	BYTE		m_byStep;
	BYTE		m_byEndPos;
	BYTE		m_byRunPos;
	int			m_nTotalUse;

	int			m_nTotalGetVal;
	BYTE		m_byExSel;
	DWORD		m_dwTickExStart;
};