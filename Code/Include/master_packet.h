#ifndef _MASTER_PACKET_H_
#define _MASTER_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum MASTER_ERROR_CODE
{
	MASTER_RESULT_OK,						// �ɹ�,����
	MASTER_RESULT_ERROR_MASTER_SIMPLEINFO,  // û��ʦ��SI,����
	MASTER_RESULT_ERROR_TUDI_SIMPLEINFO,	// û��ͽ��SI,����
	MASTER_RESULT_ERROR_HAVE_MASTER,		// ͽ����ʦ��,��ʦ
	MASTER_RESULT_ERROR_TUDILVL,            // ͽ�ܵȼ�����, ��ʦ����ʦ
	MASTER_RESULT_ERROR_MASTERLVL,			// ʦ���ȼ�����, ��ʦ
	MASTER_RESULT_ERROR_DELTALVL,			// �ȼ�������, ��ʦ
	MASTER_RESULT_ERROR_TUDI_COUNT,			// ͽ����������,��ʦ
	MASTER_RESULT_ERROR_IN_TMGR,			// CTudiMgr����ʧ��,��ʦ�����룬����
	MASTER_RESULT_ERROR_HASCHUSHI,
	MASTER_RESULT_ERROR_IN_CD,
	MASTER_RESULT_ERROR_IN_TUDI_REFUSE,
	MASTER_RESULT_ERROR_IN_MASTER_REFUSE,

	MASTER_RESULT_OK_SPECIAL,			//���ⲻ��Ǯ
};

enum MASTER_PACKET_MENU
{
	// �ͻ���������Ϣ
	C_GET_MASTER_TUDI_INFO,  //------->GS����
	GS_GET_MASTER_TUDI_INFO, //------->CS��uin�İ�

	// ���߷���-------------->GS&C
	CS_SEND_TUDI_INFO,	  
	CS_SEND_MASTER_INFO,	// ��Ϸ�����������ػ�ȡMASTER

	
	// ��ҽ���-------------->GS
	C_MASTERSYS_CHUSHI,
	C_MASTERSYS_PANSHI,		// �������ʦ��
	C_MASTERSYS_QUTU,		// ͽ��UIN,������ͽ

	//	--------------------->C
	GS_MASTERSYS_PROCESS_WAIT,	// ���ͻ��ˣ���ʾ���ķ��������ڴ���ʦͽϵͳ����

	//	--------------------->CS
	GS_MASTERSYS_BAISHI,	// ��ʦ,�ű�
	GS_MASTERSYS_CHUSHI,	// ��ʦ,�ű�
	GS_MASTERSYS_PANSHI,	// ����ʦ��
	GS_MASTERSYS_QUTU,		// ������ͽ

	//	--------------------->C
	CS_MASTERSYS_BAISHI_RESULT,
	CS_MASTERSYS_CHUSHI_RESULT,
	CS_MASTERSYS_PANSHI_RESULT,
	CS_MASTERSYS_QUTU_RESULT,

    // -------------------------------����Ϊ�µ���չ�����ͻ�����Ҫ�����------------------------------------
	// ��ʾ��Ϣ�ı�---------->C
	CS_TUDI_LELVEL_CHG,
	CS_MASTER_LELVEL_CHG,
	CS_TUDI_JOB_CHG,
	CS_MASTER_JOB_CHG,
	CS_MASTER_HOME_CHG,
	CS_MASTER_HONOURLVL_CHG,
	CS_TUDI_ONLINE,
	CS_MASTER_ONLINE,

	//  --------------------->GS
	C_MASTERSYS_EXCHAGE_POINT,         // �ͻ�������,����

	//  --------------------->GS & C
	CS_MASTERSYS_EXCHAGE_POINT,        // ���ķ��������ּ���֪ͨ����������Ԫ������Ա��˭������Ԫ��
	
	//	--------------------->CS 
	GS_MASTERSYS_COST_GOLD,			   // ͽ�������˽�Ԫ������Ա��˭������Ԫ��
	GS_MASTERSYS_EXCHAGE_POINT,        // �������ķ������ۻ��֣���Ա��˭

