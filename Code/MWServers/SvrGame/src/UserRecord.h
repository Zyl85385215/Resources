#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "LuaStateManager.h"
#include "LuaClassObject.h"

enum	USER_RECORD_ACTION	//��������
{
	UA_NULL,
	UA_ENTER,		//����
	UA_LEAVE,		//�뿪
	UA_OPREATE,		//����
	UA_ATTACK,		//����
	UA_BEATTACK,	//������
	UA_HEAL,		//����
	UA_BEHEAL,		//������
	UA_KILL,		//ɱ��
	UA_BEKILLED,	//��ɱ��
	UA_RELIVE,		//����
	UA_BERELIVE,	//������
	UA_GAIN,		//���
	UA_LOST,		//ʧȥ
	UA_USE,			//ʹ��
	UA_COMPLETE,	//���
	UA_SEND,		//����
	UA_MAX,
};

enum	USER_RECORD_OBJECT	//��������
{
	UO_NULL,
	UO_MAP,			//��ͼ
	UO_MYSELF,		//�Լ�
	UO_PET,			//����
	UO_HORSE,		//����
	UO_PLAYER,		//���
	UO_ITEM,		//��Ʒ
	UO_MAPCOPY,		//����
	UO_QUEST,		//����
	UO_MESSAGE,		//��Ϣ
	UO_MAIL,		//�ʼ�
	UO_CROPS,		//ũ����
	UO_ENEMY,		//����
	UO_ACTIVITY,	//�
	UO_TIME,		//ʱ��
	UO_MAX,
};

class URInfoObj
{
public:
	BYTE	bAction;		//����
	BYTE	bObject;		//����
	DWORD	dwTick;			//����ʱ��
	int		nID;			//ID
	int		nVal;			//����ֵ����ͬ����ͬ��
};

enum{
	RMB_SAVE_NULL,
	RMB_SAVE_NORMAL,		//Ĭ�ϣ�Ҳ����δ֪
	RMB_SAVE_RECHARGE,		//��ֵ
	RMB_SAVE_SHOPBUY,		//�̳ǹ���
	RMB_SAVE_DEBUGCODE,		//DEBUG����
	RMB_SAVE_FORT_UNLOCKMANOR,	//����ׯ԰
	RMB_SAVE_SIGNUP_MOUNTH,		//ÿ��ǩ��
	RMB_SAVE_EXPANDBAG,		//��չ����
	RMB_SAVE_ONLINEAWARD,		//���߽���
	RMB_SAVE_LVUPAWARD,		//��������
	RMB_SAVE_RECHARGEAWARD,		//�׳佱��
	RMB_SAVE_DEGREEAWARD,		//��Ծ�Ƚ���
	RMB_SAVE_RESETSKILL,		//���ܵ�����
	RMB_SAVE_MCCARD,		//��������
	RMB_SAVE_GAMBLE,		//�Ĳ���
	RMB_SAVE_TURNTABLE,		//��ת��
	RMB_SAVE_SLAVE_GET,		//�����
	RMB_SAVE_HORSE_CULTIVATE,		//��������
	RMB_SAVE_GREENMODE,		//����ξ�

	RMB_SAVE_TPCOMMPAY,		//��ֵ
	RMB_SAVE_ACHIEVEMENT,	//�ɾͽ���
	RMB_SAVE_MARRY,	//���
	RMB_SAVE_INVITE,
	RMB_SAVE_SCRPIT,
	RMB_SAVE_WAR_RANK,	//������
	RMB_SAVE_FAMILY,	//����
	RMB_SAVE_EQUIP,	//��װ��
	RMB_SAVE_MAKE_SKILL,	//�����
	RMB_SAVE_MAKE_POWER,	//�����
	RMB_SAVE_LOGINGIFT,	//�ۼƵ�½
	RMB_SAVE_MENTOR,	//ʦͽ
};

static char* szRmbSaveName[] = {"�ޣ����ߣ�",
	"δ֪","��ֵ","�̳ǹ���","debug����","����ׯ԰",
	"ÿ��ǩ��(��ǩ����)","��չ����","���߽���","��������","�׳佱��",
	"��Ծ�Ƚ���","���ܵ�����","��������","�ڿ��","��ת��",
	"�����","��������","����ξ�","��ֵ","�ɾͽ���",
	"������","�����뽱��","��ű���","������","����","����װ��",
	"�����","�����","�ۼƵ�½","ʦͽ"};


