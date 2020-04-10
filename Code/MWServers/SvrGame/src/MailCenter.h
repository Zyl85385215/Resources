#pragma once

#include "xnlist.h"
#include <Windows.h>
#include "ios_user_packet.h"

//邮件对象
class DBControl;
class UserData;
class MailObject
{
public:

	void	DBInsertMail(DBControl* pDB);
	void	DBUpdateMail(DBControl* pDB);
	void	DBDeleteMail(DBControl* pDB);

	DWORD	dwMailID;
	DWORD	uinSender;
	DWORD	uinRecver;
	char	szTitle[32];
	char	szBody[255];
	BYTE	byState;		
	int		nMoney;			//负数的钱表示付费取件, 付的费用邮件方式返回给发出者
	int		nItemID[nMaxMailItemCount];
	int		nCnt[nMaxMailItemCount];
	int		nItemIndex[nMaxMailItemCount];
	DWORD	dwReturnTick;	//有效期, 超期的邮件将自动退回

	void	SetItemID();

	bool	AddItem(int nIndex);
	int		GetMailItemCount();
};

//个人邮箱
class MailBox
{
public:
	MailBox(DWORD dwUin);
	~MailBox();

	void	DBInitMBox(DBControl* pDB);
	void	SetMailRead(DWORD	dwMailID,DBControl* pDB);

	bool	DeleteMail(DWORD	dwMailID);

	DWORD	m_dwUin;
	
	xnList*	pListMails;
	int		m_nMailCount;

	MailObject*	GetMail(DWORD dwMailID);
};

/*--------------------------------------------------------
中心有待发/待退邮件列表
在线玩家5秒一次update到中心取件,之后存邮箱发客户端
已送达的邮件玩家上线自己到数据库取

---------------------------------------------------------*/
//邮局
class MailCenter
{
public:
	MailCenter();
	~MailCenter();

	
	void			DBInitMail(DBControl* pDB);
	int				GetMaxMailID(DBControl* pDB);

	//玩家线程调用,请求发件
	MailObject*		RequestSendMail(DWORD	dwSend, DWORD	dwRecv, char* szT, char* szB, int nMoney = 0, int* nIndex = NULL);
	MailObject*		RequestSendItemMail(DWORD	dwSend, DWORD	dwRecv,const char* szT,const char* szB, int nMoney = 0, int nItemId = 0,int nItemCnt = 0,bool bBind = false);
	
	//玩家线程调用,给玩家派件,分钟
	int				GetMyMails(UserData* pUser, MailBox* pBox);

	int 			GetUserMailCntBy(DWORD dwUin);

	//主线程检测邮件超期退回等
	void			UpdateMintue();
	void			AddMailLog(MailObject*  pMail);

	

private:
	CRITICAL_SECTION	m_xMailLock;
	xnList*				m_pLMailWait;
	DBControl*			m_pDB;
	DWORD				m_dwMaxMailID;
};

class SysMailObj{
public:
	DWORD	dwID;
	char	szTitle[32];
	char	szBody[255];	
	int		nMoney;			//负数的钱表示付费取件, 付的费用邮件方式返回给发出者
	int		nItemID;
	int		nCnt;
	bool	bBind;
	DWORD	dwReturnTick;	//有效期
};
const int const nNextSysMailWFlag = 30001;
class SysMailCenter
{
public:
	SysMailCenter();
	~SysMailCenter();
	void			DBInit(DBControl* pDB);

	int				AddSysMail(const char* szT,const char* szB,DWORD dwTime,int nMoney = 0, int nItemId = 0,int nItemCnt = 0,BYTE byBind = 0);

	void			Update();

	SysMailObj*		GetSysMailFromID(DWORD dwID);

	void			RemoveSysMail(DWORD dwID);

	void			GetSystemMail(UserData* pUser);

	int				GetMaxSysID();
	int				m_nNextID;
private:
	void			_insertDB(SysMailObj* pObj);
	void			_deleteDB(DWORD dwID);
	DBControl*			m_pDB;
	
	xnList*				m_pLMailList;
};