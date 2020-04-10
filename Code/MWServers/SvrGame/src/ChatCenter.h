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


	BYTE	byCRType;		//房间类型
	int		nKeyVal;		//房间编号
	

	xnList*	pList;		//成员

};

/*-----------------------------------------------------
玩家登录和注销就到聊天中心注册/取消 各个房间信息
所有的消息都是发到房间里的
------------------------------------------------------*/

class ChatCenter:public CSingleton<ChatCenter>
{
public:
	ChatCenter();
	~ChatCenter();

	ChatRoom*		RegChatObj(BYTE byCRT, int nKeyVal,UserData* pUser);		//地图频道，地图编号为key，其他通用频道key为0
	void			UnRegChatObj(ChatRoom* pRoom,UserData* pUser);
	void			Update();
	bool			AddMsg(BYTE byCRT,int nKeyVal,UserData* pSendUser,DWORD dwRecvUin,char* szMsg,int* szID = NULL,int nCnt = 0);	//pSendUser信息来源，为NULL则为系统发送；dwRecvUin一般为0，私聊时目标UIN
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