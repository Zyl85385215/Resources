#include "ItemPlusConfig.h"
#include "xnExtstr.h"
#include <string>

CreateSingleton(ItemPlusConfig);
CreateSingleton(ItemReputeConfig);
CreateSingleton(UpgradeStoneUpgradeConfig);
CreateSingleton(ItemReplaceConfig);

static void SetValue(ItemPlusData * pData,int nTypeNum ,char * szText)
{
	sscanf(szText,"%d|%d",
		&pData->nMinValue[nTypeNum],&pData->nMaxValue[nTypeNum]);
}

static void SetRideValue(RidePlusData * pData,int nTypeNum, char * szText)
{
	sscanf(szText,"%d|%d",
		&pData->nMinValue[nTypeNum],&pData->nMaxValue[nTypeNum]);
}

bool ItemPlusConfig::load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nCurSectionCount = 0;
	for(int i=1; i<=4; i++)
	{
		for (int j=0;j<item_plus_type_MAX;j++)
		{
			nCurSectionCount++;
			char szSectionName[32];
			sprintf(szSectionName,"%d%d",i,j);
			pIni->SetSection(szSectionName);
			int nJobIndex = i-1;
			int nType = j;
			m_pData[nJobIndex][nType] = new ItemPlusData;
			ZeroMemory(m_pData[nJobIndex][nType],sizeof(ItemPlusData));
			int nTypeNum=0;
			char * szText = NULL;
			int nPlusType = 0;
			szText = pIni->ReadString("waigong");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("neigong");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("waifang");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("neifang");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("shengming");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("neili");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("mingzhong");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("duobi");
			if(szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("zhongji");
			if (szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;

			szText = pIni->ReadString("zhiming");
			if (szText&&nTypeNum<item_plus_MAX)
			{
				m_pData[nJobIndex][nType]->bValueType[nTypeNum] = nPlusType;
				SetValue(m_pData[nJobIndex][nType],nTypeNum,szText);
				szText = NULL;
				nTypeNum++;
			}
			nPlusType++;
		}
	}
	for (int i=nCurSectionCount;i<pIni->sectionCount;i++)
	{
		pIni->SetSection(i);
		int nTypeNum=0;
		char * szText = NULL;
		szText = pIni->ReadString("waigong");
		RidePlusData * pData = new RidePlusData;
		pData->nID = atoi(pIni->sectionName);
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("neigong");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("waifang");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("neifang");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("shengming");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("neili");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("mingzhong");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("duobi");
		if(szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("zhongji");
		if (szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}

		szText = pIni->ReadString("zhiming");
		if (szText&&nTypeNum<item_plus_MAX)
		{
			SetRideValue(pData,nTypeNum,szText);
			szText = NULL;
			nTypeNum++;
		}
		m_pRidePlusList->Add(pData);
	}

	pIni->Free();
	return false;
}

ItemPlusConfig::ItemPlusConfig()
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<item_plus_type_MAX;j++)
		{
			m_pData[i][j] = NULL;
		}
	}
	m_pRidePlusList = xnList::Create();
}

ItemPlusConfig::~ItemPlusConfig()
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<item_plus_type_MAX;j++)
		{
			if (m_pData[i][j]!=NULL)
			{
				delete m_pData[i][j];
				m_pData[i][j] = NULL;
			}
		}
	}
	while (m_pRidePlusList->Count)
	{
		RidePlusData * pData = (RidePlusData *)m_pRidePlusList->Delete(0);
		delete pData;
	}
	m_pRidePlusList->Free();
}

RidePlusData * ItemPlusConfig::GetRidePlusData( int nID )
{
	for (int i=0;i<m_pRidePlusList->Count;i++)
	{
		RidePlusData * pData = (RidePlusData *)m_pRidePlusList->Items[i];
		if (pData->nID == nID)
		{
			return pData;
		}
	}
	return NULL;
}

