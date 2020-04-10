#pragma once

#include <Windows.h>
#include "xnlist.h"

//��ȡ���ݿ�,���紫��,�ͻ��˻�Ҫ֧�ִ��ļ����ȡ���ݵķ�ʽ

#define SAFE_DELETE(p)			{ if(p) { delete (p); (p) = 0; } }

enum	AMZDDATA_STATE
{
	AMZ_STATE_NORMAL,
	AMZ_STATE_CHGED,
	AMZ_STATE_DELETE,
	AMZ_STATE_RELEASE,		//���Ա��ͷ���
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

//shipData����ɲ���֮һ
class DBControl;
class AmzDataBase
{
public:
	AmzDataBase(BYTE bEnumMax,BYTE	bType, int nID);
	virtual ~AmzDataBase();

	int	operator[](BYTE bEnum);				//��Ҫ��ֵ�͵���ָ����ַ����
	void	SetVal(BYTE bEnum, int nVal,bool bSaveDB = true);
	int		GetVal(BYTE bEnum);
	void	SyncNet(bool bTotal=false);							//���޸Ĺ���ֵͬ�����ͻ���

	BYTE	bModelType;			//ģ������
	BYTE	bEMax;
	BYTE	bStateFlag;
	int		nModelID;			//���ݱ��
	LONGLONG	llKey;			//��������е�����Ψһ���

	int		nExData;			//���������,��ֱ����,Ҳ���Ը���pbData����,���ݿ�����ͬһ�� exdata  BLOB����, ���� �ַ�,�����ṹ��

	BYTE*	pbData;			//2�������ݿ�
	int		nLinkCnt;		//���ü���

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