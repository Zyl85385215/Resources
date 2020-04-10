#ifndef _HOME_PACKET_H_
#define _HOME_PACKET_H_
#include "groupcmd.h"
#include <windows.h>
#include "item_packet.h"
#include "item_levelup_packet.h"
//#define CREATE_HOME_ITEM        45181         //���ݽ���ͼ
#define CREATE_HOME_ITEM        15111         //���ּ�����
#define REQUEST_HOME_ITEM_PIECE 15133
#define GIVE_CREATE_HOME_ITEM   15112         //������
#define CREATE_HOME_COST		1000000       //�����������Ҫ����Ϸ��   
#define CREATE_HOME_LEVEL		30            //�������ʱ�ӳ��ĵȼ�����
#define CREATE_HOME_MINPLAYER   5             //�������ʱ�������С��ģ
#define HOME_MAX_LEVEL          20             //���ĵȼ�����
#define HOME_MAX_COLEADER       1             //���峤����������
/*#define HOME_MAX_OLDER          3             //Ԫ�ϵ���������*/
#define MAX_HOMENAME            20            //���������󳤶�---10������
#define MAX_DUTYNAME            20            //���ְλ������󳤶�---10������ 
#define ITEM_RESOURCEID         40000   
#define REBUILDDAY              3
#define REBUILDHOUR             72
#define REBUILD_COST            1000000

#define PERMITLEADER            2031          //�峤Ȩ��
#define PERMITCOLEADER          119           //���峤Ȩ��  
#define PERMITOLDER             19           //Ԫ��Ȩ��
#define PERMITMEMBER            0             //��ԱȨ��

struct HomeSkill  
{
	WORD wSkillID;
	BYTE bLevel;
	DWORD dwLevelUpTime;	//��ʼ������ʱ��
};


class HomeProducts
{
public:
	BYTE	m_bType;
	bool	m_bIsGrowing;
	int		m_nLevel;
	int		m_nTotalCost;
	int		m_nBankMoney;
};

enum
{
	HOME_PRODUCTS_TYPE_DRUG,
	HOME_PRODUCTS_TYPE_WEAPON,
	HOME_PRODUCTS_TYPE_ARMOR,
	HOME_PRODUCTS_TYPE_ACCOUTERMENT,
	HOME_PRODUCTS_TYPE_MAX,
};

enum 
{
	HOME_HONOUR_NULL,
	HOME_HONOUR_BDKILL,
	HOME_HONOUR_KILL,
	HOME_HONOUR_SCRIPT,
	HOME_HONOUR_MAX,
};

enum
{
	HOME_JUAN_NULL,
	HOME_JUAN_ADD,
	HOME_JUAN_BUILD,
	HOME_JUANQIAN,//��Ǯ
	HOME_JUANGOLD,//��Ԫ��
};

enum
{
	HOME_DOWNLEVEL_WARN,
	HOME_DOWNLEVEL_TIP,
};

struct HomeWarInf
{
	 int		Week;
	 char	szHomeName[48];

};

enum HOME_PERMIT
{
	HOME_PERMIT_MEMBERADD=1,		          // �����³�Ա         				
	HOME_PERMIT_MEMBERREMOVE=2,		          // ɾ����Ա/�˳����		
	HOME_PERMIT_CHGNOTIFY=4,			      // �޸İ�ṫ��
	HOME_PERMIT_CHGPERMIT=8,		          // �޸İ���ԱȨ��
	HOME_PERMIT_REBUILD=16,                   // �Լ� 
	HOME_PERMIT_RESOURCELEVEL=32,             // ��Դ����
	HOME_PERMIT_ENFORCELEVEL=64,              // ǿ������
	HOME_PERMIT_LEVELUP=128,                  // �������
	HOME_PERMIT_GIVEQUEST=256,                // ��������--
	HOME_PERMIT_GETMENAGEPOINT=512,           // ��ȡ�����
    HOME_PERMIT_ITEMS=1024,                   // �ֿ����
};

enum HOME_DUTY
{
	HOME_DUTY_LEADER,                  //�峤
	HOME_DUTY_COLEADER,                //���峤
 	HOME_DUTY_OLDER,                   //Ԫ��
 	HOME_DUTY_MEMBER,                  //��Ա
	HOME_DUTY_MAX,       
};

enum _CreateHome
{
	CREATE_HOME_LOCKED,
	CREATE_HOME_UNLOCKERD,
	CREATE_HOME_FAILED,
};
static int  g_nEnForce[11]={0,3,6,9,12,17,22,27,34,41,50};
enum PLAYER_ENFORCE
{
	ENFORCE_HP,
	ENFORCE_MP,
	ENFORCE_ATKW,
	ENFORCE_ATKN,
	ENFORCE_APN,
	ENFORCE_DEFW,
	ENFORCE_DEFN,
	ENFORCE_PPN,
	ENFORCE_DPW,
    ENFORCE_MAX,
}; 

#pragma pack (push)
#pragma pack (1)

enum
{
	SEND_HOME_BUILD,									   //���Ͱ�Ὠ����Ϥ
	SEND_HOME_MESSAGE,                                     //���Ͱ����ʾ��Ϣ
	SEND_HOME_MESSAGECODE,								   //���Ͱ����ʾ
	SEND_HOME_INFO,                                        //���Ͱ�������Ϣ
	SEND_HOME_MP,                                          //���Ͱ���ʽ𡢷��ٶ�
	SEND_HOME_RESOURCE,                                    //���Ͱ����Դ���
	SEND_HOME_ENFORCE,                                     //���Ͱ��ǿ�����
	SEND_HOME_DUTYSETTING,                                 //���Ͱ��ְλ���
	SEND_HOME_MSGCHANGE,                                   //���Ͱ����ּ�仯
	SEND_HOME_LEVELCHANGE,                                 //���Ͱ��ȼ��仯
	SEND_HOME_REBUILD,                                     //���Ͱ���Լ���Ϣ

    HOME_MEMBER_ADD,                                       //����г�Ա����
	HOME_MEMBER_REMOVE,                                    //����г�Ա�뿪

	HOME_MEMBER_DUTYCHANGE,                                //��Աְλ�����仯    
	HOME_MEMBER_JOBCHANGE,                                 //��Աְҵ�����仯
	HOME_MEMBER_LEVELCHANGE,                               //��Ա�ȼ������仯  
	HOME_MEMBER_LOGONOFF,                                  //��Ա�ϡ�����
	HOME_MEMBER_POWERCHANGE,                               //��Ա���׶ȷ����仯 

