#pragma once

#include "xnlist.h"
#include "singleton.h"

/*--------------------------------------------------------
���Ƶ�ͼ����Ĳ���֡���

�ᶯ�Ĺֵ������ռ�����
�ϲ�����ͨ������ͶӰ����ͼ��
--------------------------------------------------------*/
class   MapAIObj;
class	MapSprObj;
class	MapObj;

class	MObjActCtrl
{
public:
	MObjActCtrl();
	~MObjActCtrl();

	//���ú�����MapSprObj, ������ά��
	bool	LinkCtrl(MapAIObj* pMObj);
	//����ɾ����ʱ������ģ��ɾ������
	bool	DisLinkCtrl(MapObj* pMObj);
	void	ResetObj();
	void	GetInViewObj(int nPosX, int nPosY, int nEX, int nEY,xnList* pLOut);
	void	Update();

	MapSprObj*	CheckSelObj(int nX, int nY);
	MapSprObj*	SearchObj(int nX, int nY,BYTE bEnemy,DWORD	nMinDistance, DWORD dwQusEnemyID = 0);
	MapSprObj*	GetObjByID(BYTE	byType, int	nID);

	xnList*	m_pListAllMObj;
	xnList*	m_pListCache;
private:
	void	_checkPlaySound(BYTE bType, BYTE bSex,BYTE bJob, BYTE bAct, BYTE bFrm,char*	szSkinSound = NULL);
};