	//	--------------------->CS
	CS_MASTERSYS_COST_GOLD,
	CS_MASTERSYS_IS_CHUSHI,
	GS_MASTERSYS_MASTER_NAME,          // ��ʦͽ��ӵ�еĳƺ�
	CS_SEND_IS_SHIELD,		//�ͻ����Ƿ�ѡ���ΰ�ʦ
	CS_SEND_MASTER_TEACH,
	CS_SEND_MASTER_CD,
	C_MASTERSYS_SHOUTU,
	GS_MASTERSYS_IS_PANSHI,
	C_MASTERSYS_BAISHI,
	CS_TEACH_CNT_CHANGE,

	GET_MASTERS_INFO,  // �ͻ���Ҫ����ʦ���б�
	REG_MASTERS_INFO,  // ��Ӧע��ʦ����Ϣ
	REG_MASTER_INFO,   // ��Ӧ�ͻ����������ʦ����Ϣ

	ASK_MASTER_INFO,  // �Ͷ�����ʦ����Ϣ
	MASTER_IS_REGIST, // ��������Ƿ�ע��ʦ����Ϣ

	MODIFY_MASTER_INFO, // �޸�ʦ����Ϣ
	ASK_REG_MASTER,     // �ͻ���Ҫ��ע��ʦ����Ϣ

	CANCLE_REG_MASTER, // ȡ��ʦ��ע����Ϣ

	NPC_OPEN_MASTER_LIST, // npc ��ʦ���б�
	NPC_OPEN_REG_MASTER, // NPC ��ע��ʦ������
	NPC_CANCLE_REG_MASTER, // NPC ȡ��ע��
	REG_MASTER_SUCCESS, // ע��ʦ���ɹ�

	CS_MASTER_BUYBOX,
	CS_STUDENT_GETBOX,
	CS_STUDENT_BUYBOX,
	CS_MASTER_GETBOX,
	CS_MASTER_LEADBOX,
	CS_STUDENT_LEADBOX,

	C_SEND_BAISHI,
};

struct MASTER_C_SEND_BAISHI_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_CS_MASTER_LEADBOX_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_CS_STUDENT_LEADBOX_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_CS_MASTER_BUYBOX_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_CS_STUDENT_GETBOX_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_CS_STUDENT_BUYBOX_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_CS_MASTER_GETBOX_Data
{
	DWORD masterUin;
	DWORD studentUin;
};

struct MASTER_NPC_OPEN_MASTER_LIST_Data
{

};

struct MASTER_REG_MASTER_SUCCESS_Data
{

};

struct MASTER_NPC_OPEN_REG_MASTER_Data
{

};

struct MASTER_NPC_CANCLE_REG_MASTER_Data
{

};

struct MASTER_CANCLE_REG_MASTER_Data
{
	DWORD dwUin;
};

struct MASTER_ASK_MASTER_INFO_Data
{
	DWORD dwUin;
};

struct MASTER_GET_MASTERS_INFO_Data
{
	DWORD dwUin;
};

struct MASTER_ASK_REG_MASTER_Data
{
	DWORD dwUin;
	BYTE byStart;
	BYTE byEnd;
	char szDsc[1];
};

struct MASTER_MODIFY_MASTER_INFO_Data
{
	DWORD dwUin;
	BYTE byStart;
	BYTE byEnd;
	char szDsc[1];
};

struct MASTER_MASTER_IS_REGIST_Data
{
	BYTE byReg;
};


struct MASTER_CS_SEND_TUDI_INFO_Data
{
	DWORD	dwUin;
	char	szName[16];
	BYTE	byLevel;
	BYTE	byJob;
	BYTE	byChuShi;
	BYTE	bOnline;
};

struct MASTER_CS_SEND_MASTER_INFO_Data
{
	DWORD	dwUin;
	char	szName[16];
	char	szHomeName[20];
	BYTE	byLevel;
	BYTE	byJob;
	BYTE	byHonourLvl;
	BYTE	bOnline;
	DWORD   dwCd;
	BYTE	bIsChuShi;
};

struct MASTER_C_MASTERSYS_QUTU_Data
{
	DWORD	dwTudiUin;
};

struct MASTER_C_MASTERSYS_CHUSHI_Data
{
	DWORD	dwTudiUin;
};

struct MASTER_GS_GET_MASTER_TUDI_INFO_Data
{
	DWORD	dwUin;
};

struct MASTER_C_MASTERSYS_SHOUTU_Data
{
	char   szTudiName[20];
};