	REQUEST_CREATE_HOME,                                   //���봴�����
	REQUEST_DISMISS_HOME,                                  //�����ɢ���

    REQUEST_GETRESOURCE,                                   //������ȡ�����Դ
	REQUEST_ADDHOME_MEMBER,                                //�������ĳ��Ա
	REQUEST_LEAVE_HOME,                                    //�����뿪���
	REQUEST_REMOVEHOME_MEMBER,                             //����ɾ��ĳ��Ա
	REQUEST_CHANGE_MEMBERDUTY,                             //����ı��Աְλ
	REQUEST_CHANGE_HOMEMSG,                                //�����޸İ������
	REQUEST_REBUILD_HOME,                                  //�����Լ�
	REQUEST_GETMENAGE,                                     //������ȡ�������
	REQUEST_LEVELUP,                                       //��������

	RESULT_HOME_PACKET,                                    //������
	ADD_HOME_MP,
	ADD_MEMBER_POWER,
	MEMBER_ENFORCE,

	JOIN_HOME_WAR,											//�μӰ��ս��
	REQUEST_JOIN_HOME_MAP,									//����������ͼ
	JOIN_HOME_MAP,											//�������ͼ

	HOME_MEMBER_LOGOUTTIMECHANGE,							//����ʱ��仯
	REQUEST_HOME_SKILL_LIST,								//
	SEND_HOME_SKILL_LIST,									//
	ADD_HOME_SKILL,
	SEND_ADD_HOME_SKILL,
	UPGRADE_HOME_SKILL,
	SEND_UPGRADE_HOME_SKILL,
	SEND_HOME_SKILL_RESULT,

	REQUEST_LEAGUE,											//����ͬ��
	ALLOW_LEAGUE,											//��������
	REQUEST_LEAGUE_RESULT,
	ALLOW_LEAGUE_RESULT,
	SEND_LEAGUE_LIST,
	SEND_ADD_LEAGUE,
	SEND_DEL_LEAGUE,

	REQUEST_JOIN,
	CANCEL_JOIN,
	REQUEST_REQUEST_LIST,
	REQUEST_HOME_SINFO_LIST,
	SEND_REQUEST_LIST,
	SEND_HOME_SINFO,
	ALLOW_REQUEST_JOIN,
	CANCEL_REQUEST_JOIN,
	HOMELIST_OPEN,

	REQUEST_CHANGE_HOMENAME,
	CHANGE_HOME_ICON,										//�޸İ��ͼ��
	REQUEST_JUANQIAN,										//����Ǯ
	REQUEST_OPEN_PRODUCTS,
	REQUEST_ACTIVE_PRODUCTS,
	REQUEST_ADD_BANK_MONEY,
	GET_HOME_WINNER,
	SEND_WINNER,
	MEMBER_REQUEST_CREATE_HOME,                                    //��Ӧ���
	MEMBER_REQUEST_CREATE_HOME_END,                                //��Ӧ�����
	REQUEST_HOMECREATE_LIST,
	SEND_REQUEST_CREATE_HOME_LIST,
	CLOSE_REQUEST_HOME_CREATE_MENU,
	OPEN_REQUEST_HOME_CREATE_MENU,
	SEND_REQUEST_CLEAR,
	HOME_MEMBER_POINTCHANGE,
	ADD_MEMBER_POINT,
	OPEN_HOME_PROFFER,
	SEND_ADD_ENEMY,
	SEND_DEL_ENEMY,
	SEND_ENEMY_LIST,
	OPEN_HOME_FRIEND,
	OPEN_HOME_ENEMY,
	CLOSE_HOME_FRIEND,
	CLOSE_HOME_ENEMY,
	REQUEST_ADD_ENEMY,                                           //��Ӧ���ӵж԰����Ϣ
	REQUEST_ADD_ENEMY_RESULT,
	ASK_HOME_FRIEND,
	ASK_HOME_ENEMY,
	SEND_REQUEST_ADD_HOME_FRIEND_LIST,
	SEND_REQUEST_ADD_HOME_ENEMY_LIST,
	SEND_REQUEST_ADD_HOME_FRIEND_CLEAR,
	SEND_REQUEST_ADD_HOME_ENEMY_CLEAR,
	UPDATE_ENEMY_TIME,                                      //һ����һ���Ӹ���
	SEND_HOME_DOWNLEVEL,
	HOME_WAR_NPC_UP,                                         //���NPC����
	ADD_HOME_CONTRIBUTE,                                     //�����״���
	SEND_HOME_CONTRIBUTE,
	HOME_WAR_NPC_CANCEL,
	HOME_TIPS,
	OPEN_HOME_PROFFER2,
	LUA_HOME_MAIL,
	HOME_WAR_TIP,
	HOME_ADD_HOMEBILL,
	HOME_CLIENT_ADDBILL,
	HOME_TURNOVER_BILL,
	SEND_HOME_BILL,
	REQUEST_RECORD_LIST,
	SEND_RECORD_LIST,
	DEL_ENEMY,
	DEL_ENEMY_RESULT,
	ALLOW_CANCEL_ENEMY,
	ALLOW_CANCEL_ENEMY_RESULT,
	HOME_CHANGE_HEIMUYA,    //C->CS  ͶƱ
	HOMECHANGEHEIMUYA_TIP,  //CS->C  ��ͶƱѡ��
	HOME_CHANGE_RESULT,     //CS->S  ����ı�
	HOME_MEMBER_CHANGE_HEIMUYA,
	HOME_MEMBER_CHANGE,

	REQUEST_HOME_ULSTATE,
	HOME_STATE_UNLOCK,
	HOME_CHANGE_HOME_NAME,
	REQUEST_JUAN_HOMEHONOUR,
	ADD_HOME_HONOUR_JUANXIAN,
	REQUEST_CREATE_HALL,
	SEND_HOME_HALL,
	REMOVE_HOME_HALL_MEMBER,
	ADD_HOME_HALL_MEMBER,
	SEND_HOME_CREATE_SURE,
	CREATE_HOME_HALL,
	SEND_LEADER_TOSURE_HALL_CREATE,
	SEND_LEADER_TOSURE_HALL_CREATE_RESULT,
	REMOVE_HOME_HALL_SELF,
	HOME_ASKFOR_INHALL,
	HOME_ASKFOR_INHALL_RESULT,
	REQUEST_ADD_INHALL,
	REMOVE_HOME_HALL,
	PERMIT_HOME_DUTY, //�����ÿ�ְλ

