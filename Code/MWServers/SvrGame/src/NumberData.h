#pragma once

#include <Windows.h>
#include "xnlist.h"

class NumberData
{
public:
	NumberData(int nCnt);
	~NumberData();

	void	SetVal(BYTE byEnum, int nVal);
	int		GetVal(BYTE byEnum);
	void	AddVal(BYTE byEnum, int nVal);
	bool	SetData(BYTE* p,int nSize);
	bool	GetData(BYTE* p,int &nSize);

private:
	BYTE* m_pData;
	int m_nMax;
};