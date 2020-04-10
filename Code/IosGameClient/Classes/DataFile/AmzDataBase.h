#pragma once

#include "PacketCenter.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif
#include "xnlist.h"

//存取数据库,网络传输,客户端还要支持从文件里读取数据的方式

enum	AMZDDATA_STATE
{
	AMZ_STATE_NORMAL,
	AMZ_STATE_CHGED,
	AMZ_STATE_DELETE,
	AMZ_STATE_RELEASE,		//可以被释放了
	AMZ_STATE_NEW,
};

enum	AMZDATA_TYPE
{
//	AMZ_DATA_CARD,
	AMZ_DATA_MAGIC,

	AMZ_DATA_ITEM,
	AMZ_DATA_EQUIPEX,

	AMZ_DATA_MAX,
};

//shipData的组成部分之一
class AmzDataBase
{
public:
	AmzDataBase(BYTE bEnumMax,BYTE	bType, int nID);
	virtual ~AmzDataBase();

	int	operator[](BYTE bEnum);				//若要赋值就得用指针或地址返回
	void	SetVal(BYTE bEnum, int nVal);
	int		GetVal(BYTE bEnum);
	void	SyncNet(bool bTotal=false);							//把修改过的值同步到客户端

	BYTE	bModelType;			//模块类型
	BYTE	bEMax;
	BYTE	bStateFlag;
	int		nModelID;			//数据编号

	int		nExData;		//扩展参数, 可直接作为值用,也可以用来描述pbData后接的结构长度
	
	BYTE*	pbData;			//2进制数据块
};

class AmzDataManager	: public PacketListener
{
public:
	AmzDataManager();
	~AmzDataManager();

	void		Update();
	virtual	bool PacketProcess(ZPacket* pPacket);

	AmzDataBase*	AddAmzData(BYTE bEMax, BYTE bMT, int nMID);

	AmzDataBase*	GetAmzData(BYTE bMT, int nMID);
	AmzDataBase*	PopAmzData(BYTE bMT);
	bool			RemoveAmzData(BYTE bMT, int nMID,bool bRelease = false);

	int			GetNewMID(BYTE bMT);

	AmzDataBase*	CreateAmzData(BYTE bEMax,BYTE bMT,char* pData);

	void		ClearData();
private:
	xnList*		m_pListNew;
	xnList*		m_pListAmzD[AMZ_DATA_MAX];
	xnList*		m_pListCacha;
};