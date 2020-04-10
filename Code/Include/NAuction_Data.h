#ifndef _NAUCTION_DATA_H_
#define	_NAUCTION_DATA_H_

#include <windows.h>
#include <time.h>
#pragma pack (push)
#pragma pack (1)

#define NEW_AUCTION_VERSION 0

enum
{
	NA_ITEM_OVERTIME,		//����
	NA_ITEM_SELLOUT,		//����

	NA_ITEM_NORMAL,			//������

	NA_ITEM_ERROR = 99,
};

enum
{
	NA_OP_ADD,
	NA_OP_ADD_ERROR,	//��Ʒ��������Ʒ
	NA_OP_BUY,
	NA_OP_BUY_OVER,	//���۱�����
	NA_OP_SELLOUT,
	NA_OP_OVERTIME,
	NA_OP_PICKOUT,
};

#define PNA_PACKET_DATA	BYTE*
#define PNA_OBJ_DATA	BYTE*
#define PNA_ITEM_DATA	BYTE*

#define NA_MAX_ITEMCNT	10

class NA_Object
{//���͸��ͻ�����Ʒ�ļ���Ϣ,�ÿͻ�������
public:

	NA_Object::NA_Object()
	{
		memset(this,0,sizeof(NA_Object));
		tmEnd = time(NULL) + 86400;
		byFlag = NA_ITEM_NORMAL;
	}

	NA_Object::~NA_Object()
	{
		if(pData)
			free(pData);
	}
	char*	GetItemName(char* szName)
	{
		strcpy(szName,szSearch);
		for (int i = 0; i < strlen(szName); i++)
		{
			if(szName[i] == '#')
			{
				szName[i] = 0;
				break;
			}
		}
		return szName;
	}
	DWORD  dwSeller;	//������
	DWORD  dwBuyer;		//��ǰ����߼۸���
	int	   nPriceCur;	//��ǰ��
	int	   nPriceMax;	//һ�ڼ�
	DWORD  dwKey;		//ʶ����
	LONGLONG	   llKey;		//��ƷINDEX
	BYTE   byType;		//��Ʒ����
	BYTE   byLvl;		//��Ʒ�ȼ�
	BYTE   byGoldType;
	char   szSearch[32];	//�����ؼ���,Ĭ��Ϊ��Ʒ����
	time_t tmEnd;		//time(NULL);��ʼ���۵�ʱ��
	BYTE   byFlag;		//��Ʒ״̬  0-->������, 1-->������
	PNA_OBJ_DATA  pData;	//PNA_OBJ_DATA
};

class NA_SearchKey
{//����������
public:
	NA_SearchKey()
	{
		memset(this,0,sizeof(NA_SearchKey));
	}
	DWORD dwSeller;
	DWORD dwBuyer;
	BYTE byType;
	BYTE byLvlMin;
	BYTE byLvlMax;
	char szKey[32];
	time_t tmStart;

};

struct NADATA_ItemBase
{
	WORD					m_wItemID;
	BYTE					m_bType;
	//BYTE					m_bSlot;								//	λ��,11��255��װ����λ0--10
	WORD					m_wDura;								//	�־�
	WORD					m_wDuraMax;
	WORD					m_wData;
	DWORD					m_dwItemIndex;							//	Ψһ��ʶ���һ����Ʒ
	DWORD					m_wDeadLine;
	DWORD					m_dwCreateTime;							//	����ʱ��ʱ��
	LONGLONG				m_lItemKey;								//	Ԥ��,��������

	BYTE					m_bDemonPowLvl;							//	ħ���ȼ�
	BYTE					m_bPlusLevel;
	int						m_nPlusScore;
};

#pragma pack (pop)
#endif