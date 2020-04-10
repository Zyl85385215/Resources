#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "GlobalDefine.h"
#include "EffectObj.h"
#include "SkinConfig.h"
#include "ios_mapobj_packet.h"

enum
{
	OBJ_STATE_STAND,
	OBJ_STATE_RUN,
	OBJ_STATE_ATK,
	OBJ_STATE_SKILL,
	//OBJ_STATE_HURT,		//�ܻ�����ֻ�ڿͻ�����ʾ
	
	OBJ_STATE_MAX,
};

//��ͼ�������(������,��ͼ��Ч��),��Ŀ�귶Χ��Ч, ���й�Ч�ɲ����������
class	MapObj
{
public:
	MapObj();
	virtual ~MapObj();


	//����һ����Ч,�Ƚ��б�, ����ѭ����Ч�б�ȶ�����,��ѭ����Чֱ�Ӳ���һ�ξ�OK��
	bool		AddEffect(EffectObj* pEff);
	void		SetAlpha(BYTE byVal);

	int			m_nID;			
	BYTE		m_byType;		//0, ����������, 1 ����, 2����, 3����, 4,NPC
	DWORD		m_nMIndex;
	int			nPosX;
	int			nPosY;
};