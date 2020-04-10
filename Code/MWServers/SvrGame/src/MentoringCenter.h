#pragma once

#include <Windows.h>
#include "xnlist.h"
#include <map>
#include "DBControl.h"

#define GRADUATEAGE		50
#define STUDENTAGE		10

class UserSimple;
struct MentoringData
{
	DWORD	m_dwID;			//ʦͽ��ϢID
	BYTE	m_bTeachFlag;
	BYTE	m_bStudyFlag;
	UserSimple*	m_pTeacher;	
	UserSimple*	m_pPupil;
	BYTE	m_byState;		//1���� 0ɾ�� 255��ҵ 
};

struct MRequestData
{
	UserSimple* pUser;
	int		nVal;
	BYTE	byType;		//0��ʦ�б�1ͽ������
	DWORD	dwEndTick;
	bool	ifDelete;
};

class UserData;
class MentoringCenter
{
public:
	MentoringCenter(DBControl*	pDB);
	~MentoringCenter();
	void	DBInit();	
	void	Update();
	void	GetRelationByuin(int nUin,bool ifTeacher,OUT xnList* pList);
	MentoringData* GetRelation(int nTuin,int nPuin);
	MRequestData*	GetTeacher(int nUin);
	MRequestData*	GetRequest(int nPUin,int nTUin);
	bool	ClearRequest(int nPuin);
	bool	ClearTeachRequest(int nTuin);
	bool	DeleteObj(int nTuin,int nPuin);
	void	CheckTeacherList();
	void	ResetDataFlag();

	void	RequestInstruct(UserData* pUser);		//��ҵ
	void	RequestStudy(UserData* pUser,DWORD dwTUin);		//��ʦ
	void	RequestTeacherList(UserData* pUser,int nFlag);
	void	RequestSendItem(UserData* pUser,int nID);
	void	RequestGetItem(UserData* pUser,int nID);

	void	DealWithMsg(UserData* pUser,DWORD dwStuUin );	//��ͽ

	void	DealWithRequest(UserData* pUser,DWORD dwPUin,bool bRlt);	//��������
	void	DealWithRequestP(UserData* pPupil,DWORD dwTuin,bool bRlt);
	void	BreakRelationship(UserData* pUser,int nID);
	void	RequestTotalInfo(UserData* pUser);

	bool	CheckGraduate(UserData* pUser);
	bool	DeleteObj(MentoringData* pObj);

	void	DeleteUser(DWORD dwUin);	//ɾ��ɫʱ�Ĳ���

	bool	CheckTeacherUser(DWORD dwUin);

	DWORD	m_dwMaxID;
	std::map<DWORD,MentoringData*>	m_mapMentoring;
	xnList*	m_pTeacherList;
	int		m_nTListFlag;
	xnList*	m_pRequestList;

	xnList*	m_pRubbishList;

private:
	DBControl*	m_pDB;
	int		_getMaxID();
	bool	_insertObj(MentoringData* pObj);
	bool	_updateObj(MentoringData* pObj);
	bool	_deleteObj(MentoringData* pObj);

	bool	_insertRequest(MRequestData* pObj);
	bool	_deleteRequest(MRequestData* pObj);
};

extern MentoringCenter* g_pMentoringCenter;

class UserMentoring
{
public:
	UserMentoring(UserData* pUser);
	~UserMentoring();
	void	Update();
	void	AddData(MentoringData* pData);
	void	DeleteData(int i);
	void	AddRequest(MRequestData* pData);
	void	AddMyRequest(MRequestData* pData);

	void	SendAll();
	void	SendInstruct();
	void	SendShow();
	void	SendDataAdd(MentoringData* pData);
	void	SendDataDelete(MentoringData* pData);
	void	SendDataChanged(MentoringData* pData);
	void	SendRequestAdd(MRequestData* pData);
	void	SendRequestDelete(MRequestData* pData);
	void	SendMyRequestAdd(MRequestData* pData);
	void	SendMyRequestDelete(MRequestData* pData);
	void	SendHonour();


	UserData*	m_pUser;

	bool		m_ifInstruct;
	bool		m_SendInstruct;

	bool		m_ifShow;
	bool		m_SendShow;

	xnList*		m_pRelationList;
	xnList*		m_pRequestList;
	xnList*		m_pMyRequestList;
};