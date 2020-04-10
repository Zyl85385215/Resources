#ifndef ITEM_LEVELUP_PACKET_H
#define ITEM_LEVELUP_PACKET_H

#include "item_packet.h"

#pragma pack (push)
#pragma pack (1)


#define MAX_MOHUA_LEVEL 15
#define MAX_PLUS_LEVEL 15
#define MOHUA_STONE_ID 43161
#define MOHUA_ADVSTONE_ID 43162
#define MOHUA_PROTECT_ID 43163
#define SMELT_STONE_ID 43170
#define UPDATE_STONE_ID 43171 //������ʯ
#define UPDATE_STONEEX_ID 43159	//�߼�
#define UPDATE_STONEEX2_ID 43153	//�߼�2
#define LUCKY_STONE_ID 43160	//�߼�

struct ItemExtend;
typedef bool (ItemExtend::*LoadItemExtend)(BYTE * pbData, int nSize);
typedef bool (ItemExtend::*SetItemExtend)(BYTE * pbData, int &nSize);

enum
{
	ITEMEXTEND_VERSION_0,
	ITEMEXTEND_VERSION_MAX
};

enum 
{
	itemextend_db_level,
	itemextend_db_apN,
	itemextend_db_apW,
	itemextend_db_atkN,
	itemextend_db_atkW,
	itemextend_db_atkSpeed,
	itemextend_db_moveSpeed,
	itemextend_db_defN,
	itemextend_db_defW,
	itemextend_db_dpN,
	itemextend_db_dpW,
	itemextend_db_maxHp,
	itemextend_db_maxMp,
	itemextend_db_ppN,
	itemextend_db_ppW,
	itemextend_db_maxDura,

	//�츳����
	itemextend_db_str,
	itemextend_db_con,
	itemextend_db_ing,
	itemextend_db_dex,
	itemextend_db_wis,
	itemextend_db_luk,

	//��������
	itemextend_db_rebuildTimes,

	//��������
	itemextend_db_type,
};

extern LoadItemExtend m_pLoadItemExtend[ITEMEXTEND_VERSION_MAX];
extern SetItemExtend  m_pSetItemExtend[ITEMEXTEND_VERSION_MAX];

struct ItemExtend:public ItemExtendInfo
{
public:
	bool		LoadItemExtend(BYTE * pbData, int nSize)
	{
		return (this->*m_pLoadItemExtend[*(int*)pbData])(pbData, nSize);
	}
	bool		SetItemExtend(BYTE * pbData, int & nSize, int nVersion)
	{
		if (nVersion < ITEMEXTEND_VERSION_0 || nVersion >= ITEMEXTEND_VERSION_MAX)
			return false;
		return (this->*m_pSetItemExtend[nVersion])(pbData, nSize);
	}
	bool		LoadItemExtend0(BYTE * pbData, int nSize);
	bool		SetItemExtend0(BYTE * pbData, int & nSize);
};


enum _ITEM_LEVELUP
{	
	ITEM_LEVELUP_OPEN,
	ITEM_LEVELUP_START,   // NPC����װ��	
	ITEM_LEVELUP_TIMEUP,  // ������Ҫʱ�䵽
	ITEM_LEVELUP_RESULT,  // �жϽ��
	ITEM_LEVELUP_END,     // �رմ���
	ITEM_LEVELUP_CANCEL,  // ȡ������
	ITEM_MOHUA_OPEN,	  // 
	ITEM_SMELT_OPEN,	  //
	ITEM_PLUS_OPEN,
	RIDE_REFRESH_OPEN,
	TRUMP_REFRESH_OPEN,
	ITEM_PLUS_RESET_OPEN, //�����ܽ���
	ITEM_PLUS_RESET_EX,//���������ܽ���
	ITEM_PLUS_EX_OPEN,      //���Կ����������
	ITEM_PLUS_EX_CLOSE,     //�ر��������
	ITEM_PLUS_EX_ACCEPT,    //������������
	ITEM_PLUS_EX_CANCEL,    //ȡ����������
};

enum
{
	item_levelup_rlt_success,    // �ɹ�
	item_levelup_rlt_failed,     // ʧ��

	item_levelup_rlt_checkpass,  // ���ͨ��
	item_levelup_rlt_checkfailed // ��Ʒ����
};  

enum
{
	ITEM_LEVELUP_STATUS_NONE,
	ITEM_LEVELUP_STATUS_DOING,
	ITEM_LEVELUP_STATUS_END
};

struct ITEM_PLUS_ITEM_PLUS_OPEN_Data
{
	BYTE flag;
};

struct ITEM_PLUS_ITEM_LEVELUP_OPEN_Data
{
	BYTE flag;
};

struct ITEMLEVELUP_ITEM_LEVELUP_OPEN_Data 
{
	BYTE flag;
};

struct ITEMLEVELUP_ITEM_LEVELUP_START_Data 
{
	DWORD	dwEquitIndex;          //װ��
	bool	bProtect;
	DWORD	dwProtectIndex;
	BYTE	bCount/*[4]*/;             //��ʯ����
	DWORD	dwItemIndex/*[4]*/;        //������ʯ
};

struct ITEMLEVELUP_ITEM_LEVELUP_RESULT_Data 
{		
	BYTE		bRlt;	
	DWORD		index;
};

struct ITEMMOHUA_ITEM_LEVELUP_START_Data 
{
	DWORD	dwEquitIndex;          //װ��  
	DWORD	dwProtect;
	BYTE	byAdvMohua;
};

struct ITEMSMELT_ITEM_LEVELUP_START_Data 
{
	DWORD	dwEquitIndex;          //װ��    
};

struct ITEM_PLUS_ITEM_LEVELUP_START_Data
{
	int dwBaseEquipIndex;
	int dwPlusEquipIndex;
	WORD	wUseCnt;
};

struct ITEM_PLUS_ITEM_PLUS_RESET_EX_Data
{
	int dwBaseEquipIndex;
	int dwPlusEquipIndex;
};

struct ITEM_PLUS_ITEM_PLUS_EX_ACCEPT_Data
{
	int  dwBaseEquipIndex;
};

struct ITEM_PLUS_ITEM_PLUS_EX_CANCEL_Data
{
	int  dwBaseEquipIndex;
};


struct ITEMMOHUA_ITEM_LEVELUP_RESULT_Data 
{		
	BYTE		bRlt;		
};

struct ITEM_PLUS_ITEM_LEVELUP_RESULT_Data
{
	BYTE		bRlt;
};

struct ITEMSMELT_ITEM_LEVELUP_RESULT_Data 
{		
	BYTE		bRlt;		
};

struct RIDE_PLUS_ITEM_LEVELUP_RESULT_Data
{
	BYTE		bRlt;
};

#pragma pack (pop)
#endif