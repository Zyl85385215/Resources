#pragma once


#include "xnlist.h"
#include <Windows.h>
#include "singleton.h"
#include "ios_rpgchat_packet.h"

class UserData;
class ChatRoom
{
public:
	ChatRoom(BYTE bT, int nKey);
	~ChatRoom();

	void	AddMsg(char * szMsg,ShowItemSimpleInfo* szInfo,int nCnt,UserData* pUser1,UserData* pUser2);
	void	AddP2PMsg(DWORD	dwObj, char* szTitle, char* szMsg);
	bool	CheckMember(DWORD dwuin);


	BYTE	byCRType;		//��������
	int		nKeyVal;		//������
	

	xnList*	pList;		//��Ա

};

/*-----------------------------------------------------
��ҵ�¼��ע���͵���������ע��/ȡ�� ����������Ϣ
���е���Ϣ���Ƿ����������
------------------------------------------------------*/

class ChatCenter:public CSingleton<ChatCenter>
{
public:
	ChatCenter();
	~ChatCenter();

	ChatRoom*		RegChatObj(BYTE byCRT, int nKeyVal,UserData* pUser);		//��ͼƵ������ͼ���Ϊkey������ͨ��Ƶ��keyΪ0
	void			UnRegChatObj(ChatRoom* pRoom,UserData* pUser);
	void			Update();
	bool			AddMsg(BYTE byCRT,int nKeyVal,UserData* pSendUser,DWORD dwRecvUin,char* szMsg,int* szID = NULL,int nCnt = 0);	//pSendUser��Ϣ��Դ��ΪNULL��Ϊϵͳ���ͣ�dwRecvUinһ��Ϊ0��˽��ʱĿ��UIN
	bool			AddMsg(ChatRoom* pRoom,UserData* pSendUser,DWORD dwRecvUin,char* szMsg,int* szID = NULL,int nCnt = 0);
	void			SendSystemMsg(char* szMsg,BYTE byType,int nid,int nCnt,bool ifPop = true);
	void			SendRoomMsg(char* szMsg,ChatRoom* pRoom);
	void			GetItemInfo(UserData* pUser,int nID);

	
private:
	xnList*		m_pList;
	ChatRoom*		_getChatRoom(BYTE byCRT, int nKeyVal);
	xnList*		m_pShowList;
	DWORD		m_dwNextID;
};