	REQUEST_HOME_MAILNOTICE,
	ADD_HOME_BANK_GOLD,
	GET_HOME_BANK_GOLD,
	HOME_MEMBER_TOTAL_GOLD_CHANGE,
	SEND_TOOFFICERINFO,
	ADDHOMEMEMBER_SUCCESSFULLY,
};

enum _HOMERESULT
{
    RESULT_DISMISS_HOME,                                   //����ɢ
	RESULT_NOPERMIT,                                       //������û��Ȩ��
	RESULT_DUTY_HAVE,                                      //Ŀǰ��ְλ�Ѿ����˵���
	RESULT_NOLEVEL,                                        //Ŀǰ��������
	RESULT_CREATE_SUCCESS,                                 //��ᴴ���ɹ�
	RESULT_CREATE_ERROR_MONEY,                             //��ᴴ��ʧ��--�����߽�Ǯ����
	RESULT_CREATE_ERROR_LEVEL,                             //��ᴴ��ʧ��--�����ߵȼ�����
	RESULT_CREATE_ERROR_LESSPERSON,                        //��ᴴ��ʧ��--�����������������ʱ��Ҫ���������ƣ�
	RESULT_CREATE_ERROR_NAME,                              //��ᴴ��ʧ��--�����������Ϸ�������ظ���
	RESULT_CREATE_ERROR_OTHERHOME,                         //��ᴴ��ʧ��--�����д����Ѿ�����������
	RESULT_CREATE_ERROR_OTHERCONF,                         //
};

enum LEVELUP
{
	LEVELUP_HOME,
	LEVELUP_RESOURCE,
	LEVELUP_ENFORCE,
};

enum REQUESTTYPE
{
    REQUEST_QUESTION,
	REQUEST_ANSWER,
};

enum HOME_SKILL_RESULT_TTYPE
{
	HOME_SKILL_ADD_OK,
	HOME_SKILL_UPGRADE_OK,
	HOME_SKILL_ERROR_MONEY,
	HOME_SKILL_ERROR_POWER,
	HOME_SKILL_ERROR_HONOUR,
};

enum HOME_BANK_TTYPE
{
	HOME_BANK_OPEN,
	HOME_BANK_CLOSE,
	HOME_BANK_CONTRIBUTE,
	HOME_BANK_DELETE,
	HOME_BANK_PAYFOR,
	HOME_BANK_SENDITEMS,
	HOME_BANK_ADDITEM,	//���
	HOME_BANK_DELETEITEM,
	HOME_BANK_CS_ADDITEM,
	NA_SEND_HOME_SIMPLE_INFO,
	NA_SEND_HOME_BANK_ITEMS,
	NA_ADD_HOME_ITEM,   //CSTOS STOCS
	NA_DELETE_HOME_ITEM,//CSTOS STOCS
	NA_GIVE_HOME_ITEM,  //STOCS
	NA_ADD_HOME_ITEM_TOS,//����Ϸ�������İ�
	NA_CANGIVE_ITEM,
	NA_ASK_HOME_ITEMS,
	NA_ADD_HOME_SIMPLE_INFO,
	NA_DELETE_HOME_SIMPLE_INFO,
	NA_GIVE_HOME_TIP, //�����ķ��������İ����Ʒ��ʾ��
	FRESH_ITEMMARKET_BILL,
	HOME_BANK_ASK_OPEN_BANK,
	FRESH_NA_BILL,
	ADD_ACTER_POINT,
	BACK_ACTER_ITEM,
	ADD_ACTER_EXP,
	HOME_BANK_MONEY_CHANGE,
	BANK_MONEY_ERROR,
	HOME_HONOUR_NUM_CHANGE,
	CHECK_ACTER_ORDER,
	FRESH_MASTER_POINT,
	HOME_BANK_GOLD_CHANGE,
	GET_HOME_BANK_FLAG,
	GET_HOME_NOGIVE_GOLD,
	SEND_SHOW_NPC_EX,
	SEND_SHOW_FIGURE,
	SEND_FLOWER_POINT,
	CHECK_FLOWER_TOTAL_ORDER,
	CHECK_FLOWER_DAY_ORDER,
	SEND_FLOWER_TO,
	ADD_SEND_FLOWER_POINT,
	BACK_ADD_FLOWER_POINT,
	SEND_FLOWER_OK,
	SEND_FLOWER_NO,
	ADD_SEND_FLOWER_ITEM,
	RESET_SEND_FLOWER_DAY_POINT,
	REMOVE_SEND_FLOWER_STATE,
	CHECK_FLOWER_WEEK_ORDER,
	RESET_SEND_FLOWER_WEEK_POINT,
	CHECK_FLOWER_MONTH_ORDER,
	RESET_SEND_FLOWER_MONTH_POINT,
	BOOK_COLLECT_FRESH,
	BOOK_COLLECT_ADD,
	BOOK_PIECE_ADD,
	ADD_BOOK_COLLECT_LEVEL,
	STEAL_BOOK_COLLECT_PIECE,
	GET_STEAL_INFO,
	SEND_STEAL_INFO,
	SEND_ROBOT_INFO,
	ADD_BOOK_COLLECT_LEVEL_END,
	BOOK_PIECE_CHANGE,
	GET_ROBOT_INFO,
	STEAL_TIME,
	ROBOT_BOOK_PIECE,
	ROBOT_TIME,
};

struct StealInfo
{
	int nLvl;
	DWORD dwUin;
	int bJob;
	char szName[20];
	BYTE sex;
};

//struct CRECV_FROM_PLAYER_BASE_ATTRIB_Data;


struct HOMEBANK_SEND_LOOK_INFO_Data
{
	DWORD dwUin;
	DWORD dwLookUin;
	DWORD dwTicket;
};

struct HOME_ADDHOMEMEMBER_SUCCESSFULLY_Data
{

};

struct HOMEBANK_SEND_LOOK_INFO2_Data
{
	DWORD dwUin;
	DWORD dwLookUin;
	DWORD dwTicket;
};

struct HOMEBANK_SEND_DEATEL_LOOK_ERROR_Data
{
	int nType;
};

struct HOMEBANK_ROBOT_BOOK_PIECE_Data
{
	DWORD dwUin;
	int   nSkillID;
	int   nPos;
	int   nDelt;
	char  szRobotName[20];
};

struct HOMEBANK_STEAL_TIME_Data
{
	int nTimes;
};

struct HOMEBANK_ROBOT_TIME_Data
{
	int nTimes;
};

struct HOMEBANK_GET_ROBOT_INFO_Data
{
	int nSkillID;
	int nPos;
};

struct HOMEBANK_BOOK_PIECE_CHANGE_Data
{
	int nSkillID;
	int nPos;
	int nNum;
};

