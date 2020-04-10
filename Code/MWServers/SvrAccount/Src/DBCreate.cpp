#include "stdafx.h"
#include "DBCreate.h"
#include "DBTables.h"
#include "DBIO.h"

//--------------------------------------------------------------------------------------------------------------------
int GetDBVersion(DBBase * pDBBase)
{
	DBIO * pDB = pDBBase->GetDBServer();
	QueryRlt xRlt;
	QueryRlt xVersionRlt;

	if(pDB->Query(&xRlt,"SHOW TABLES LIKE '%s'", DB_DATA_VERSION))
	{
		if(pDB->Query(&xVersionRlt,"select * from "DB_DATA_VERSION" where dbname = 'common'"))
		{
			MySqlQuery* pCmd = xVersionRlt.GetRlt();
			if(pCmd->FetchRow()) return atoi(pCmd->GetStr("version"));
		}
	}
	return -1;
}

void UpdateDBVersion(DBBase * pDBBase, int nPrevVersion, int nCurrVersion)
{
	if(nPrevVersion != -1)
	{
		pDBBase->OnExecuteSQL("update "DB_DATA_VERSION" set version = %d where dbname = 'common'", nCurrVersion);
	}
	else
	{
		pDBBase->OnExecuteSQL("create table "DB_DATA_VERSION" (dbname varchar(64))");
		pDBBase->AddColumes(DB_DATA_VERSION, DBVersionTable_Colume, sizeofrecord(DBVersionTable_Colume, DB_Colume));
		pDBBase->OnExecuteSQL("CREATE INDEX IX_NAME ON "DB_DATA_VERSION" (dbname)");
		pDBBase->OnExecuteSQL("insert "DB_DATA_VERSION" (dbname, version) values ('common', %d)", nCurrVersion);
	}
}

//--------------------------------------------------------------------------------------------------------------------
//CenterDB
CreateDB::CreateDB()
{
	m_pDB			= NULL;
	m_dwDBMaxUin	= 0;
	for (int i = 0; i < 10; i++)
	{
		m_ppDB[i]	= NULL;
	}
}

CreateDB::~CreateDB()
{
	if(m_pDB)
		delete m_pDB;
	for (int i = 0; i < 10; i++)
	{
		if(m_ppDB[i])
			delete m_ppDB[i];
	}
}

bool CreateDB::_CreateRpgUserTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_USER_TABLE" (Uin int PRIMARY KEY)");
	AddColumes(RPG_USER_TABLE, RpgUser_Colume, sizeofrecord(RpgUser_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_ACCOUNTUIN ON "RPG_USER_TABLE" (AccountUin)");
	OnExecuteSQL("CREATE INDEX IX_ACCOUNTUIN_UIN ON "RPG_USER_TABLE" (AccountUin, Uin)");
	OnExecuteSQL("CREATE INDEX IX_NICKNAME ON "RPG_USER_TABLE" (Nickname)");
	return TRUE;
}

bool CreateDB::_CreatActiveTable()
{
	OnExecuteSQL("CREATE TABLE "ACTIVATE_TABLE" (id varchar(32))");
	AddColumes(ACTIVATE_TABLE, ActiveTable_Colume, sizeofrecord(ActiveTable_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "ACTIVATE_TABLE" (id)");
	return TRUE;
}

bool CreateDB::_CreateRpgItemTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_ITEM_TABLE" (uin int)");
	AddColumes(RPG_ITEM_TABLE, RpgItem_Colume, sizeofrecord(RpgItem_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_ITEM_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_SLOT ON "RPG_ITEM_TABLE" (uin, id)");
	OnExecuteSQL("CREATE INDEX IX_ItemKeyEx ON "RPG_ITEM_TABLE" (objkey)" );
	return TRUE;
}
bool CreateDB::_CreateRpgMailTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_MAIL_TABLE" (mailid int primary key)");
	AddColumes(RPG_MAIL_TABLE, RpgMail_Colume, sizeofrecord(RpgMail_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN_RECV ON "RPG_MAIL_TABLE" (recv, state)");
	OnExecuteSQL("CREATE INDEX IX_UIN_SEND ON "RPG_MAIL_TABLE" (send, state)");
	return TRUE;
}

bool CreateDB::_CreateRpgNASaleTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_NASALE_TABLE" (naindex int primary key)");
	AddColumes(RPG_NASALE_TABLE, RpgNASale_Colume, sizeofrecord(RpgNASale_Colume, DB_Colume));



	return TRUE;
}

bool CreateDB::_CreateRpgFriendTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_FRIEND_TABLE" (uinA int)");
	AddColumes(RPG_FRIEND_TABLE, RpgFriend_Colume, sizeofrecord(RpgFriend_Colume, DB_Colume));

	return TRUE;
}

