#pragma once

#include "singleton.h"
#include <map>
#ifdef _ISSERVER
	#include "TimeControl.h"
#else
	#include "MTime.h"
#endif

#include "xnlist.h"

typedef struct
{
	CMTime	tmStart;
	CMTime	tmEnd;
	BYTE	bPricePer;
	BYTE	bClosed;
}MI_TIME_OBJ;

class MallILimitConfig : public CSingleton<MallILimitConfig>
{
public:
	MallILimitConfig(void);
	~MallILimitConfig(void);

	bool			Load( const char * szFile );
	MI_TIME_OBJ*	GetMIObj(int nItemID);

	void			AddLockMallI(int nItemID);

	std::map<int,xnList*> mapMILimitTime;
};


class	RideExChgObj
{
public:
	DWORD	dwItemID;
	DWORD	dwUseIID[10];
	WORD	wNeedCnt[10];
	DWORD	dwCostItem;
	bool	bOutBind;
	bool	bUseBind;
};

class	CfgRideExChg	: public	CSingleton<CfgRideExChg>
{
public:
	CfgRideExChg();
	~CfgRideExChg();

	void	Load(char*	szFile);
	RideExChgObj*	GetRECObj(DWORD	dwItem);
	RideExChgObj*	GetRECByCost(DWORD	dwItem);
	RideExChgObj*	GetRECByIndex(WORD wPos);

	xnList	*		m_pList;
};

