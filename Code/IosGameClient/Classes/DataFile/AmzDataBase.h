#pragma once

#include "PacketCenter.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif
#include "xnlist.h"

//��ȡ���ݿ�,���紫��,�ͻ��˻�Ҫ֧�ִ��ļ����ȡ���ݵķ�ʽ

enum	AMZDDATA_STATE
{
	AMZ_STATE_NORMAL,
	AMZ_STATE_CHGED,
	AMZ_STATE_DELETE,
	AMZ_STATE_RELEASE,		//���Ա��ͷ���
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

//shipData����ɲ���֮һ
class AmzDataBase
{
public:
	AmzDataBase(BYTE bEnumMax,BYTE	bType, int nID);
	virtual ~AmzDataBase();

	int	operator[](BYTE bEnum);				//��Ҫ��ֵ�͵���ָ����ַ����
	void	SetVal(BYTE bEnum, int nVal);
	int		GetVal(BYTE bEnum);
	void	SyncNet(bool bTotal=false);							//���޸Ĺ���ֵͬ�����ͻ���

	BYTE	bModelType;			//ģ������
	BYTE	bEMax;
	BYTE	bStateFlag;
	int		nModelID;			//���ݱ��

	int		nExData;		//��չ����, ��ֱ����Ϊֵ��,Ҳ������������pbData��ӵĽṹ����
	
	BYTE*	pbData;			//2�������ݿ�
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