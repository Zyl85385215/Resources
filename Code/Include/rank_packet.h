#ifndef _RANK_PACKET_H_
#define _RANK_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define _RANK_MAX_ 20

enum 
{
	RANK_LIST,
	RANK_DATA,
	RANK_VIEW,
	RANK_VIEW_EX,
	VIEW_PLAYER,
	VIEW_REQUIP,
	RANK_PERSON,

	ADD_SLAVE_INFO,
};

enum RANKTYPE                     //���а�����
{
	RANK_HERO,//�ȼ�
	RANK_WEATH,//�Ƹ�
	RANK_KILLER,//����
	RANK_MAJOR,  //����
	RANK_ZHANSHI,//��ʿ
	RANK_XIAKE,//����
	RANK_DAOSHI,//��ʿ
	RANK_YAOSHI,//ҩ
	RANK_MAJOR_SHANG,//������Ѯ�����أ�
	RANK_MAJOR_ZHONG,//������Ѯ�����أ�
	RANK_MAJOR_XIA,//������Ѯ�����أ�
	RANK_HOME,//���
	RANK_COUNTRY,//���ң����أ�
	RANK_HONOUR,//�ٽ�
	RANK_XUANXIU_MAN,//�����أ�
	RANK_XUANXIU_WOM,//�����أ�
	RANK_EQUIP,//װ��
	RANK_TRUMP,//�����أ�
	RANK_FLOWER,//�����أ�
	RANK_INTERER_SCORE,//����
	RANK_ARENA_SCORE,
	RANK_ACTEX_POINT,
	RANK_SLAVE,
	RANK_MARRY_POINT,
	RANK_HOME_CONTRIBUTION,	//������
	RANK_TEACH_MONEY,	//��ʦ���
	RANK_TEACH_STUDENTS,	//��ʦ��ͽ
	RANK_EQUIP_SCORE,	//װ������
	RANK_FLOWER_TOTAL_POINT,
	RANK_FLOWER_DAY_POINT,
	RANK_FLOWER_WEEK_POINT,
	RANK_FLOWER_MONTH_POINT,
	RANK_TOTAL,//ע�����ϵ�ö��ֵһһ��Ӧ�ڽ����ϵĿؼ�����INTERFACE���ߴ�Interface.IFP��order�Ľ����ϣ��ؼ����ڣ�������Щ��������
};


enum COUNTRYRANKTYPE                     //�����Ե����а�
{
	COUNTRY_ALL,
	COUNTRY_QIGUO,
	COUNTRY_CHUGUO,
	COUNTRY_ZHAOGUO,
	COUNTRY_WUGUO,
	COUNTRY_QINGGUO,
	COUNTRY_TOTAL,
};

struct RANK_RANK_LIST_Data 
{
	BYTE       nCount;
	BYTE       byType;
	BYTE	   byCountry;
	//char       szTitle[1];
};

struct RANK_RANK_DATA_Data 
{
	DWORD      dwUin;
	DWORD      dwVal1;
	DWORD      dwVal2;
	BYTE       byPosition;
	BOOLEAN    bOnline;
	char       szName[1];
};

struct  RANK_ADD_SLAVE_INFO_Data
{
	DWORD	dwPoint;
	//DWORD	dwSlaveUin;
	DWORD	dwUin;
	char	szName[1];
};


struct RANK_RANK_PERSON_Data
{
	DWORD dwOrder;
};

struct RANK_VIEW_REQUIP_Data 
{
	DWORD      dwIndex;
	DWORD      dwPoint;
	DWORD      dwItemID;
	BYTE       byPosition;
	BYTE		byType;
	BYTE		byCountry;
	char       szName[1];
};

struct RANK_RANK_VIEW_Data 
{
	DWORD      dwUin;
	WORD       wRankIndex;
	BYTE       byCountry;
	BYTE       byOnlySelf; // ֻȡ����������Ϣ add by zp
};

struct RANK_RANK_VIEW_EX_Data 
{
	DWORD      dwUin;
	WORD       wRankIndex;
	BYTE	   byCountry;
	BYTE       byStart;
	BYTE	   byCnt;
};

struct RANK_VIEW_PLAYER_Data 
{
	DWORD      dwUin;
	char	   szDest[1];
};

#pragma pack (pop)
#endif