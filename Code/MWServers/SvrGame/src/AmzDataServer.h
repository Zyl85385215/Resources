#pragma once

#include <Windows.h>
#include "xnlist.h"

//存取数据库,网络传输,客户端还要支持从文件里读取数据的方式

#define SAFE_DELETE(p)			{ if(p) { delete (p); (p) = 0; } }

enum	AMZDDATA_STATE
{
	AMZ_STATE_NORMAL,
	AMZ_STATE_CHGED,
	AMZ_STATE_DELETE,
	AMZ_STATE_RELEASE,		//可以被释放了
	AMZ_STATE_CREATE,
	AMZ_STATE_DBGET,
	AMZ_STATE_CHGED_NOTSAVEDB,
};

enum	AMZDATA_TYPE
{
	//AMZ_DATA_CARD,
	AMZ_DATA_SLAVE,
	AMZ_DATA_ITEM,
	AMZ_DATA_EQUIPEX,
	AMZ_DATA_MAX,
};

//shipData的组成部分之一
class DBControl;
class AmzDataBase
{
public:
	AmzDataBase(BYTE bEnumMax,BYTE	bType, int nID);
	virtual ~AmzDataBase();

	int	operator[](BYTE bEnum);				//若要赋值就得用指针或地址返回
	void	SetVal(BYTE bEnum, int nVal,bool bSaveDB = true);
	int		GetVal(BYTE bEnum);
	void	SyncNet(bool bTotal=false);							//把修改过的值同步到客户端

	BYTE	bModelType;			//模块类型
	BYTE	bEMax;
	BYTE	bStateFlag;
	int		nModelID;			//数据编号
	LONGLONG	llKey;			//服务端特有的数据唯一编号

	int		nExData;			//额外的数据,可直接用,也可以跟在pbData后面,数据库里用同一的 exdata  BLOB类型, 用于 字符,其他结构体

	BYTE*	pbData;			//2进制数据块
	int		nLinkCnt;		//引用计数

	inline	int		GetByteSize(){return bEMax*sizeof(int);};
	void	GetByteData(void* pData){memcpy(pData,pbData,GetByteSize());};
};

class AmzDataManager
{
public:
	AmzDataManager(DWORD dwUin, DBControl* pDB);
	~AmzDataManager();

	void		Update();
	void		SaveLeave();

	AmzDataBase*	AddAmzData(BYTE bEMax, BYTE bMT, int nMID);

	AmzDataBase*	GetAmzData(BYTE bMT, int nMID);
	int				GetAmzTCnt(BYTE bMT);
	bool			RemoveAmzData(BYTE bMT, int nMID,bool bRelease = false);

	void			DBInit();

	AmzDataBase*				GetAmzDataByPos(BYTE bMT, int nCount);

	bool			AddAmzData(AmzDataBase* pBase);

	static AmzDataBase*			CreateAmzData(BYTE bEMax,BYTE bMT,char* pData = NULL);

	DBControl*	m_pDB;
private:
	char**		_getDBValsName(BYTE bMType);
	void		_saveAmzDB(AmzDataBase* pData);
	void		_insertAmzDB(AmzDataBase* pData);
	void		_removeAmzDB(AmzDataBase* pData);

	xnList*		m_pListAmzD[AMZ_DATA_MAX];
	xnList*		m_pListCacha;
	DWORD		m_dwUin;
};