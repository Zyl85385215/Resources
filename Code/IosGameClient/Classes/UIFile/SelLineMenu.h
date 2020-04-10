#pragma once

#include "CtrlObj.h"
#include "singleton.h"
#include <string>

struct ServerLine{
	int		nSvrID;
	char *	szIP;
	int		nPort;
	char *	szSpr;
	char *	szName;
};

class	ServerLineCfg	: public CSingleton<ServerLineCfg>
{
public:
	ServerLineCfg();
	~ServerLineCfg();

	void		LoadFile();
	ServerLine*	GetLine(int nArea, BYTE bLine);
	void		RfSvrLine(int nIndex, char* szIP, int nPort);
	ServerLine*	RandGetLine();

	int         GetAreaCount(int nMaxLine);
	void        GetSvrListByArea(xnList* pList,BYTE ByArea);

	bool        CheckAreaVisiable(BYTE bArea);

	void        GetAreaList(xnList* pList,BYTE byType);

	xnList*		m_pList;

	char*		m_szAppUrl;
};

class SelLineMenu
{
public:
	SelLineMenu();
	~SelLineMenu();
	void	Close();

	void	SetSelLine(int nIndex);
	void	SetMaxLine(int nMaxCnt);
	void	RefreshLineByArea(BYTE byArea);
	void	RefreshArea(BYTE byArea);
	DForm*		m_pForm;
	DForm*		m_pFormEx;
	DForm*		m_pFormNotice;

	DComponent*	m_ppCompBtn[9];
	DComponent*	m_ppCompBack[9];
	DComponent*	m_ppCompState[9];
	DComponent*	m_ppCompArea[4];
	DComponent*	m_ppCompName[4];

	DComponent* m_pView;
	void        ShowNotice();

	std::string	strNotice;

	BYTE    bArea;
	int		nSelIndex;
	int		m_nMaxSrvCnt;
	int		m_nOpenSrvCnt;
	ServerLine*	m_pSelLine;

	xnList* m_pStateList;
	int     GetLineState(int nLine);

	//xnList* m_pAreaNameList;
	//char*   GetAreaName(BYTE byArea);
};