struct HOMEBANK_ADD_BOOK_COLLECT_LEVEL_END_Data
{
	bool blFg;
};
struct HOMEBANK_SEND_ROBOT_INFO_Data
{
	DWORD uin;
	int nNum;
	StealInfo pInfo[1];
};

struct HOMEBANK_SEND_STEAL_INFO_Data
{
	int nNum;
	StealInfo pInfo[1];
};

struct HOMEBANK_GET_STEAL_INFO_Data
{
	int nSkillID;
	int nPos;
};

struct HOMEBANK_STEAL_BOOK_COLLECT_PIECE_Data
{
	DWORD dwUin;
	int   nSkillID;
	int   nPos;
};


struct HOMEBANK_ADD_BOOK_COLLECT_LEVEL_Data
{
	int nSkillID;
};

struct HOMEBANK_BOOK_PIECE_ADD_Data
{
	int nSkillID;
	int Num[6];
	bool bShow;
};

struct HOMEBANK_BOOK_COLLECT_ADD_Data
{
	int  nSkillID;
	int   nLevel;
	bool  bShow;
};

struct HOMEBANK_BOOK_COLLECT_FRESH_Data
{

};


struct HOMEBANK_HOME_BANK_OPEN_Data
{
	DWORD dwHomeBankMoney;
	DWORD dwHomeBankGold;
	DWORD dwHomeBankNoGiveGold;
};

struct HOMEBANK_HOME_BANK_MONEY_CHANGE_Data
{
	DWORD dwHomeBankMoney;
};
struct HOMEBANK_HOME_BANK_CLOSE_Data
{

};

struct HOMEBANK_HOME_BANK_CONTRIBUTE_Data
{
	DWORD itemIndex;
};

struct HOMEBANK_HOME_BANK_DELETE_Data
{
	DWORD itemIndex;
};

struct HOMEBANK_SEND_FLOWER_POINT_Data
{
	DWORD dwFlowerTotalPoint;
	DWORD dwFlowerDayPoint;
	DWORD dwFlowerWeekPoint;
	DWORD dwFlowerMonthPoint;
};

struct HOMEBANK_HOME_BANK_PAYFOR_Data
{

};

struct HOMEBANK_HOME_BANK_SENDITEMS_Data
{
	BYTE count;
	CLIENTITEMEX item[1];
};

struct HOMEBANK_HOME_BANK_ADDITEM_Data
{
	CLIENTITEMEX item;
};

struct HOMEBANK_HOME_BANK_CS_ADDITEM_Data
{	//���ķ���������Ϸ���������õ����ݽṹ
	LONGLONG llKey;		//��ƷKEY
	BYTE     byType;	//��Ʒ����
	DWORD    ItemID;    //��ƷID
	DWORD    dwIndex;   //�Լ��õ�INDEX
	DWORD    dwUin;     //�ĸ���Ҹ������
	BYTE     pData[1];  //�������Ʒ����
};

struct HOMEBANK_SEND_FLOWER_TO_Data
{
	DWORD  dwItemID;
	char   szName[20];
};

struct HOMEBANK_NA_SEND_HOME_BANK_ITEMS_Data
{
	DWORD dwIndex;
	char szName[32];
	BYTE  pData[1];
};

struct HomeItemSimple
{
	DWORD   m_dwIndex;
	DWORD   m_dwItemID;
	WORD    m_wNum;
};
struct HOMEBANK_NA_SEND_HOME_SIMPLE_INFO_Data
{
	BYTE  bHomeItemNum;
	HomeItemSimple pInfo[1];
};

struct HOMEBANK_NA_ADD_HOME_SIMPLE_INFO_Data
{
	HomeItemSimple pInfo;
};

struct HOMEBANK_NA_DELETE_HOME_SIMPLE_INFO_Data
{
	DWORD dwIndex;
};

struct HOMEBANK_NA_GIVE_HOME_TIP_Data
{
	char szText[48];
};

struct HOMEBANK_FRESH_ITEMMARKET_BILL_Data{
	DWORD	dwFromUIN;
	DWORD   dwItemMarketBill;
};

struct HOMEBANK_FRESH_MASTER_POINT_Data{
	DWORD   dwFromUin;
	DWORD   dwMasterBill;
	DWORD	dwPointUsed;
};

struct HOMEBANK_HOME_BANK_GOLD_CHANGE_Data{
	DWORD   dwHomeBankGold;
	DWORD   dwHomeBankNoGiveGold;
};

struct HOMEBANK_GET_HOME_BANK_FLAG_Data{
	int    nFlagValue;
	int    nPer;
};

struct HOMEBANK_GET_HOME_NOGIVE_GOLD_Data{
	int nPer;
};

struct HOMEBANK_SEND_SHOW_NPC_EX_Data
{
	char  szName[40];
	bool  blFg;
};

struct HOMEBANK_SEND_SHOW_FIGURE_Data
{
	char szName[40];
};

struct  HOMEBANK_NA_ADD_HOME_ITEM_Data
{
	LONGLONG llItemKey;
	char	szText[48];
	BYTE	pData[1];
};

struct HOMEBANK_NA_DELETE_HOME_ITEM_Data
{
	DWORD dwToUin;
	DWORD dwIndex;
	LONGLONG llItemKey;
};

struct HOMEBANK_NA_GIVE_HOME_ITEM_Data
{
	DWORD	dwToUin;
	DWORD	dwFromUin;
	LONGLONG	llKey;
	BYTE	data[1];
};

struct HOMEBANK_NA_ADD_HOME_ITEM_TOS_Data
{
	DWORD dwToUin;
	DWORD dwIndex;
	LONGLONG llItemKey;
};

struct HOMEBANK_NA_CANGIVE_ITEM_Data
{
	LONGLONG llItemKey;
};

struct HOMEBANK_NA_ASK_HOME_ITEMS_Data
{
};


struct HOMEBANK_HOME_BANK_DELETEITEM_Data
{
	CLIENTITEMEX item;
};

struct HOMEBANK_HOME_BANK_ASK_OPEN_BANK_Data
{
};

struct HOMEBANK_FRESH_NA_BILL_Data
{
	DWORD   dwFromUIN;
	DWORD   dwNaGold;
	DWORD   dwNaBindGold;
};

struct HOMEBANK_BANK_MONEY_ERROR_Data
{
	DWORD  dwUin;
	BYTE   bError;
	BYTE   bType;
};

struct HOMEBANK_HOME_HONOUR_NUM_CHANGE_Data
{
	DWORD  dwUin;
	DWORD  dwHomeHonourNum;
};

