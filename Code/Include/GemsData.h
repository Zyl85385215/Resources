#ifndef _GEMSDATA_H
#define _GEMSDATA_H

#include <Windows.h>

#pragma pack (push)
#pragma pack (1)

#define MAX_GEM_POSITION 8

#define GEMS_DATA_VERSION_NEWEST GEMS_DATA_VERSION_0

class CGemsData;
typedef bool (CGemsData::*SetGemsDataFunc)(BYTE * pbGemsData, int nSize);
typedef bool (CGemsData::*GetGemsDataFunc)(BYTE * pbGemsData, int & nSize);

enum
{
	GEMS_DATA_VERSION_0,
	GEMS_DATA_VERSION_MAX
};

extern SetGemsDataFunc m_pSetGemsDataFuncArray[GEMS_DATA_VERSION_MAX];
extern GetGemsDataFunc m_pGetGemsDataFuncArray[GEMS_DATA_VERSION_MAX];

struct GemData
{
	BYTE	bPos;
	int		nGemID;
};

class CGemsData
{
protected:
	int m_GemsArray[MAX_GEM_POSITION];
	int	m_nGemsCount;
	int m_nSlotCount;

public:
	CGemsData(void);
	CGemsData(BYTE * pbData, int nSize);
	CGemsData(CGemsData * pGemsData);
	~CGemsData(void);

	inline BYTE GetSlotCount(void) { return m_nSlotCount; }
	inline BYTE GetGemsCount(void) { return m_nGemsCount; }
	
	bool AddGem(BYTE bPos, int nGemID);
	bool DelGem(BYTE bPos);
	int	 GetGem(BYTE bPos);
	bool AddSlot();

	bool SetGemsDataV0(BYTE * pbData, int nSize);
	bool GetGemsDataV0(BYTE * pbData, int & nSize);

	bool GetGemsData(BYTE * pbData, int & nSize);
};

#pragma pack (pop)

#endif