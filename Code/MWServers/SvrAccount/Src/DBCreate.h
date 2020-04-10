#ifndef _DBOPERATE_H
#define _DBOPERATE_H

#include "DBIO.h"
#include "xnThread.h"


//some other class

class DBIO;
struct DB_Colume;
#define sizeofrecord(a, b) (sizeof(a)/sizeof(b))

class DBBase
{
public:
	DBBase();
	~DBBase();
	DBIO*			GetDBServer() {return m_pDB;}	
	DBIO*			RandGetDB();
	bool			 OnConnectToDB(const char* szDBname,const char* szAddress,const char* szUser,const char* szPWD);
	void	         AddColume(char * tableName, char * columeName, char * columeType, char * defaultValue, char * prevName = NULL);
	void	         AddColumes(char * tableName, DB_Colume * columes, int count);
	bool			 OnExecuteSQL(const char* szFormat,...);
	void			 OnClose();

	DBIO*        m_pDB;
	DBIO*        m_ppDB[10];
};

struct UseBookItemInfo
{ 
	int nSkillID;
	int nItemID;
	int nNum;
};
//CenterDB
class CreateDB:public DBBase
{
public:
	CreateDB();
	~CreateDB();
	bool			CreateCharacterDB();
private:
	DWORD           m_dwDBMaxUin;
	bool            _CreateIosADSupTable();

	bool            _CreateMkRegAccountTable();
	bool            _CreateMkOnlineTable();
	bool            _CreateMkRmbCostTable();

	bool            _CreateAccountTable();

	bool            _CreateRpgUserTable();
	bool            _CreateRpgItemTable();
	bool            _CreatActiveTable();

	bool            _CreateRpgEquipExTable();
	//bool			 _CreateRpgEquipTable();
	bool			 _CreateRpgSlaveTable();
	bool			_CreateFamilyTable();
	bool            _CreateRpgMailTable();
	bool            _CreateRpgNASaleTable();
	bool            _CreateRpgSysMailTable();
	bool            _CreateRpgFriendTable();
	bool			_CreateRpgNewSlaveTable();
	bool			_CreateRpgQuestTable();
	bool			_CreateRpgInviteTable();
	bool			_CreateRpgWorldTestTable();
	bool			_CreateRpgFortTable();
	bool			_CreateRpgRankTable();
	bool			_CreateRpgRankUserTable();
	bool			_CreateRpgRankUWTable();
	bool			_CreateRpgItemManageTable();
	bool			_CreateRpgWarRankTable();
	bool			_CreateRpgWorldFlagTable();

	bool			_CreateSkillTable();
	bool			_CreateStateTable();
	bool			_CreateRpgMentoringTable();
	bool			_CreateRpgMRequestTable();
	bool			_CreateRpgAchievementTable();
	bool			_CreateRpgMarryTable();
	bool			_CreateRpgMarryRequestTable();
	bool			_CreatePayTable();
	bool			_CreateMarkLoginOutTable();
};

class LogDB:public DBBase
{
public:
	LogDB();
	~LogDB();
	bool					CreateLogDB();
	void					SaveTPPayLog(DWORD	uin,char* szAcc,BYTE bAction,const char* szData, int gold,int nRmb);
	bool					CheckOrderNo(char* szOrde);
private:
	void					_OnExecuteSQLInThread(const char * szFormat, ...);
	xnThread*				m_pThread;
};

#endif