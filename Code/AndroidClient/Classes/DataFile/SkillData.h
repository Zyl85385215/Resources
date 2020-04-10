#pragma once

#include "SkillConfig.h"
#include "ios_mapcmd_packet.h"

class SkillObj
{
public:
	SkillObj(WORD	wID,BYTE	bLv);
	~SkillObj();

	//void		ResetCD();		//��������CD
	bool		CheckTargetRange(int	nDis);	//��Ŀ��ľ����ܷ��ͷ�
	void		SetLv(BYTE	bLv);

	SkillBase*	pSBase;
	SEffLvInfo*	pLvBase;
	BYTE		bySLv;
	DWORD		dwCDTick;	//�´�CD�õ�ʱ���
};

class SkillCenter
{
public:
	SkillCenter();
	~SkillCenter();

	SkillObj*	CheckUserSkill(WORD	wSID);		//��⼼���Ƿ���CD�Ϳ��ͷ�����
	void		SetActSkill(WORD wSID, BYTE bLv,BYTE bPos,int nCD);	//���¼�����Ϣ
	SkillObj*	GetActiveSkill(WORD	wSID);
	void		ResetSkillCD(WORD wSID);
	void		ClearActSkill();
	void		ClearSkill();

	void		RemoveSkill(WORD wSID);
	SkillObj*	SetSkill(WORD wSID, BYTE bLv);
	SkillObj*	GetSkill(WORD	wSID);


	xnList*		m_pListSkill;		//����ѧ��ļ���
	xnList*		m_pListSPSkill;

	int		CheckSkillSetting(WORD wSID);
	SkillObj*	m_ppActSkill[MAX_ACTIVE_SKILL];
	//xnList*		m_pMObjSkill;		//���ƶ���Ŀ�ݼ�����
};