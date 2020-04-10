#ifndef _MASTER_PACKET_H_
#define _MASTER_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum MASTER_ERROR_CODE
{
	MASTER_RESULT_OK,						// 成功,所有
	MASTER_RESULT_ERROR_MASTER_SIMPLEINFO,  // 没有师傅SI,所有
	MASTER_RESULT_ERROR_TUDI_SIMPLEINFO,	// 没有徒弟SI,所有
	MASTER_RESULT_ERROR_HAVE_MASTER,		// 徒弟有师傅,拜师
	MASTER_RESULT_ERROR_TUDILVL,            // 徒弟等级问题, 拜师，出师
	MASTER_RESULT_ERROR_MASTERLVL,			// 师傅等级问题, 拜师
	MASTER_RESULT_ERROR_DELTALVL,			// 等级差问题, 拜师
	MASTER_RESULT_ERROR_TUDI_COUNT,			// 徒弟数量问题,拜师
	MASTER_RESULT_ERROR_IN_TMGR,			// CTudiMgr操作失败,出师，叛离，驱逐
	MASTER_RESULT_ERROR_HASCHUSHI,
	MASTER_RESULT_ERROR_IN_CD,
	MASTER_RESULT_ERROR_IN_TUDI_REFUSE,
	MASTER_RESULT_ERROR_IN_MASTER_REFUSE,

	MASTER_RESULT_OK_SPECIAL,			//特殊不扣钱
};

enum MASTER_PACKET_MENU
{
	// 客户端请求信息
	C_GET_MASTER_TUDI_INFO,  //------->GS命令
	GS_GET_MASTER_TUDI_INFO, //------->CS含uin的包

	// 上线发送-------------->GS&C
	CS_SEND_TUDI_INFO,	  
	CS_SEND_MASTER_INFO,	// 游戏服务器会拦截获取MASTER

	
	// 玩家交互-------------->GS
	C_MASTERSYS_CHUSHI,
	C_MASTERSYS_PANSHI,		// 命令，叛离师门
	C_MASTERSYS_QUTU,		// 徒弟UIN,驱逐逆徒

	//	--------------------->C
	GS_MASTERSYS_PROCESS_WAIT,	// 给客户端，表示中心服务器正在处理师徒系统操作

	//	--------------------->CS
	GS_MASTERSYS_BAISHI,	// 拜师,脚本
	GS_MASTERSYS_CHUSHI,	// 出师,脚本
	GS_MASTERSYS_PANSHI,	// 叛离师门
	GS_MASTERSYS_QUTU,		// 驱逐逆徒

	//	--------------------->C
	CS_MASTERSYS_BAISHI_RESULT,
	CS_MASTERSYS_CHUSHI_RESULT,
	CS_MASTERSYS_PANSHI_RESULT,
	CS_MASTERSYS_QUTU_RESULT,

    // -------------------------------以下为新的扩展包，客户端需要处理的------------------------------------
	// 显示信息改变---------->C
	CS_TUDI_LELVEL_CHG,
	CS_MASTER_LELVEL_CHG,
	CS_TUDI_JOB_CHG,
	CS_MASTER_JOB_CHG,
	CS_MASTER_HOME_CHG,
	CS_MASTER_HONOURLVL_CHG,
	CS_TUDI_ONLINE,
	CS_MASTER_ONLINE,

	//  --------------------->GS
	C_MASTERSYS_EXCHAGE_POINT,         // 客户端申请,命令

	//  --------------------->GS & C
	CS_MASTERSYS_EXCHAGE_POINT,        // 中心服务器积分减少通知服务器增加元宝，成员：谁，多少元宝
	
	//	--------------------->CS 
	GS_MASTERSYS_COST_GOLD,			   // 徒弟消费了金元宝，成员：谁，多少元宝
	GS_MASTERSYS_EXCHAGE_POINT,        // 请求中心服务器扣积分，成员：谁

	//	--------------------->CS
	CS_MASTERSYS_COST_GOLD,
	CS_MASTERSYS_IS_CHUSHI,
	GS_MASTERSYS_MASTER_NAME,          // 出师徒弟拥有的称号
	CS_SEND_IS_SHIELD,		//客户端是否勾选屏蔽拜师
	CS_SEND_MASTER_TEACH,
	CS_SEND_MASTER_CD,
	C_MASTERSYS_SHOUTU,
	GS_MASTERSYS_IS_PANSHI,
	C_MASTERSYS_BAISHI,
	CS_TEACH_CNT_CHANGE,

	GET_MASTERS_INFO,  // 客户端要求获得师傅列表
	REG_MASTERS_INFO,  // 回应注册师傅信息
	REG_MASTER_INFO,   // 回应客户端请求个人师傅信息

	ASK_MASTER_INFO,  // 客端请求师傅信息
	MASTER_IS_REGIST, // 发送玩家是否注册师傅消息

	MODIFY_MASTER_INFO, // 修改师傅信息
	ASK_REG_MASTER,     // 客户端要求注册师傅信息

	CANCLE_REG_MASTER, // 取消师傅注册信息

	NPC_OPEN_MASTER_LIST, // npc 打开师傅列表
	NPC_OPEN_REG_MASTER, // NPC 打开注册师傅界面
	NPC_CANCLE_REG_MASTER, // NPC 取消注册
	REG_MASTER_SUCCESS, // 注册师傅成功

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
	bool    m_bIsHeimuya;  // true:黑道  false:白道
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