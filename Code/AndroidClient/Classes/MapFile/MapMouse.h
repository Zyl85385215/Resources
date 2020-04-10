#pragma once

#include "MapControl.h"
/*----------------------------------------------------------------------
�趨����ƶ�
ѡ��Ŀ��


------------------------------------------------------------------------*/
//��ͼ�ϵĿ����¼���������ת����ת�ӵ�����
class MapEventCenter
{
public:
	MapEventCenter();
	~MapEventCenter();

	void	MapEventMove(int nX, int nY);
	bool	MapEventDown(int nX, int nY);
	void	MapEventUp(int nX, int nY);

	void	Update();

	int		m_nClickX;
	int		m_nClickY;
	//bool	m_bClickHold;	//��ָһֱ��ס
	DWORD	m_dwTickMove;	//�ϴ��ƶ�ָ��
};

class MapAIObj;
class _BaseGuidePt;
class MiniMapMenu;
class MapCommand
{
	friend class MiniMapMenu;
public:
	MapCommand();
	~MapCommand();

	void	Reset(bool bMapTrans = false);

	bool	CmdPathObj(_BaseGuidePt* pGC);
	bool	CmdMapPoint(int nMIndex, int nPosX, int nPosY, _BaseGuidePt* pGC = NULL);
	bool	CmdPoint(int nPosX, int nPosY,bool bReset = true, bool bFromAuto = false);
	bool	CmdNormalAtk(MapAIObj*	pObj,bool bClearSkill = true);
	bool	CmdSkillAtk(MapAIObj*	pObj,WORD wSID);
	//��ǰ��ͼ���¸��ƶ���,����ͼʧ�ܾͲ���������
	bool	PopNextPoint(int nMap, POINT& pt);

	bool	IsInRest();	//����״̬
	bool	IsMoving();
	//���ƶ���ʱ����ж���û����Ŀ��
	MapAIObj*	PopAtkCmd(int& nSkill);

	MapAIObj*	m_pObjNAtk;		//�ȼ�Ŀ��

	//int		m_nTrackCmd;	//-1��ʾ���һ�, ���� ��NPC�Ի�
	_BaseGuidePt*	pGuideCmd;
	WORD	m_wTrySkill;
	bool	m_bFromAuto;
	int		m_nLastPX;
	int		m_nLastPY;
	int		m_nLastMIndex;	
protected:
	int		m_nPathMIndex;	//��ǰѰ·�����ͼ

	bool	m_bReCmd;		//ָ�������´�
	xnList*	m_pLPtPath;		//Ѱ·�ڵ�
	xnList*	m_pLMapTrans;	//���ŵ�ͼ�����ת��
};

//����ʶ������,ͨ��move�¼����ô���, down,up�¼����
class HandSignCmd
{
public:
	HandSignCmd();
	~HandSignCmd();

	BYTE	AddPtCheck(int nX, int nY);		//0��, 1��  2��  3��  4�� 5�� 6\	7 5��,8	6��
	void	ClearPt();

private:
	BYTE	_AnalyPtList();
	bool	_checkPts(BYTE bType);
	BYTE	_checkPt(int nSX, int nSY, int nEX, int nEY);
	DWORD		dwTickCmd;
	xnList*		pListPt;
};

extern	HandSignCmd*	g_pHandSign;
extern	MapCommand*	g_pMapCommand;