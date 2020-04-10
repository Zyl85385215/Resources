#pragma once


#include "CtrlObj.h"
class GetNumMenu{
public:
	GetNumMenu();
	~GetNumMenu();
	void	Open(void (*EventFunc)(int nResult,void * param),int nMaxCount = 9999999,int nStartCount = 0, void * para = NULL);
	void	Close();
	void	Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	int		m_nMaxCount;
	int		m_nCount;
	void * m_param;
	void	(*eventFunc)(int nResult,void * param);
};