bool CreateDB::_CreateRpgWorldTestTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_WORLDTEST_TABLE" (uinA int)");
	AddColumes(RPG_WORLDTEST_TABLE, RpgWorldTest_Colume, sizeofrecord(RpgWorldTest_Colume, DB_Colume));

	return TRUE;
}

// bool CreateDB::_CreateRpgEquipTable()
// {
// 	OnExecuteSQL("CREATE TABLE "RPG_EQUIP_TABLE" (uin int)");
// 	AddColumes(RPG_EQUIP_TABLE, RpgEquip_Colume, sizeofrecord(RpgEquip_Colume, DB_Colume));
// 
// 	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_EQUIP_TABLE" (uin)");
// 	OnExecuteSQL("CREATE INDEX IX_UIN_SLOT ON "RPG_EQUIP_TABLE" (uin, id)");
// 	OnExecuteSQL("CREATE INDEX IX_ItemKeyEx ON "RPG_EQUIP_TABLE" (objkey)" );
// 	return TRUE;
// }
bool CreateDB::_CreateRpgSlaveTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_SLAVE_TABLE" (uin int)");
	AddColumes(RPG_SLAVE_TABLE, RpgSlave_Colume, sizeofrecord(RpgSlave_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_SLAVE_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_SLOT ON "RPG_SLAVE_TABLE" (uin, id)");
	OnExecuteSQL("CREATE INDEX IX_ItemKeyEx ON "RPG_SLAVE_TABLE" (objkey)" );
	return TRUE;
}

bool CreateDB::_CreateRpgNewSlaveTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_NEWSLAVE_TABLE" (uin int)");
	AddColumes(RPG_NEWSLAVE_TABLE, RpgNewSlave_Colume, sizeofrecord(RpgNewSlave_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_NEWSLAVE_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_SLOT ON "RPG_NEWSLAVE_TABLE" (uin, id)");
	OnExecuteSQL("CREATE INDEX IX_ItemKeyEx ON "RPG_NEWSLAVE_TABLE" (objkey)" );
	return TRUE;
}

bool CreateDB::_CreateAccountTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_ACCOUNT_TABLE" (AccountUin int PRIMARY KEY AUTO_INCREMENT)");
	AddColumes(RPG_ACCOUNT_TABLE, Account_Colume, sizeofrecord(Account_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_ACCOUNT_TABLE" (AccountUin)");
	OnExecuteSQL("CREATE INDEX IX_ACC ON "RPG_ACCOUNT_TABLE" (accname)");
	OnExecuteSQL("ALTER TABLE "RPG_ACCOUNT_TABLE" AUTO_INCREMENT = 10000");

	return TRUE;
}

bool CreateDB::_CreateMkRegAccountTable()
{
	OnExecuteSQL("CREATE TABLE "MARK_REGACCOUNT_TABLE" (id int PRIMARY KEY)");
	AddColumes(MARK_REGACCOUNT_TABLE, MkRegAcc_Colume, sizeofrecord(MkRegAcc_Colume, DB_Colume));
	return TRUE;
}

