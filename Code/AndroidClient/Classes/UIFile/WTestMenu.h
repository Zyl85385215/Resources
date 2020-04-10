#pragma once
#include "CtrlObj.h"


class WTestMenu
{
public:
	WTestMenu();
	~WTestMenu();
	void Update();
	void Open();
	void Close();
	void Answer();

	DForm* m_pForm;
	CCtrlRadio* m_pRadio;
};

class WTestRankMenu
{
public:
	WTestRankMenu();
	~WTestRankMenu();
	void Update();
	void Open();
	void Close();
	DForm* m_pForm;
};