struct HOMEBANK_CHECK_ACTER_ORDER_Data
{
	DWORD  dwUin;
};

struct HOMEBANK_ADD_ACTER_POINT_Data
{
	DWORD dwItemID;
	BYTE  bNum;
	DWORD dwUIN;
	char  szName[40];
};

struct HOMEBANK_BACK_ACTER_ITEM_Data
{
	DWORD dwFromUIN;
	DWORD dwItemID;
	BYTE  bNum;
	DWORD dwUIN;
};

struct HOMEBANK_ADD_ACTER_EXP_Data
{
	DWORD dwItemID;
	BYTE  bNum;
	DWORD dwUIN;
};

struct HOME_REQUEST_OPEN_PRODUCTS_Data
{
	DWORD dwHomeId;
	BYTE byType;
};

struct HOME_REQUEST_ACTIVE_PRODUCTS_Data
{
	DWORD dwHomeId;
	BYTE byType;
	BOOL bActive;
};

struct HOME_REQUEST_ADD_BANK_MONEY_Data
{
	DWORD dwHomeId;
	BYTE byType;
	int	nMoney;
};

struct HOME_SEND_HOME_MESSAGE_Data
{
	WORD wHomeID;//new add
    char szMsg[1];
};

struct HOME_SEND_HOME_MESSAGECODE_Data
{
	BYTE byMsgType;
};

struct HOME_SEND_HOME_INFO_Data             //��������Ϣ--���ID��������ơ�������������ȼ����������
{
    WORD  wHomeID;
	char  szHomeName[MAX_HOMENAME];
	int   nMemberCount;
	int   nMemberLimit;
	BYTE  byLevel;
	BYTE  byRebuildTime;
	BYTE  bchLock;
	bool  bHomeProffer;
	bool  bHomeProffer2;
	DWORD dwContributeTime;
	DWORD	dwHStates;
	bool  sendFlag;
	char  szHomeMsg[1];
};

struct HOME_SEND_HOME_MP_Data             //����ʽ𡢷��ٶȡ������
{
	DWORD  dwMoney;
	DWORD  dwPower;
	DWORD  dwMenage;
	DWORD  dwHonour;
	DWORD  dwHomeHonour;
};



struct HOME_SEND_HOME_RESOURCE_Data      //�����Դ�ȼ�
{
	BYTE   byLevel;
};

struct HOME_SEND_HOME_ENFORCE_Data      //���ǿ���ȼ�
{
	BYTE   byLevel;
};

struct HOME_SEND_HOME_DUTYSETTING_Data   //���ְλ�������
{
    WORD  wPermit[HOME_DUTY_MAX];
};

struct HOME_SEND_HOME_MSGCHANGE_Data    //�������
{
	char  szHomeMsg[1];
};

struct HOME_SEND_HOME_LEVELCHANGE_Data  //���ȼ����
{
	BYTE  byLevel;
	int   nMemberLimit;
};

struct HOME_SEND_HOME_REBUILD_Data
{
    DWORD dwTime;                      //0--û���Լ�          
};

struct HOME_HOME_MEMBER_ADD_Data         //����Ա����
{
    DWORD dwUin;
	char  szName[16];
	BYTE  byDutyIndex;
	BYTE  byLevel;
	BYTE  byJob;
	bool  bOnline;
	DWORD dwPower;
	DWORD dwTotalPower;
	char  szLogoutTime[12];
	DWORD dwPoint;
	DWORD dwTotalPoint;
	int   nDays;
	DWORD dwHallID;
	BYTE  byHallDutyIndex;
	DWORD dwTotalGold;
	char  szHallName[16];
};

struct HOME_HOME_MEMBER_REMOVE_Data    //����Ա�뿪���
{
	DWORD dwOrderUin;
    DWORD dwUin;
};

struct HOME_HOME_MEMBER_DUTYCHANGE_Data  //����Աְλ�仯
{
    DWORD dwUin;
	BYTE  byDutyIndex;
};

struct HOME_HOME_MEMBER_JOBCHANGE_Data   //����Աְҵ�仯
{
	DWORD dwUin;
	BYTE  byJob;
};

struct HOMEBANK_CHECK_FLOWER_DAY_ORDER_Data
{
	DWORD  dwUin;
};

struct HOMEBANK_CHECK_FLOWER_WEEK_ORDER_Data
{
	DWORD  dwUin;
};

struct HOMEBANK_CHECK_FLOWER_MONTH_ORDER_Data
{
	DWORD  dwUin;
};

struct HOME_HOME_MEMBER_LEVELCHANGE_Data   //����Ա�ȼ��仯
{
	DWORD dwUin;
	BYTE  byLevel;
};

struct HOME_HOME_MEMBER_LOGONOFF_Data    //����Ա������
{
	DWORD dwUin;
	bool  bOn;
};

struct HOME_HOME_MEMBER_POWERCHANGE_Data
{
    DWORD dwUin;
	DWORD dwPower;
	DWORD dwTotalPower;
};

struct HOME_REQUEST_CREATE_HOME_Data    //�������
{
	DWORD dwCreateUin;
	int   nCount;
	DWORD dwUins[6];
	bool  bIsHeimuya;
	char  szHomeName[1];
};

struct HOME_REQUEST_DISMISS_HOME_Data   //��ɢ���
{
	DWORD  dwHomeID;
};

struct HOME_REQUEST_LEAVE_HOME_Data     //�뿪���
{
	DWORD  dwHomeID;
};

struct HOME_REQUEST_ADDHOME_MEMBER_Data   //�����³�Ա
{
	DWORD dwHomeID;
	DWORD dwUin;
	char  szName[1];
};

struct HOME_REQUEST_REMOVEHOME_MEMBER_Data   //������Ա
{
	DWORD dwHomeID;
	DWORD dwUin;
};

struct HOME_REQUEST_CHANGE_MEMBERDUTY_Data    //�ı��Աְλ
{
	DWORD dwHomeID;
	DWORD dwUin;
	BYTE  byDutyIndexOld;
	BYTE  byDutyIndexNew;
};

struct HOME_REQUEST_CHANGE_HOMEMSG_Data     //�ı乫��
{
	DWORD dwHomeID;
	char  szNote[1];
};

struct HOME_REQUEST_REBUILD_HOME_Data            //�Լ�
{
	DWORD dwHomeID;
    DWORD dwUin;                           
};

struct HOME_REQUEST_GETRESOURCE_Data      //������ȡ��Դ   
{
	DWORD dwHomeID;
    DWORD dwUin;
};