bool CreateDB::_CreateMkOnlineTable()
{
	OnExecuteSQL("CREATE TABLE "MARK_ONLINECNT_TABLE" (id int PRIMARY KEY AUTO_INCREMENT)");
	AddColumes(MARK_ONLINECNT_TABLE, MkOnline_Colume, sizeofrecord(MkOnline_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_TIME ON "MARK_ONLINECNT_TABLE" (timemark)");
	return TRUE;
}

bool CreateDB::_CreateIosADSupTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_IOSAD_TABLE" (id int PRIMARY KEY AUTO_INCREMENT)");
	AddColumes(RPG_IOSAD_TABLE, IosADSup_Colume, sizeofrecord(IosADSup_Colume, DB_Colume));
	OnExecuteSQL("CREATE INDEX IX_MAC ON "RPG_IOSAD_TABLE" (macad)");
	OnExecuteSQL("CREATE INDEX IX_TPNAME ON "RPG_IOSAD_TABLE" (tpname)");
	OnExecuteSQL("CREATE INDEX IX_IDFA ON "RPG_IOSAD_TABLE" (idfa)");
	OnExecuteSQL("CREATE INDEX IX_MACIDFA ON "RPG_IOSAD_TABLE" (macad,idfa)");
	return TRUE;
}

bool CreateDB::_CreateMkRmbCostTable()
{
	OnExecuteSQL("CREATE TABLE "MARK_RMBCOST_TABLE" (objid int PRIMARY KEY)");
	AddColumes(MARK_RMBCOST_TABLE, MkRmbCost_Colume, sizeofrecord(MkRmbCost_Colume, DB_Colume));
	return TRUE;
}

bool CreateDB::_CreateFamilyTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_HOME_TABLE" (homeid int)");
	AddColumes(RPG_HOME_TABLE, Home_Colume, sizeofrecord(Home_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_HOME_TABLE" (homeid)");
	return TRUE;
}

bool CreateDB::CreateCharacterDB(void)
{
	int nVersion = GetDBVersion(this);
	if(nVersion == DB_VERSION) return true;

	_CreateIosADSupTable();
	_CreateMkRegAccountTable();
	_CreateMkOnlineTable();
	_CreateMkRmbCostTable();
	_CreateAccountTable();
	_CreateRpgUserTable();
	_CreateRpgItemTable();
	_CreatActiveTable();
	_CreateRpgEquipExTable();
	_CreateRpgSlaveTable();
	_CreateRpgMailTable();
	_CreateRpgNASaleTable();
	_CreateRpgFriendTable();
	_CreateFamilyTable();
	_CreateRpgNewSlaveTable();
	_CreateRpgQuestTable();
	_CreateRpgInviteTable();
	_CreateRpgWorldTestTable();
	_CreateRpgFortTable();
	_CreateRpgRankTable();
	_CreateRpgRankUserTable();
	_CreateRpgRankUWTable();
	_CreateSkillTable();
	_CreateStateTable();
	_CreateRpgItemManageTable();
	_CreateRpgSysMailTable();
	_CreateRpgWarRankTable();
	_CreateRpgWorldFlagTable();
	_CreateRpgMentoringTable();
	_CreateRpgMRequestTable();
	_CreateRpgAchievementTable();
	_CreateRpgMarryTable();
	_CreateRpgMarryRequestTable();
	_CreatePayTable();
	_CreateMarkLoginOutTable();

	UpdateDBVersion(this, nVersion, DB_VERSION);
	return true;
}

bool CreateDB::_CreateRpgEquipExTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_EQUIPEX_TABLE" (id int)");
	AddColumes(RPG_EQUIPEX_TABLE, RpgEquipEx_Colume, sizeofrecord(RpgEquipEx_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_EQUIPEX_TABLE" (id)");
	OnExecuteSQL("CREATE INDEX IX_ItemKeyEx ON "RPG_EQUIPEX_TABLE" (objkey)" );
	return TRUE;
}

bool CreateDB::_CreateRpgQuestTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_QUEST_TABLE" (uin int)");
	AddColumes(RPG_QUEST_TABLE, RpgQuest_Colume, sizeofrecord(RpgQuest_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_QUEST_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_QID ON "RPG_QUEST_TABLE" (uin,qid)");
	return TRUE;
}

bool CreateDB::_CreateSkillTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_SKILL_TABLE" (uin int)");
	AddColumes(RPG_SKILL_TABLE, RpgSkill_Colume, sizeofrecord(RpgSkill_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_SKILL_TABLE" (uin)");
	return TRUE;
}