bool ItemReputeConfig::load(const char * filename){
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;


	char szText[20]={0};

	pIni->SetSection("EquipID");
	ReputeItemCount=atoi(pIni->ReadString("count"));
	ReputeItemIDList=new int*[ReputeItemCount];
	for (int i=0;i<ReputeItemCount;i++)
	{
		ReputeItemIDList[i]=new int[5];
		for(int ii=0;ii<5;ii++){
			sprintf(szText,"item%d_%d",i+1,ii);
			ReputeItemIDList[i][ii]=atoi(pIni->ReadString(szText,"0"));
			int sadf=2134;
		}

	}
	pIni->SetSection("NeedVal");
	
	for (int i1=0;i1<item_plus_type_MAX;i1++)
	{
		for (int i2=0;i2<MAX_REPUTE_LV;i2++)
		{
			sprintf(szText,"val%d%d",i2,i1);
			m_pNeedVal[i1][i2]=atoi(pIni->ReadString(szText,"0"));
		}
	}
	pIni->SetSection("NeedRepute");
	for (int i1=0;i1<item_plus_type_MAX;i1++)
	{
		for (int i2=0;i2<MAX_REPUTE_LV;i2++)
		{
			sprintf(szText,"Repute%d%d",i2,i1);
			m_pNeedRepute[i1][i2]=atoi(pIni->ReadString(szText,"0"));
		}
	}
	

}
int ItemReputeConfig::isReputeEquip(WORD itemID){
	for (int i=0;i<ReputeItemCount;i++)
	{
		for (int j=0;j<5;j++)
		{
			if (ReputeItemIDList[i][j]==itemID)
			{
				return i;
			}
		}
	}
	return -1;
}

ItemReputeConfig::ItemReputeConfig(){
	
}

ItemReputeConfig::~ItemReputeConfig(){
	for (int i=0;i<ReputeItemCount;i++)
	{
		delete [] ReputeItemIDList[i];
	}
	
}

bool UpgradeStoneUpgradeConfig::load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;


	char szText[20]={0};

	pIni->SetSection("itemID");

	for (int i=0;i<19;i++)
	{
		sprintf(szText,"item%d",i+1);
		itemid[i]=atoi(pIni->ReadString(szText,"0"));
	}

	for (int i=0;i<19;i++)
	{
		sprintf(szText,"level%d",i+1);
		pIni->SetSection(szText);
		itemexp[i]=atoi(pIni->ReadString("needexp","0"));
		minval[i]=atoi(pIni->ReadString("minval","0"));
		maxval[i]=atoi(pIni->ReadString("maxval","0"));
	}
	pIni->Free();
}

int UpgradeStoneUpgradeConfig::isUpgradeStone( DWORD itemID )
{
	for (int i=0;i<19;i++)
	{
		if (itemID ==itemid[i])
		{
			return i+1;
		}
	}
	return 0;
}

bool ItemReplaceConfig::load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;
	pIni->SetSection("other");
	sprintf(txt,"%s",pIni->ReadString("txt",""));
	char * txttmp = strstr(txt,"$");
	while (txttmp!=NULL)//»»ÐÐ·û
	{
		*txttmp = 10;
		txttmp = strstr(txt,"$");
	}
	int count = pIni->sectionCount-1;
