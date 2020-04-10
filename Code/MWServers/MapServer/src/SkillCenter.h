#pragma once

#include "SkillConfig.h"
#include "ios_mapcmd_packet.h"

class MapAIObj;
class SkillObj
{
public:
	SkillObj(WORD	wID,BYTE	bLv);
	~SkillObj();

	void		ResetCD(MapAIObj* pParent);		//��������CD
	bool		CheckTargetRange(int	nDis);	//��Ŀ��ľ����ܷ��ͷ�
	void		SetLv(BYTE	bLv);

	SkillBase*	pSBase;
	SEffLvInfo*	pLvBase;
	BYTE		bySLv;
	BYTE		byPos;
	DWORD		dwCDTick;	//�´�CD�õ�ʱ���

	WORD		wUserTCnt;	//����ʹ�ô���
};

class SkillCenter
{
public:
	SkillCenter(MapAIObj* pObj);
	~SkillCenter();

	SkillObj*	CheckUserSkill(WORD	wSID);		//��⼼���Ƿ���CD�Ϳ��ͷ�����
	SkillObj*	AutoUserSkill();
	void		SetSkill(WORD wSID, BYTE bLv,BYTE	bPos, DWORD	dwCD);	//���¼�����Ϣ
	void		ResetJobSkill();
	SkillObj*	GetSkill(WORD	wSID);

	xnList*		m_pListSkill;
	SkillObj*	m_ppActSkill[MAX_ACTIVE_SKILL];
	MapAIObj*	m_pParent;

	void		StrikeSkill(WORD wSkill);
	WORD		m_wStrikeSkill;
};