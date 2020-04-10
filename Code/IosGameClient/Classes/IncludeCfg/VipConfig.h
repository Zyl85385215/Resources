#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <vector>
#include <string>


struct VipInfoObj
{
	int m_nLv;
	int m_nNeedgold;							//���������ֵ
	int	m_nExpadd;								//����ӳ�  ID
	int	m_nTaxsub;								//˰�ռ���  �ٷֱ�
	int	m_nPrestigeadd;						//�����ӳ�  �ٷֱ�
	int	m_nField;								//�������Ӹ���
	bool	m_fixswitch;						//�һ�������
	bool	m_chatswitch;						//������Ч����
	bool	m_buyswitch;						//�һ����򿪹�
	int	m_nPoweradd;							//���������
	int	m_nCardtimes;							//�������ⷭ�ƴ���
	bool	m_proclamtion;					//���߹��濪��
	bool	m_skillswitch;						//�һ��Զ��ͷŴ�������
	std::vector<std::string>	vecdesc;	//���������мӡ�\n��
};

class VipConfig : public CSingleton<VipConfig>
{
public:
	VipConfig();
	~VipConfig();

	VipInfoObj* GetObjByLv(int nLv);
	void LoadFile(char* pFile);
	int GetMaxLvl() { return m_pList->Count;}
private:
	xnList* m_pList;
};

struct VipAward
{
	int      nGold;
	xnList*  pList;
};

class VipAwardConfig : public CSingleton<VipAwardConfig>
{
public:
	VipAwardConfig();
	~VipAwardConfig();

	VipAward* GetObjByVipLv(int nLv);
	void LoadFile(char* pFile);
private:
	xnList* m_pList;
};