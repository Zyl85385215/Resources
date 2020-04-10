#pragma once
#include "CtrlObj.h"
#include "ClientChat.h"
#include <string>
#include "ItemData.h"
#define FACECOUNT	48

class ItemObj;
class FaceChooseMenu
{
public:
	FaceChooseMenu();
	~FaceChooseMenu();
	void Open();
	void Close();
	DForm* m_pForm;
};

class ItemChooseMenu
{
public:
	ItemChooseMenu();
	~ItemChooseMenu();
	void Open();
	void Close();
	void Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	DComponent* m_pEquip[10];
	int		m_nSelectPage;
	bool	m_bLockEquip;
	void	UpdatePage(int	nSelectPage,bool bSelect);

	void	SetType(int nType);		//设置详细信息的类型
	void	SetFilter(bool (*nFun)(ItemObj* pObj,void* param),void* param = NULL);				//设置背包过滤
	void	SetLockEquip(bool bLock);	//锁定装备
	void	SetChooseCallback(void (*fnCallBack)(void* pParam,BYTE byType));	//选择回调
	void	SetButtonSpr(const char* szSpr1,const char* szSpr2 = NULL);			//显示的按钮
	bool	(*nFilter)(ItemObj* pObj,void* param);
	void	(*fCallBackEvent)(void* pParam,BYTE byType);
	void*	m_pParam;
	int		m_nType;
	const char* szSpr1;
	const char* szSpr2;

	bool    m_bReSort;
};

class ChatNameProcessMenu
{
public:
	ChatNameProcessMenu();
	~ChatNameProcessMenu();
	void	Open(int nUin,int nx,int ny,char* pName);
	void	Close();

	DForm*	m_pForm;
	int m_nUin;
	char* m_pName;
};

class ChatSysMenu
{
public:
	ChatSysMenu();
	~ChatSysMenu();
	void Open();
	void Close(bool ifSave = false);

	DForm* m_pForm;
	CCtrlCheck* m_pCheck[6];
};

class ChatMenu
{
public:
	ChatMenu();
	~ChatMenu();
	void Open();
	void Close();
	void Update();
	void InsertItem(ItemObj* pItem);
	void InsertFace(int nIcon);
	void Clear();
	void Send();
	void SetChannel(BYTE byCha);
	void SetPage(BYTE byPg);
	void RefleshPage();
	void UpdateFace();
	void SetTargent(char * szName);

	DForm* m_pForm;
	DForm* m_pButtonForm;
	DComponent* m_pEdit;
	DComponent* m_pView;
	DComponent*	m_pBack;
	std::string m_strWords;
	int m_szItemID[5];
	int m_nCnt;
	BYTE m_bChannel;
	BYTE m_byPage;
	bool m_ifSpeek;
	char m_szTarName[20];
	xnList* m_pFaceList;
	FaceChooseMenu* m_pFacemenu;
	ItemChooseMenu* m_pItemMenu;
	ChatNameProcessMenu* m_pProcessMenu;
	ChatSysMenu*	m_pSysMenu;

	DWORD		m_dwWCLmtTick;
};

class SimpleChatMenu
{
public:
	SimpleChatMenu();
	~SimpleChatMenu();
	void RefreshPage();
	DForm* m_pForm;
	DComponent* m_pView;
};

class SystemMessageMenu
{
public:
	SystemMessageMenu();
	~SystemMessageMenu();
	void Update();

	bool m_isPlaying;
	char* m_ShowMsg;
	DComponent* m_pCompFront;
	DForm* m_pForm;
};

class WarMessageMenu
{
public:
	WarMessageMenu();
	~WarMessageMenu();
	void	Update();

	DForm* m_pForm;
	DComponent* m_pComp[3];
	DComponent* m_pBack[3];
	DWORD	m_dwMoveTick[3];
	char* m_pMsg;
	DWORD m_dwTick;
};