struct HOME_REQUEST_GETMENAGE_Data       //��ȡ�����
{
	DWORD dwHomeID;
    DWORD dwUin;
};

struct HOME_REQUEST_JUANQIAN_Data
{
	BYTE byCount;
	BYTE bKind;
};

struct HOME_REQUEST_JUAN_HOMEHONOUR_Data
{

};

struct HOME_REQUEST_LEVELUP_Data        //�������--������Դ��ǿ��
{
	DWORD dwHomeID;
    DWORD dwUin;
	BYTE  byType;
};

struct HOME_RESULT_HOME_PACKET_Data
{
	DWORD  dwHomeID;
	DWORD  dwInviteID;
	BYTE   byResultType;
};

struct HOME_ADD_HOME_MP_Data
{
	DWORD	dwHomeID;
	int		dwMoney;
	int		dwPower;
	int		dwMenagePoint;
	int		nHonour;
	BYTE	byType;
	DWORD	dwUin;
};


struct HOME_ADD_MEMBER_POWER_Data
{
	DWORD dwHomeID;
	DWORD dwUin;
	int   dwPower;
};

struct HOME_ADD_MEMBER_POINT_Data
{
	DWORD dwHomeID;
	DWORD dwUin;
	int   dwPoint;
};

struct HOME_ADD_HOME_HONOUR_JUANXIAN_Data
{
	DWORD dwHomeID;
	DWORD dwUin;
	DWORD dwTimes;
};

struct HOME_REQUEST_CREATE_HALL_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	BYTE  bResult;
	char  szHallName[16];
};

struct HOME_REQUEST_ADD_INHALL_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	DWORD dwRequestUin;
	BYTE  bResult;
};

struct HOME_SEND_HOME_HALL_Data
{
	DWORD dwUin;
	DWORD dwHallID;
	BYTE  bHallDutyIndex;
	char  szHallName[16];
};

struct HOME_REMOVE_HOME_HALL_MEMBER_Data
{
	DWORD  dwUin;
	DWORD  dwLeaderUin;
	DWORD  dwHomeID;
};

struct HOME_PERMIT_HOME_DUTY_Data
{
	DWORD  dwPermitUin;
	DWORD  dwLeaderUin;
	DWORD  dwHomeID;
};

struct HOME_REQUEST_HOME_MAILNOTICE_Data
{
	int		nHomeID;
	char	szBody[1];
};

struct HOME_REMOVE_HOME_HALL_SELF_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
};

struct HOME_REMOVE_HOME_HALL_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	DWORD dwBemovedUin;
	bool  bSelfRemove;
};

struct HOME_ADD_HOME_HALL_MEMBER_Data
{
	DWORD  dwUin;
	DWORD  dwLeaderUin;
	DWORD  dwHomeID;
};

struct HOME_SEND_HOME_CREATE_SURE_Data
{
	char  szName[16];
};

struct HOME_CREATE_HOME_HALL_Data
{
	DWORD dwHomeID;
	DWORD dwUin;
	char szName[16];
};

struct HOME_SEND_LEADER_TOSURE_HALL_CREATE_Data
{
	DWORD dwUin;
	char szCreateName[16];
	char szName[16];
};

struct HOME_HOME_ASKFOR_INHALL_Data
{
	DWORD dwUin;
	char szHallName[16];
	char szAskName[16];
};

struct HOME_HOME_ASKFOR_INHALL_RESULT_Data
{
	BYTE bResult;
};

struct HOME_SEND_LEADER_TOSURE_HALL_CREATE_RESULT_Data
{
	BYTE bResult;
};

struct HOME_JOIN_HOME_WAR_Data
{
	WORD       wHomeID;
	int		   nLimitLvl;//�ȼ����ƣ�����ֱ�Ӵ����ķ��������˵������ϵİ���Ա�ȼ������ٶ����ķ������ķ�����
};

struct HOME_REQUEST_JOIN_HOME_MAP_Data
{
	DWORD dwHomeID;
	BYTE  bmapType;
};

struct HOME_JOIN_HOME_MAP_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	BYTE  bmapType;
};

struct HOME_HOME_MEMBER_LOGOUTTIMECHANGE_Data
{
	DWORD dwUin;
	char  szLogoutTime[12];
};

struct HOME_REQUEST_HOME_SKILL_LIST_Data
{
	WORD wServerID;
};

struct HOME_SEND_HOME_SKILL_LIST_Data
{
	DWORD dwHomeID;
    BYTE bCount;
	HomeSkill Skill[1];
};

struct HOME_ADD_HOME_SKILL_Data
{
	DWORD dwHomeID;
    WORD  wSkillID;
	WORD wServerID;
	DWORD dwUin;
};

struct HOME_SEND_ADD_HOME_SKILL_Data
{
	DWORD dwHomeID;
	WORD  wSkillID;
	BYTE  bLevel;	
};

struct HOME_UPGRADE_HOME_SKILL_Data
{
	DWORD dwHomeID;
	WORD  wSkillID;
	WORD wServerID;
    DWORD dwUin;	
};

struct HOME_SEND_UPGRADE_HOME_SKILL_Data
{
	DWORD dwHomeID;
	WORD  wSkillID;
	BYTE  bLevel;	
};

struct HOME_SEND_HOME_SKILL_RESULT_Data
{    
	BYTE	bError;
};

struct HOME_REQUEST_LEAGUE_Data
{
	bool	bAdd;
	DWORD	dwUin;
	char	szHomeName[48];
};

struct HOME_ALLOW_LEAGUE_Data
{
	DWORD	dwAllowHomeID;					//	��Ӧ�����ID
	char	szRequestHomeName[48];			//	���뷽�������
	bool	bAllowed;
};

struct HOME_ALLOW_CANCEL_ENEMY_Data
{
	DWORD  dwAllowHomeID;
	char   szRequestHomeName[48];
	bool   bAllowed;
};


struct HOME_DEL_ENEMY_Data
{
	bool	bAdd;
	DWORD	dwUin;
	char	szDelEnemyHomeName[48];
};
enum
{
	REQUEST_LEAGUE_ERROR_CODE_PLAYER_ERROR,	//	�����˲����ڣ�����û���㹻�İ��Ȩ��
	REQUEST_LEAGUE_ERROR_CODE_NO_HOME,		//	�����ڸð��
	REQUEST_LEAGUE_ERROR_CODE_NO_TARGET,	//	�Է��峤������
	REQUEST_LEAGUE_ERROR_CODE_SELF_HOME,	//	��������Լ��İ��Ϊͬ��
	REQUEST_LEAGUE_ERROR_CODE_WAITING,		//	�ȴ��ظ�
	REQUEST_LEAGUE_ERROR_CODE_SUCCESSFUL,	//	�ɹ�
	REQUEST_LEAGUE_ERROR_LIMIT_OF_HEIMUYA,  //  ������������
	REQUEST_LEAGUE_ERROR_CODE_TIME_ERROR,
	REQUEST_LEAGUE_ERROR_CODE_MAX,
};

