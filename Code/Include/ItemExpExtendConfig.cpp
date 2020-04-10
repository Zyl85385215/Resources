#include "itemexpextendconfig.h"
#include "xnfile.h"

CItemExpExtendConfig * CItemExpExtendConfig::m_pItemExpExtendConfig = NULL;

bool CItemExpExtendConfig::Load(const char * szFileName)
{
	int nSize = 0;
	BYTE * pFileData = (BYTE*)xnFileRead((char*)szFileName, &nSize);

	if(nSize == 0 || pFileData == NULL) return false;

	int nTempSize = 0;
	BYTE * pData = pFileData;

	//解析二进制数据
	while(nTempSize < nSize)
	{
		LevelExtendInfoMap * pMap = new LevelExtendInfoMap;

		//物品id
		WORD wItemId = *(WORD*)pData;	
		pData+=2;
		//等级段数量
		BYTE bLevelCnt = *(BYTE*)pData;
		pData++;
		//读取每个等级段
		for(int i=0; i<bLevelCnt; i++)
		{
			ItemExpExtendInfo * pItemExpExtendInfo = new ItemExpExtendInfo;
			memcpy(pItemExpExtendInfo, pData, sizeof(ItemExpExtendInfo));
			pData += sizeof(ItemExpExtendInfo);

			(*pMap)[pItemExpExtendInfo->bLevel] = pItemExpExtendInfo;
		}
		m_configMap[wItemId] = pMap;

		nTempSize += 3 + sizeof(ItemExpExtendInfo) * bLevelCnt;
	}

	free(pFileData);
	return true;
}

bool CItemExpExtendConfig::GetExpExtendInfo(DWORD wItemId, BYTE bLevel, ItemExpExtendInfo& xExtendInfo)
{
	LevelExtendInfoMap * pLevelExtendDataMap = m_configMap[wItemId];
	if(!pLevelExtendDataMap) return false;

	ItemExpExtendInfo * pItemExpExtendInfo = (*pLevelExtendDataMap)[bLevel];
	if(!pItemExpExtendInfo) return false;

	memcpy(&xExtendInfo, pItemExpExtendInfo, sizeof(ItemExpExtendInfo));
	return true;
}

CItemExpExtendConfig::CItemExpExtendConfig()
{

}

CItemExpExtendConfig::~CItemExpExtendConfig()
{
	ItemLevelMap::iterator itemLevelMap_iterator = m_configMap.begin();
	for(; itemLevelMap_iterator != m_configMap.end(); itemLevelMap_iterator++)
	{
		LevelExtendInfoMap * pMap = itemLevelMap_iterator->second;

		if(!pMap) continue;

		LevelExtendInfoMap::iterator levelExtendDataMap_iterator = pMap->begin();

		for(; levelExtendDataMap_iterator != pMap->end(); levelExtendDataMap_iterator++)
		{
			ItemExpExtendInfo * pItemExpExtendInfo = levelExtendDataMap_iterator->second;
			SAFE_DELETE(pItemExpExtendInfo);
		}
		SAFE_DELETE(pMap);
	}
}
