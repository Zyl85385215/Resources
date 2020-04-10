#pragma once

#include "singleton.h"
#include "windows.h"
#include "xnlist.h"

class MapLimitObj
{
public:
	MapLimitObj()
	{
		pListItem	= xnList::Create();
	}
	~MapLimitObj()
	{
		pListItem->Free();
	}
	bool		CheckItemCanUse(int nItemID);
	int			nMapIndex;
	bool		bLimitGroup;
	bool		bLimitSlave;		//1��ʾ�޷��ٻ����0��ʾ�����ٻ�����
	bool		bLimitItem;			//1��ʾֻ��������Ʒ���ã�0��ʾֻ��������Ʒ������
	bool		bLimitDeath;		//1��ʾû����������ͷ�,0��ʾ��
	bool		bLimitHeibaiGroup;  //1��ʾ������ڰ���ӣ�0�෴
	xnList*		pListItem;			//������Ʒ�б�
};

class MapLimitConfig : public CSingleton<MapLimitConfig>
{
public:
	MapLimitConfig();
	~MapLimitConfig();

	void	LoadFile(char* szFile);
	MapLimitObj*	GetMapLimit(int nMapIndex);		//��ͼ����ʱֱ�ӹ��ڵ�ͼ��

private:
	xnList*		m_pListMapLimit;
};