enum
{
	CANCEL_ENEMY_ERROR_CODE_PLAYER_ERROR,	//	�����˲����ڣ�����û���㹻�İ��Ȩ��
	CANCEL_ENEMY_ERROR_CODE_NO_HOME,		//	�����ڸð��
	CANCEL_ENEMY_ERROR_CODE_HAVE_ENEMY,    //  ���Ѿ��еж԰�� �����ٴ����
	CANCEL_ENEMY_ERROR_CODE_NO_TARGET,	    //	�Է�����Ѵ��ڵж�״̬
	CANCEL_ENEMY_ERROR_CODE_SELF_HOME,	    //	��������Լ��İ��Ϊ�ж԰��
	CANCEL_ENEMY_ERROR_CODE_WAITING,
	CANCEL_ENEMY_ERROR_CODE_SUCCESSFUL,	//	�ɹ�
	CANCEL_ENEMY_ERROR_LIMIT_OF_HEIMUYA,   //  ������������
	CANCEL_ENEMY_ERROR_CODE_OF_LEAGUE,     //  ��������
	CANCEL_ENEMY_ERROR_CODE_MAX,
};

struct HOME_REQUEST_LEAGUE_RESULT_Data
{
	BYTE	bErrorCode;
};

struct HOME_DEL_ENEMY_RESULT_Data
{
	BYTE    bErrorCode;
};
struct HOME_ALLOW_LEAGUE_RESULT_Data
{
	char	szHomeName[48];
	bool	bAllowed;
};

struct HOME_ALLOW_CANCEL_ENEMY_RESULT_Data
{
	char	szHomeName[48];
	bool	bAllowed;
};

struct HOME_HOME_CHANGE_HEIMUYA_Data
{
	DWORD  dwUin;
	DWORD  dwHomeID;
	bool   bOk;
};

struct HOME_HOME_MEMBER_CHANGE_HEIMUYA_Data
{
	DWORD  dwUin;
	DWORD  dwHomeID;
	bool   bOk;
};

struct HOME_REQUEST_HOME_ULSTATE_Data
{
	DWORD	dwHomeID;
	DWORD	dwUin;
	BYTE	bType;
};

struct HOME_HOME_STATE_UNLOCK_Data
{
	DWORD  dwStateUnlock;
};

struct HOME_HOME_CHANGE_HOME_NAME_Data
{
	DWORD dwUin;
	char szName[256];
	BYTE bChangeNameStatus;
};

struct HOME_HOME_MEMBER_CHANGE_Data
{

};

struct HOME_HOMECHANGEHEIMUYA_TIP_Data
{

};

struct HOME_HOME_CHANGE_RESULT_Data
{
	bool  bResult;
};



struct LeagueInfo
{
	DWORD	dwHomeID;
	char	szHomeName[48];
};

struct LeagueInfoEx
{
	DWORD dwHomeID;
	DWORD dwTime;
	char  szHomeName[48];
};

struct HOME_SEND_LEAGUE_LIST_Data
{
	bool	bInit;
	DWORD	dwHomeID;
	DWORD	dwLeagueCnt;
	LeagueInfo	xLeagueInfo[1];
};

struct HOME_SEND_ENEMY_LIST_Data
{
	bool	bInit;
	DWORD	dwHomeID;
	DWORD	dwLeagueCnt;
	LeagueInfoEx	xLeagueInfo[1];
};

struct HOME_OPEN_HOME_FRIEND_Data
{

};

struct HOME_OPEN_HOME_ENEMY_Data
{

};

struct HOME_CLOSE_HOME_FRIEND_Data
{

};

struct HOME_CLOSE_HOME_ENEMY_Data
{

};

struct HOME_SEND_WINNER_Data
{
	 HomeWarInf	xHomeInfo[1];
};

struct HOME_MEMBER_REQUEST_CREATE_HOME_Data
{
	DWORD dwHomeID;
	DWORD dwUin;
	bool  bIsHeimuya;
};

struct HOME_MEMBER_REQUEST_CREATE_HOME_END_Data
{
	DWORD dwHomeID;
	_CreateHome  createhomeFlag;
};

typedef struct HOME_SEND_ADD_LEAGUE_Data
{
	DWORD	dwHomeID;
	LeagueInfo	xLeagueInfo;
}HOME_SEND_DEL_LEAGUE_Data;

struct HOME_SEND_ADD_ENEMY_Data
{
	DWORD	dwHomeID;
	DWORD   dwTime;
	LeagueInfo	xLeagueInfo;
};

struct HomeRequestJoiner
{
	DWORD	dwUin;
	BYTE	byLv;
	BYTE	byJob;
	char	szName[16];
};

struct HOME_SEND_REQUEST_LIST_Data
{
	BYTE				byCnt;
	HomeRequestJoiner	xRJer[1];
};

struct HOME_REQUEST_JOIN_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
};

struct HOME_CANCEL_JOIN_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
};

struct HOME_REQUEST_REQUEST_LIST_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
};

struct HOME_REQUEST_RECORD_LIST_Data
{
	DWORD   dwUin;
	DWORD	dwHomeID;
};

struct SendDeatalRecord
{
	char szLeadName[15];
	char szName[15];
	DWORD dwMoney;
	char szTime[20];
	BYTE bType;
};
struct HOME_SEND_RECORD_LIST_Data
{
	int  nNum;
	SendDeatalRecord	pData[1];
};

struct HOME_REQUEST_HOMECREATE_LIST_Data
{
	DWORD   dwUin;
};

struct HOME_REQUEST_HOME_SINFO_LIST_Data
{
	DWORD	dwUin;
};

struct HOME_GET_HOME_WINNER_Data
{
	 DWORD	dwUin;
};

struct HOME_SEND_HOME_SINFO_Data
{
	DWORD	dwHomeID;
	char	szHomeName[48];
	BYTE	byHomeLv;
	DWORD	byMemLimit;
	BYTE	byMemCur;
	BYTE	byRequesterCnt;
	BYTE	byIsRequested;
	bool    bHeimuya;
};
struct HOME_ALLOW_REQUEST_JOIN_Data
{
	DWORD	dwOrderUin;
	DWORD	dwObjUin;
	DWORD	dwHomeID;
};