struct MASTER_C_MASTERSYS_BAISHI_Data
{
	char szShifuName[20];
};

struct MASTER_CS_TEACH_CNT_CHANGE_Data
{
	DWORD uin;
	int  nCnt;
};

struct MASTER_GS_MASTERSYS_COST_GOLD_Data
{
	DWORD	dwUin;
	DWORD	dwPoint;
};

struct MASTER_CS_MASTERSYS_COST_GOLD_Data
{
	DWORD	dwUin;
	DWORD	dwPoint;
};


struct MASTER_REG_MASTERS_INFO_Data
{
	DWORD	m_dwUin;    
	BYTE	m_btSex;   // Sex
	BYTE	m_btJob;
	BYTE	m_btLevel;  // Level
	bool    m_bIsHeimuya;  // true:�ڵ�  false:�׵�
	BYTE    m_byStartime;   // 
	BYTE    m_byEndtime;
	BYTE    m_byOnline;

	char	m_szNameDesc[1];  // name and desc
};

struct MASTER_REG_MASTER_INFO_Data
{
	BYTE    m_byStartime;   // 
	BYTE    m_byEndtime;

	char	m_szNameDesc[1];  // name and desc
};

typedef struct MasterSys_Process
{
	DWORD	dwMasterUin;
	DWORD	dwTudiUin;
	BYTE	bSpecial;
}

MASTER_GS_MASTERSYS_CHUSHI_Data,
MASTER_GS_MASTERSYS_PANSHI_Data,
MASTER_GS_MASTERSYS_QUTU_Data,
MASTER_GS_MASTERSYS_LEVELUP_Data;

typedef struct MASTER_GS_MASTERSYS_BAISHI_Data
{
	DWORD	dwMasterUin;
	DWORD	dwTudiUin;
	int     nBaiShiCd;
	int     nTeachLowLvl;
	int     nStudyLowLvl;
	int     nStudyHighLvl;
};

typedef struct MasterSys_Result
{
	DWORD	dwMasterUin;
	DWORD	dwTudiUin;
	DWORD	dwErrorCode;
}
MASTER_CS_MASTERSYS_BAISHI_RESULT_Data,
MASTER_CS_MASTERSYS_CHUSHI_RESULT_Data,
MASTER_CS_MASTERSYS_PANSHI_RESULT_Data,
MASTER_CS_MASTERSYS_QUTU_RESULT_Data;

struct MASTER_CS_TUDI_LELVEL_CHG_Data
{
	DWORD	dwTudiUin;
	BYTE	byLevel;
};

struct MASTER_CS_MASTER_LELVEL_CHG_Data
{
	BYTE	byLevel;
};

struct MASTER_CS_TUDI_JOB_CHG_Data
{
	DWORD	dwTudiUin;
	BYTE	byJob;
};

struct MASTER_CS_MASTER_JOB_CHG_Data
{
	BYTE	byJob;
};

struct MASTER_CS_MASTER_HOME_CHG_Data
{
	char	szHomeName[20];
};

struct MASTER_CS_MASTER_HONOURLVL_CHG_Data
{
	BYTE	byHonourLvl;
};

struct MASTER_CS_MASTER_ONLINE_Data
{
	BYTE	bOnline;
};

struct MASTER_CS_TUDI_ONLINE_Data
{
	DWORD	dwTudiUin;
	BYTE	bOnline;
};

struct MASTER_CS_MASTERSYS_EXCHAGE_POINT_Data
{
	DWORD	dwUin;
	int		nGoldCnt;
};

struct MASTER_GS_MASTERSYS_EXCHAGE_POINT_Data
{
 	DWORD	dwUin;
};

struct MASTER_AS_MASTERSYS_EXCHAGE_POINT_Data
{
	DWORD	dwUin;
	int		nGoldCnt;
};

struct MASTER_GS_MASTERSYS_MASTER_NAME_Data
{
	DWORD	dwUin;
	char	szMasterName[16];
};
struct MASTER_CS_SEND_IS_SHIELD_Data
{
	bool dwShield;
};
struct MASTER_CS_SEND_MASTER_TEACH_Data
{
	DWORD dwUin;
};
struct MASTER_CS_SEND_MASTER_CD_Data
{
	DWORD dwCd;
};

#pragma pack (pop)
#endif