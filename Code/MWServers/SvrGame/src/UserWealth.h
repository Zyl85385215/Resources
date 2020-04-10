#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "UserRecord.h"

#define UW_EXP_VALUE	1000000000

enum
{
	UW_RMB,
	UW_GOLD,
	UW_LV,
	UW_EXP,
	UW_POW,
	UW_SKILLPT,
	UW_VIPLV,
	UW_VIPEXP,
	UW_GSLAVELMT,
	UW_HONOUR,

	UW_STARLV,

	UW_BINDRMB,
	UW_LIFE_STARLV,
	UW_EXP_CNT,

	UW_MAX,
};

enum
{
	UW_GOLD_NULL,		//0
	UW_GOLD_TUMO,		//��ħ
	UW_GOLD_ADD,		//�������
	UW_GOLD_MARRY,		//��齱��
	UW_GOLD_MENTOR,		//ʦͽ����
	UW_GOLD_NASALE,		//������
	UW_GOLD_DREAM,		//����ξ�
	UW_GOLD_KILL_ENEMY,		//ɱ��
	UW_GOLD_INVICODE,		//������
	UW_GOLD_MCOPY,		//����
	UW_GOLD_MAIL,		//�ʼ�
	UW_GOLD_QUEST,		//������
	UW_GOLD_TEST,		//����
	UW_GOLD_ZUAN,		//���˴�ת��
	UW_GOLD_AWARD,		//��ת���콱
	UW_GOLD_LUCK_AWAED,		//��ʯ��
	UW_GOLD_VIP_GIFT,		//VIP���
	UW_GOLD_EXPCOPY,		//Զ������ExpeditionCopy
	UW_GOLD_ACHIEVENT,		//�ɾͽ���
	UW_GOLD_ONLINEAWARD,		//���߽���
	UW_GOLD_LVUPAWARD,		//��������
	UW_GOLD_LOGINGIFT,		//�ۼƵ�½
	UW_GOLD_RECHARGE_AWARD,		//�׳影��
	UW_GOLD_SIGUP_MOUTH,	//ÿ��ǩ��
	UW_GOLD_DEGREEAWARD,	//��Ծ�Ƚ���

};

class DBControl;
class UserData;
class UserWealth
{
public:
	UserWealth(UserData* pUser);
	~UserWealth();

	int		GetWealth(BYTE bEnum)	{return	m_nWealths[bEnum];}

	bool	SubWealth(BYTE bEnum, int nVal,BYTE byOrigin = RMB_SAVE_NORMAL);		//��������false
	bool	AddWealth(BYTE bEnum, int nVal,BYTE byOrigin = RMB_SAVE_NORMAL,char* szDesc = NULL,BYTE byType = UW_GOLD_NULL);		//byOrigin����ʯ���������Ļ����ӣ���д��ԭ��
	bool	DBInitWealth(BYTE bEnum, int nVal);
	void	SendWealthSingle(BYTE bEnum , int nChangeVal,DWORD dwUin);

	void	SetExpWealthVal(DWORD);
	DWORD	GetExpWealthVal();
	bool	SubExpWealth(BYTE bEnum, DWORD nVal,BYTE byOrigin = RMB_SAVE_NORMAL);		//��������false

	void	SetTotalSend();

	int		operator[](BYTE bEnum)	{return m_nWealths[bEnum];}

	void	CheckSaveDB(DBControl* pDB,DWORD	dwUin,DWORD	dwAcc);
	void	SaveToDB(DBControl* pDB,DWORD	dwUin,DWORD	dwAcc);
	bool	SetWealth(BYTE bEnum, int nVal);
private:
	int		m_nWealths[UW_MAX];
	int		m_sznChgVal[UW_MAX];
	bool	m_bChged[UW_MAX];

	int		m_nGoldChangeVal;
	int		m_nExpChangeVal;
	UserData* m_pUser;
};