struct HOME_CANCEL_REQUEST_JOIN_Data
{
	DWORD	dwOrderUin;
	DWORD	dwObjUin;
	DWORD	dwHomeID;
};

struct HOME_REQUEST_CHANGE_HOMENAME_Data
{
	char	szHomeName[MAX_HOMENAME];
};

struct HOME_CHANGE_HOME_ICON_Data
{
	DWORD	dwHomeID;
	int		nHomeICON;
};

struct HOME_SEND_REQUEST_CREATE_HOME_LIST_Data
{
	DWORD	dwHomeID;
	char	szHomeName[48];
	char    szHomeLeaderName[16];
	BYTE    byCurrMemberNum;
	BYTE    byMostMemberNum;
	DWORD   dwLastTime;
	BYTE    byIsRequested;
};

struct HOME_ASK_HOME_FRIEND_Data
{
	DWORD dwUin;
};

struct HOME_ASK_HOME_ENEMY_Data
{
	DWORD dwUin;
};

struct HOME_SEND_REQUEST_ADD_HOME_FRIEND_LIST_Data
{	//�����Ա��ӵж԰����б�
	DWORD   dwHomeID;
	char    szHomeName[48];
	char    szHomeLeaderName[16];   
};

struct HOME_SEND_REQUEST_ADD_HOME_ENEMY_LIST_Data
{	//�����Ա��ӵж԰����б�
	DWORD   dwHomeID;
	char    szHomeName[48];
	char    szHomeLeaderName[16];   
};

struct HOME_CLOSE_REQUEST_HOME_CREATE_MENU_Data
{

};

struct HOME_OPEN_REQUEST_HOME_CREATE_MENU_Data
{

};

struct HOME_SEND_REQUEST_CLEAR_Data
{

};

struct HOME_SEND_REQUEST_ADD_HOME_FRIEND_CLEAR_Data
{

};

struct HOME_SEND_REQUEST_ADD_HOME_ENEMY_CLEAR_Data
{

};

struct HOME_UPDATE_ENEMY_TIME_Data
{
	DWORD dwTime;
};

struct HOME_HOME_MEMBER_POINTCHANGE_Data
{
	DWORD dwUin;
	DWORD dwPoint;
	DWORD dwTotalPoint;
};

struct HOMEBANK_CHECK_FLOWER_TOTAL_ORDER_Data
{
	DWORD  dwUin;
};

struct HOME_HOME_MEMBER_TOTAL_GOLD_CHANGE_Data
{
	DWORD dwUin;
	DWORD dwTotalGold;
};

struct HOME_SEND_TOOFFICERINFO_Data
{

};

struct HOME_SEND_HOME_BILL_Data
{
	DWORD dwUin;
	int	  nHomeBill;
};

struct HOME_OPEN_HOME_PROFFER_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
};

struct HOME_OPEN_HOME_PROFFER2_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
};

struct HOME_LUA_HOME_MAIL_Data
{
	DWORD dwHomeID1;
	DWORD dwHomeID2;
	DWORD dwDingID;
	char  szTimeStr[1];
};

struct HOME_HOME_WAR_TIP_Data
{
	DWORD dwHomeID; //���ID
	DWORD dwHomeMoney;
	BYTE  bType;
};

struct HOME_HOME_ADD_HOMEBILL_Data
{
	DWORD dwHomeBankMoney;
	DWORD dwHomeID;
	DWORD dwUin;
	int   nDays;
};

struct HOME_HOME_CLIENT_ADDBILL_Data
{
	int nDays;
};

struct HOME_HOME_TURNOVER_BILL_Data
{
	int m_nDays;
	bool m_bShow;
};

struct HOME_SEND_DEL_ENEMY_Data
{
	DWORD	dwHomeID;
	LeagueInfo	xLeagueInfo;
};


struct HOME_REQUEST_ADD_ENEMY_Data
{
	bool	bAdd;
	DWORD	dwUin;
	char	szHomeName[48];
};

struct HOME_REQUEST_ADD_ENEMY_RESULT_Data
{
	BYTE	bErrorCode;
};

struct HOME_SEND_HOME_DOWNLEVEL_Data
{
	BYTE   bErrorCode;
};

struct HOME_HOME_TIPS_Data
{
	BYTE   bTips;
};

struct HOME_HOME_WAR_NPC_UP_Data
{
	BYTE  bNpcType;
	DWORD dwMoney;
};

struct HOME_ADD_HOME_CONTRIBUTE_Data
{
	DWORD	dwHomeID;
	DWORD   dwUin;
};

struct HOME_ADD_HOME_BANK_GOLD_Data
{
	DWORD   dwHomeID;
	DWORD   dwUin;
	int     nBankGold;
	int     nBankNoGiveGold;
	int     nTotalGold;
};

struct HOME_GET_HOME_BANK_GOLD_Data
{
	DWORD dwHomeID;
	DWORD dwUin;
	int   nPer;
};

struct HOME_SEND_HOME_CONTRIBUTE_Data
{
	DWORD   dwContributeTime;
};

struct HOME_HOME_WAR_NPC_CANCEL_Data
{

};

struct HOMEBANK_ADD_SEND_FLOWER_POINT_Data
{
	DWORD dwItemID;
	DWORD dwUin;
	char  szName[40];
};

struct HOMEBANK_BACK_ADD_FLOWER_POINT_Data
{
	DWORD dwItemID;
	DWORD dwUin;
	int   nMoney;
	int   nBindMoney;
	int   nGold;
	int   nBindGold;
	DWORD dwTicketTime;
};

struct HOMEBANK_RESET_SEND_FLOWER_DAY_POINT_Data
{
	DWORD dwUin;
	DWORD dwFlowerDayPoint;
};

struct HOMEBANK_RESET_SEND_FLOWER_WEEK_POINT_Data
{
	DWORD dwUin;
	DWORD dwFlowerWeekPoint;
};

struct HOMEBANK_RESET_SEND_FLOWER_MONTH_POINT_Data
{
	DWORD dwUin;
	DWORD dwFlowerMonthPoint;
};

struct HOMEBANK_SEND_FLOWER_OK_Data
{
	char szName[40];
};

struct HOMEBANK_SEND_FLOWER_NO_Data
{

};

struct HOMEBANK_ADD_SEND_FLOWER_ITEM_Data
{
	DWORD dwItemID;
	DWORD dwUin;
	DWORD dwTimeTicket;
};

struct HOMEBANK_REMOVE_SEND_FLOWER_STATE_Data
{
	DWORD dwUin;
};

#pragma pack(pop)

#endif