#pragma once


#include <Windows.h>
#include "ZNetPacket.h"
#include "xnlist.h"

#define CTRLVIEW_WIDTH		12
#define CTRLVIEW_HEIGHT		8

class MapAIObj;
class MapManager;
class ControlObj
{
public:
	ControlObj(DWORD	dwK);
	~ControlObj();

	//0ȡ������, 1ָ���Ϳ���, 10, �������� 11, ��Ұ����(10,11ֻ��ѡ��һ)
	void	SetCtrl(BYTE bControl, MapAIObj* pObj);

	void	RecvPacket(ZPacket* pPacket);
	void	SendPacket(ZPacket* pPacket);

	void	Update();

	void	ChgSP(int nSp);
	void	StrikeSkill(WORD wSkill);

	DWORD	m_dwKey;
	DWORD	m_dwUin;
	int		m_nMIndex;		//Ϊ0��ʾδ��ʼ��
	WORD	m_wPX,m_wPY;	//ԭ��������
	WORD	m_bForce;
	int		m_nSpVal;
	
	MapManager*	m_pRunMap;		//��ǰ���ڵ�ͼ
	MapAIObj*	m_pMainAIObj;	//��Ҫ���Ƶ�λ,�ӽ�����


	DWORD		m_dwDamageTotal;
	DWORD		m_dwHealthTotal;
	DWORD		m_dwHitTotal;

	bool	AddCtrlChild(MapAIObj* pObj);
	bool	RemoveCtrlChild(MapAIObj* pObj);
	xnList*		m_pListChild;	//������ս, �ڳ�, ����NPC��

	//��߿ɹ����Ƿ���ʾ�������,NPC,����,����֮���
	bool	GetCtrlAICommand(MapAIObj* pObj);
private:
	void	_setMainCtrl(MapAIObj* pObj, BYTE bCtrl);
};