bool CreateDB::_CreateStateTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_STATE_TABLE" (uin int)");
	AddColumes(RPG_STATE_TABLE, RpgState_Colume, sizeofrecord(RpgState_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_STATE_TABLE" (uin)");
	return TRUE;
}

bool CreateDB::_CreateRpgFortTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_FORT_TABLE" (uin int)");
	AddColumes(RPG_FORT_TABLE, RpgFort_Colume, sizeofrecord(RpgFort_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_FORT_TABLE" (uin)");
	return TRUE;
}

bool CreateDB::_CreateRpgMentoringTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_MENTORING_TABLE" (id int)");
	AddColumes(RPG_MENTORING_TABLE, MentoringTable_Colume, sizeofrecord(MentoringTable_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_MENTORING_TABLE" (id)");
	return TRUE;
}

bool CreateDB::_CreateRpgMRequestTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_MREQUEST_TABLE" (uin int)");
	AddColumes(RPG_MREQUEST_TABLE, MentoringRequest_Colume, sizeofrecord(MentoringRequest_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_MREQUEST_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_VALUE ON "RPG_MREQUEST_TABLE" (uin,value)");
	return TRUE;
}

bool CreateDB::_CreateRpgMarryTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_MARRY_TABLE" (id int)");
	AddColumes(RPG_MARRY_TABLE, MarryTable_Colume, sizeofrecord(MarryTable_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_MARRY_TABLE" (id)");
	return TRUE;
}

bool CreateDB::_CreateRpgMarryRequestTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_MARRYREQUEST_TABLE" (uin int)");
	AddColumes(RPG_MARRYREQUEST_TABLE, MarryRequest_Colume, sizeofrecord(MarryRequest_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_MARRYREQUEST_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_VALUE ON "RPG_MARRYREQUEST_TABLE" (uin,value)");
	return TRUE;
}

bool CreateDB::_CreateRpgAchievementTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_ACHIEVEMENT_TABLE" (uin int)");
	AddColumes(RPG_ACHIEVEMENT_TABLE, Achievement_Colume, sizeofrecord(Achievement_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_ACHIEVEMENT_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_ID ON "RPG_ACHIEVEMENT_TABLE" (uin,id)");

	return TRUE;
}

bool CreateDB::_CreateRpgRankTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_RANKDATA_TABLE" (id1 int)");
	AddColumes(RPG_RANKDATA_TABLE, RpgRankData_Colume, sizeofrecord(RpgRankData_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_RANKDATA_TABLE" (id1)");
	OnExecuteSQL("CREATE INDEX IX_TYPE_POS ON "RPG_RANKDATA_TABLE" (type,pos)");
	OnExecuteSQL("CREATE INDEX IX_TYPE_ID ON "RPG_RANKDATA_TABLE" (type,id1,id2)");
	return TRUE;
}

bool CreateDB::_CreateRpgRankUserTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_USERINFO_TABLE" (uin int)");
	AddColumes(RPG_USERINFO_TABLE, RpgRankUser_Colume, sizeofrecord(RpgRankUser_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_USERINFO_TABLE" (uin)");
	return TRUE;
}

bool CreateDB::_CreateRpgRankUWTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_UWATCH_TABLE" (uin int)");
	AddColumes(RPG_UWATCH_TABLE, RpgRankUWatch_Colume, sizeofrecord(RpgRankUWatch_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_UWATCH_TABLE" (uin)");
	OnExecuteSQL("CREATE INDEX IX_UIN_ID_TYPE ON "RPG_UWATCH_TABLE" (uin,id,type)");
	return TRUE;
}


bool CreateDB::_CreateRpgItemManageTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_ITEM_MANAGE_TABLE" (id int)");
	AddColumes(RPG_ITEM_MANAGE_TABLE, RpgItemManage_Colume, sizeofrecord(RpgItemManage_Colume, DB_Colume));


	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_ITEM_MANAGE_TABLE" (id)");
	return TRUE;
}