enum{
	SLAVE_SAVE_NORMAL,
	SLAVE_SAVE_GETPET,
	SLAVE_SAVE_REMOVEPET,
	SLAVE_SAVE_GETHORSE,
	SLAVE_SAVE_REMOVEHORSE,
	SLAVE_SAVE_PET_LVUP,
	SLAVE_SAVE_HORSE_LVUP,
	SLAVE_SAVE_PET_EXCHANGE,
	SLAVE_SAVE_PET_VARIATY,
	SLAVE_SAVE_PET_VFAIL,
};

static char* szSlaveSaveName[] = {"δ֪","��ó���","�Ƴ�����","�������","�Ƴ�����","��������","��������","�������","�������","�������ʧ��"};

//�����־�Լ����������¼�
class UserData;
class ItemObj;
class SlaveBaseData;
class HorseBaseData;
class UserRecord:public CLuaClassObject<UserRecord> 
{
public:
	UserRecord(UserData* pUser);
	~UserRecord();

	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);

	void		AddRecord(BYTE	bUA,BYTE byUO, int nID, int nVal);
	void		Update();
	//��ѯһ��ʱ���ڵ�ĳ������
	bool		CheckRecord(BYTE bUR, int nTick);

	bool		_checkDegree(URInfoObj* pObj);
	bool		_checkAchievement(URInfoObj* pObj);

	xnList*		m_pList;	//����һ������/ʱ��
	UserData*	m_pParent;

	///////////////////////////LUA����/////////////////////////////////
	bool		RunLuaQuestFunc(BYTE byAction,int nQID,BYTE byType);
	bool		RunLuaChatFunc(BYTE byChannel);
	bool		RunLuaDegreeFunc(int nPoint);
	bool		RunLuaActivityFunc(int nID,int nVal1,int nVal2);
	bool		RunLuaKillEnemyFunc(int nID,bool ifWBoss);
	bool		RunLuaRelationFunc(int nAction,int nTarid);
	bool		RunLuaFamilyFunc(int nAction,int nVal);
	bool		RunLuaStarFunc(int nLv);
	bool		RunLuaEquipFunc(int nAction,ItemObj* pItem,ItemObj* pItem2);
	bool		RunLuaPetFunc(int nAction,SlaveBaseData* pPet);
	bool		RunLuaHorseFunc(int nAction,HorseBaseData* pPet,int nNum);
	bool		RunLuaMapCopyFunc(int nAction,int nID,int nLevel,int nScore);
	bool		RunLuaPVPFunc(int nCount);
	bool		RunLuaWealthFunc(int nType,int nGet,int nTotal);
	bool		RunLuaSkillFunc(int nAction,int nID,int nType,int nLv);
	bool		RunLuaFarmFunc(int nAction,int nVal);
	bool		RunLuaDeadFunc();
	bool		RunLuaMarryFunc(BYTE byAction,int nTarget,int nPoint1,int nPoint2);

	//////////////////////////��־����/////////////////////////////////
	void		SaveAllLog();

	bool		AddGoldLog(int nVal);
	bool		AddGoldFromLog(int nAddVal, int nVal, BYTE byType);
	bool		AddExpLog(int nVal,BYTE byLvl);
	bool		AddRmbLog(int nLeftVal,int nVal,BYTE byType,char* szDesc=NULL);
	bool		AddBindRmbLog(int nLeftVal,int nVal,BYTE byType);
	bool		AddItemLog(int nItemID,int nCount,int nType);
	bool		AddSlaveLog(int nID,int nBaseid,BYTE bylvl,BYTE byVariatlv,BYTE byAction);
	bool		AddGMCodeLog(const char* szMsg);
	bool		AddItemChangeLog(int nItemID,int nCount,int nFrom,BYTE byType);
	bool		AddLvupLog(BYTE byLv);
	bool		AddEquipStarLog( int nItemID );
	bool		AddEquipStarLogEx( int nItemID,int nCnt,int nStarLv );
	bool		AddSlaveLvLog( int nSlaveID,int nBaseID,int nPos,int nPosLv  );
	bool		AddFortLog( int nChgScore,int nWarScore,int nCurPos,int nManorLv ,int nManorExp, BYTE byType );
};

class WorldLog
{
public:
	WorldLog();
	void		UpdateSec();
	void		SaveAll();
	bool		SaveRankLog();
	bool		SaveUserInfoLog();
	bool		SaveWarRankLog();

	bool		ifSave;
	DWORD		dwSaveTick;
};