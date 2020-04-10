#pragma once

#include "xnlist.h"
#include <Windows.h>
#include "ios_user_packet.h"

//�ʼ�����
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
	int		nMoney;			//������Ǯ��ʾ����ȡ��, ���ķ����ʼ���ʽ���ظ�������
	int		nItemID[nMaxMailItemCount];
	int		nCnt[nMaxMailItemCount];
	int		nItemIndex[nMaxMailItemCount];
	DWORD	dwReturnTick;	//��Ч��, ���ڵ��ʼ����Զ��˻�

	void	SetItemID();

	bool	AddItem(int nIndex);
	int		GetMailItemCount();
};

//��������
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
�����д���/�����ʼ��б�
�������5��һ��update������ȡ��,֮������䷢�ͻ���
���ʹ���ʼ���������Լ������ݿ�ȡ

---------------------------------------------------------*/
//�ʾ�
class MailCenter
{
public:
	MailCenter();
	~MailCenter();

	
	void			DBInitMail(DBControl* pDB);
	int				GetMaxMailID(DBControl* pDB);

	//����̵߳���,���󷢼�
	MailObject*		RequestSendMail(DWORD	dwSend, DWORD	dwRecv, char* szT, char* szB, int nMoney = 0, int* nIndex = NULL);
	MailObject*		RequestSendItemMail(DWORD	dwSend, DWORD	dwRecv,const char* szT,const char* szB, int nMoney = 0, int nItemId = 0,int nItemCnt = 0,bool bBind = false);
	
	//����̵߳���,������ɼ�,����
	int				GetMyMails(UserData* pUser, MailBox* pBox);

	int 			GetUserMailCntBy(DWORD dwUin);

	//���̼߳���ʼ������˻ص�
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
	int		nMoney;			//������Ǯ��ʾ����ȡ��, ���ķ����ʼ���ʽ���ظ�������
	int		nItemID;
	int		nCnt;
	bool	bBind;
	DWORD	dwReturnTick;	//��Ч��
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