bool CreateDB::_CreateRpgSysMailTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_SYS_MAIL_TABLE" (id int primary key)");
	AddColumes(RPG_SYS_MAIL_TABLE, RpgSysMail_Colume, sizeofrecord(RpgSysMail_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN_RECV ON "RPG_SYS_MAIL_TABLE" (id)");
	return TRUE;
}

bool CreateDB::_CreateRpgWarRankTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_RANK_WAR_TABLE" (pos int primary key)");
	AddColumes(RPG_RANK_WAR_TABLE, RpgWarRank_Colume, sizeofrecord(RpgWarRank_Colume, DB_Colume));
	return TRUE;
}

bool CreateDB::_CreateRpgWorldFlagTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_WORLDFLAG_TABLE" (id int primary key)");
	AddColumes(RPG_WORLDFLAG_TABLE, RpgWorldFlag_Colume, sizeofrecord(RpgWorldFlag_Colume, DB_Colume));
	return TRUE;
}

bool CreateDB::_CreateRpgInviteTable()
{
	OnExecuteSQL("CREATE TABLE "RPG_INVITE_TABLE" (uinsvr int)");
	AddColumes(RPG_INVITE_TABLE, RpgInvite_Colume, sizeofrecord(RpgInvite_Colume, DB_Colume));

	OnExecuteSQL("CREATE INDEX IX_UIN ON "RPG_INVITE_TABLE" (uinsvr)");
	OnExecuteSQL("CREATE INDEX IX_IVCD ON "RPG_INVITE_TABLE" (invitecode)");

	return TRUE;
}

bool CreateDB::_CreatePayTable()
{
	OnExecuteSQL("CREATE TABLE "MARK_PAY_TABLE" (accuin int)");
	AddColumes(MARK_PAY_TABLE, MkPay_Colume, sizeofrecord(MkPay_Colume, DB_Colume));
	return TRUE;
}

