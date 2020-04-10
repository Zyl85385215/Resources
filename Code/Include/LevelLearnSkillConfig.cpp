#include "LevelLearnSkillConfig.h"
#include "xnExtstr.h"
#include "Skill.h"
#include "GameWorld.h"

extern GameWorld* pWorld;

CreateSingleton(CLevelLearnSkillConfig);

LevelLearnSkillData * CLevelLearnSkillConfig::GetDataOnLevel(int nLevel,BYTE bJob)
{
	if (bJob==0)
		return NULL;
	for (int i=0;i<m_pList[bJob-1]->Count;i++)
	{
		LevelLearnSkillData * pData = (LevelLearnSkillData *)m_pList[bJob-1]->Items[i];
		if (pData->nLevel==nLevel)
		{
			return pData;
		}
	}
	return NULL;
}

static int SkillCompare(const void * Item1, const void * Item2)
{
	int a1 = *(int*)Item1;
	int a2 = *(int*)Item2;
	if (a1<a2)
		return -1;
	if (a1>a2)
		return 1;
	if (a1==a2)
		return 0;
}

bool CLevelLearnSkillConfig::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);

		LevelLearnSkillData * pData = new LevelLearnSkillData;
		int nTempInt = atoi(pIni->sectionName);
		int nJobIndex = nTempInt/1000;
		pData->nLevel = nTempInt - nJobIndex*1000;
		pData->dwSkillId[0] = atoi(pIni->ReadString("Skill1","0"));
		pData->dwSkillId[1] = atoi(pIni->ReadString("Skill2","0"));
		pData->dwSkillId[2] = atoi(pIni->ReadString("Skill3","0"));

		m_pList[nJobIndex-1]->Add(pData);
	}
	pIni->Free();



	FILE * pFile = fopen("LevelSkill.txt","w");
	for (int cao=0;cao<4;cao++)
	{
		/////////////////////////////////////////////////////////////
		xnList * dataList[100];
		for (int i=0;i<100;i++)
		{
			dataList[i] = xnList::Create();
		}
		for (int i=0;i<m_pList[cao]->Count;i++)
		{
			if (i-1>=0)
			{
				for (int j=0;j<dataList[i-1]->Count;j++)
				{
					dataList[i]->Add(dataList[i-1]->Items[j]);
				}
			}
			LevelLearnSkillData * pData = (LevelLearnSkillData *)m_pList[cao]->Items[i];
			for (int j=0;j<3;j++)
			{
				if(pData->dwSkillId[j])
				{
					for (int k=0;k<dataList[i]->Count;k++)
					{
						SkillInfo * pInfo = pWorld->pSkillBase->GetSkillBase(pData->dwSkillId[j]);
						if(pInfo && pInfo->wNeedSkill==(int)dataList[i]->Items[k])
						{
							dataList[i]->Delete(k);
						}
					}
					dataList[i]->Add((void *)pData->dwSkillId[j]);
				}
			}
		}
		for (int i=0;i<101;i++)
		{
			fprintf(pFile,"[%d]\n",i+(cao+1)*1000);
			xnList * pTList = xnList::Create();
			for (int j=m_pList[cao]->Count-1;j>=0;j--)
			{
				LevelLearnSkillData * pTempData = (LevelLearnSkillData *)m_pList[cao]->Items[j];
				if (i>=pTempData->nLevel)
				{
					for (int k=0;k<dataList[j]->Count;k++)
					{
						fprintf(pFile,"skill%d=%d\n",k,(int)dataList[j]->Items[k]);
						pTList->Add(dataList[j]->Items[k]);
					}
					break;
				}
			}
			pTList->Sort(SkillCompare);
			m_pFullList[cao]->Add(pTList);
		}
		for (int i=0;i<100;i++)
		{
			dataList[i]->Free();
		}
		//////////////////////////////////////////////////////////////
	}
	fclose(pFile);

	return false;
}

CLevelLearnSkillConfig::CLevelLearnSkillConfig( void )
{
	for (int i=0;i<4;i++)
	{
		m_pList[i] = xnList::Create();
		m_pFullList[i] = xnList::Create();
	}
}

CLevelLearnSkillConfig::~CLevelLearnSkillConfig( void )
{
	for (int j=0;j<4;j++)
	{
		int nCnt = m_pList[j]->Count;
		for(int i=0; i<nCnt; i++)
		{
			LevelLearnSkillData * pConfig = (LevelLearnSkillData*)m_pList[j]->Items[i];
			SAFE_DELETE(pConfig);
		}
		m_pList[j]->Free();
		for (int i=0;i<m_pFullList[j]->Count;i++)
		{
			xnList * pList = (xnList *)m_pFullList[j]->Items[i];
			if (pList)
			{
				pList->Free();
			}
		}
		m_pFullList[j]->Free();
	}
}