//	pItemReplaceInfo = new ItemReplaceInfo[count];

	char sztxt[128];
	char sztxt2[128];
	for (int i=0;i<count;i++)
	{
		pIni->SetSection(i+1);
		int infotype = atoi(pIni->ReadString("type","0"));
		switch(infotype)
		{
		case 0:
			{
				ItemReplaceInfo* pItemReplace = new ItemReplaceInfo;
				pItemReplace->GetCount = atoi(pIni->ReadString("itemcount2","0"));
				pItemReplace->NeedCount = atoi(pIni->ReadString("itemcount1","0"));
				pItemReplace->GetItem = new ReplaceItem[pItemReplace->GetCount];
				pItemReplace->NeedItem = new ReplaceItem[pItemReplace->NeedCount];
				sprintf(sztxt,"%s",pIni->ReadString("item2","0"));
				for (int i1=0;i1<pItemReplace->GetCount;i1++)
				{
					if (i1==pItemReplace->GetCount-1)
					{
						sscanf(sztxt,"%d:%d",&pItemReplace->GetItem[i1].itemID,&pItemReplace->GetItem[i1].itemNum);
					}else{
						sscanf(sztxt,"%d:%d,%s",&pItemReplace->GetItem[i1].itemID,&pItemReplace->GetItem[i1].itemNum,sztxt2);
						sprintf(sztxt,"%s",sztxt2);
					}
				}

				sprintf(sztxt,"%s",pIni->ReadString("item1","0"));
				for (int i1=0;i1<pItemReplace->NeedCount;i1++)
				{
					if (i1==pItemReplace->NeedCount-1)
					{
						sscanf(sztxt,"%d:%d",&pItemReplace->NeedItem[i1].itemID,&pItemReplace->NeedItem[i1].itemNum);
					}else{
						sscanf(sztxt,"%d:%d,%s",&pItemReplace->NeedItem[i1].itemID,&pItemReplace->NeedItem[i1].itemNum,sztxt2);
						sprintf(sztxt,"%s",sztxt2);
					}
				}
				pItemReplaceInfo->Add(pItemReplace);
			}
			break;
		case 1:
			{
				int a_GetCount = atoi(pIni->ReadString("itemcount2","0"));
				int a_NeedCount = atoi(pIni->ReadString("itemcount1","0"));
				char sztxtGet_1[512];
				char sztxtNeed_1[512];
				sprintf(sztxtGet_1,"%s",pIni->ReadString("item2","0"));
				sprintf(sztxtNeed_1,"%s",pIni->ReadString("item1","0"));
				int * getID = new int[a_GetCount];
				int * getNum = new int[a_GetCount];
				int * needID = new int[a_NeedCount];
				int * needNum = new int[a_NeedCount];
				char * sztmp = sztxtGet_1;
				for (int j=0;j<a_GetCount;j++)
				{
					sscanf(sztmp,"%d:%d",&getID[j],&getNum[j]);
					sztmp = strstr(sztmp,",");
					sztmp++;
				}
				sztmp = sztxtNeed_1;
				for (int j=0;j<a_NeedCount;j++)
				{
					sscanf(sztmp,"%d:%d",&needID[j],&needNum[j]);
					sztmp = strstr(sztmp,",");
					sztmp++;
				}
				for (int i1=0;i1<a_GetCount;i1++)
				{
					for (int i2=0;i2<a_NeedCount;i2++)
					{
						if (getID[i1]==needID[i2])
						{
							continue;
						}
						ItemReplaceInfo* pItemReplace = new ItemReplaceInfo;
						pItemReplace->GetCount = 1;
						pItemReplace->GetItem = new ReplaceItem;
						pItemReplace->GetItem->itemID = getID[i1];
						pItemReplace->GetItem->itemNum = getNum[i1];
						pItemReplace->NeedCount = 1;
						pItemReplace->NeedItem = new ReplaceItem;
						pItemReplace->NeedItem->itemID = needID[i2];
						pItemReplace->NeedItem->itemNum = needNum[i2];
						pItemReplaceInfo->Add(pItemReplace);
					}
				}
				free(getID);
				free(getNum);
				free(needID);
				free(needNum);
			}
			break;
		}

		
	}
	pIni->Free();
	return true;
}

ItemReplaceConfig::~ItemReplaceConfig()
{
	//delete	[] pItemReplaceInfo;
	while (pItemReplaceInfo->Count)
	{
		delete (ItemReplaceInfo*)pItemReplaceInfo->Delete(0);
	}
	pItemReplaceInfo->Free();
}

ItemReplaceInfo * ItemReplaceConfig::GetReplaceInfo( DWORD site )
{
	if (site>=pItemReplaceInfo->Count||site<0)
	{
		return NULL;
	}
	return (ItemReplaceInfo *)pItemReplaceInfo->Items[site];
}

ItemReplaceConfig::ItemReplaceConfig()
{
	pItemReplaceInfo = xnList::Create();
}
ItemReplaceInfo::~ItemReplaceInfo()
{
	if (NeedCount)
	{
		delete	[] NeedItem;
	}
	if (GetCount)
	{
		delete	[] GetItem;
	}
}

ItemReplaceInfo::ItemReplaceInfo()
{
	NeedCount = GetCount =0;
	NeedItem = GetItem = NULL;
}