bool CreateDB::_CreateMarkLoginOutTable()
{
	OnExecuteSQL("CREATE TABLE "MARK_LOGINOUT_TABLE" (uin int)");
	AddColumes(MARK_LOGINOUT_TABLE, MkLoginOut_Colume, sizeofrecord(MkLoginOut_Colume, DB_Colume));
	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
//DBBase

DBBase::DBBase()
{
	m_pDB	= NULL;
	for (int i = 0; i < 10; i++)
	{
		m_ppDB[i]	= NULL;
	}
}

DBBase::~DBBase()
{

}

//connect and close
bool DBBase::OnConnectToDB(const char* szDBname,const char* szAddress,const char* szUser,const char* szPWD)
{
	if(m_pDB != NULL)
	{
		delete m_pDB;
		m_pDB = NULL;
	}
	m_pDB = new DBIO("create");
	if(!m_pDB->ConnectDB(szAddress,szUser,szPWD,szDBname,true))
	{
		m_pDB->DisconnectDB();
		delete m_pDB;
		m_pDB = NULL;
		return false;
	}
	for (int i = 0; i < 10; i++)
	{
		m_ppDB[i] = new DBIO("create");
		if(!m_ppDB[i]->ConnectDB(szAddress,szUser,szPWD,szDBname,false))
		{
			m_ppDB[i]->DisconnectDB();
			delete m_ppDB[i];
			m_ppDB[i] = NULL;
		}
	}

	return true;
}

void DBBase::OnClose()
{
	if(m_pDB != NULL)
	{
		m_pDB->DisconnectDB();
		delete m_pDB;
		m_pDB = NULL;
	}
}

void DBBase::AddColume(char * tableName, char * columeName, char * columeType, char * defaultValue, char * prevName)
{
	if (strnicmp(columeType, "longBLOB", 8) == 0 || strnicmp(columeType, "TINYBLOB", 8) == 0
		||strnicmp(columeType, "BLOB", 4) == 0 || strnicmp(columeType, "MediumBLOB", 10) == 0)
	{
		OnExecuteSQL("ALTER TABLE %s ADD %s %s NOT NULL", tableName, columeName, columeType);
		if(prevName)
			OnExecuteSQL("ALTER TABLE %s MODIFY COLUMN %s %s NOT NULL AFTER %s", tableName, columeName, columeType,prevName);
	}
	else	
	{
		OnExecuteSQL("ALTER TABLE %s ADD %s %s NOT NULL DEFAULT %s", 
			tableName, columeName, columeType, defaultValue ? defaultValue : "0");
		if(prevName)
			OnExecuteSQL("ALTER TABLE %s MODIFY COLUMN %s %s NOT NULL  DEFAULT %s AFTER %s", tableName, columeName, columeType, defaultValue ? defaultValue : "0",prevName);
	}
}

void DBBase::AddColumes(char * tableName, DB_Colume * columes, int count)
{
	for (int i=0; i<count; i++)
	{
		AddColume(tableName, columes[i].szName, columes[i].szType, columes[i].szDefault);
	}
}

bool DBBase::OnExecuteSQL(const char* szFormat,...)
{
	char szSQL[4096]={0};
	vsprintf(szSQL,szFormat,(char*)(&szFormat+1));
	//WriteLogFile(szSQL);
	return 	(m_pDB->ExecuteSQL(szSQL));
}

DBIO* DBBase::RandGetDB()
{
	int nRand	= rand()%10000;
	for (int i = nRand; i < nRand+10; i++)
	{
		if(m_ppDB[nRand%10])
			return m_ppDB[nRand%10];
	}
	return m_pDB;
}


#define WM_EXCUTE_LOGSQL WM_USER + 2000

BOOLEAN LogThreadFunc(void * data, MSG * pMsg)
{
	if(pMsg->message == WM_EXCUTE_LOGSQL)
	{
		LogDB * pLogDB = (LogDB *)data;
		char * szSql = (char *)pMsg->lParam;

		if(!pLogDB->OnExecuteSQL(szSql))
		{
			//WriteDBError("%s\n", szSql);
		}
		free(szSql);
	}
	return true;
}

inline void LogDB::_OnExecuteSQLInThread(const char * szFormat, ...)
{
	if(szFormat == NULL)
		return;

	char szSql[4096]={0};
	vsprintf(szSql, szFormat, (char*)(&szFormat + 1));

	if(m_pThread->IsRunning())
	{
		m_pThread->SendMessage(WM_EXCUTE_LOGSQL, 0, (LPARAM)(strdup(szSql)));
		return;
	}
	//WriteDBError("log db thread is not running!\n");
	OnExecuteSQL(szSql);
}

bool LogDB::CreateLogDB()
{
	char szTmp[4096];
	sprintf(szTmp,"CREATE TABLE IF NOT EXISTS  `coin_logs` (\
				  `id` int(11) NOT NULL auto_increment,\
				  `accid` int(11) unsigned NOT NULL default '0',\
				  `accname` varchar(20) NOT NULL default '',\
				  `gold_coin` int(11) unsigned NOT NULL default '0',\
				  `payrmb` int(11) unsigned NOT NULL default '0',\
				  `action` char(10) NOT NULL default '',\
				  `client_ip` varchar(20) NOT NULL default '',\
				  `game_server` varchar(20) NOT NULL default '',\
				  `tran_id` varchar(50) NOT NULL default '',\
				  `cdate` timestamp NOT NULL default CURRENT_TIMESTAMP,\
				  PRIMARY KEY  (`id`),\
				  KEY `idx_tid` (`tran_id`)\
				  );");
	OnExecuteSQL(szTmp);

	return true;
}

LogDB::LogDB()
{
	m_pDB = NULL;
	m_pThread = new xnThread(LogThreadFunc, this);
	m_pThread->Start();
}

LogDB::~LogDB()
{
	if(m_pThread)
	{
		m_pThread->Stop();
		delete m_pThread;
	}
}

void LogDB::SaveTPPayLog( DWORD uin,char* szAcc,BYTE bAction,const char* szData, int gold,int nRmb )
{
	OnExecuteSQL("Insert Into coin_logs (accid,accname,gold_coin,payrmb,action,tran_id) values (%d,'%s', %d, %d, '%d','%s')",
		uin, szAcc,gold,nRmb, bAction,szData);
}

bool LogDB::CheckOrderNo( char* szOrde )
{
	if(m_pDB == NULL) return false;
	QueryRlt xRlt;
	if(!m_pDB->Query(&xRlt,"SELECT * FROM coin_logs where tran_id = '%s'",szOrde))
	{
		return false;
	}
	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd == NULL || !pCmd->FetchRow()) return false;
	return true;
}