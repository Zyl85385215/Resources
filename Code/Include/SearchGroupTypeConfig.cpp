#include "SearchGroupTypeConfig.h"
#include "xnExtstr.h"

CreateSingleton(SearchGroupTypeConfig);

SearchGroupTypeConfig::SearchGroupTypeConfig()
{
	m_pList = xnList::Create();
}

SearchGroupTypeConfig::~SearchGroupTypeConfig()
{
	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		char * pConfig = (char*)m_pList->Items[i];
		free(pConfig);
	}
	m_pList->Free();
}

bool SearchGroupTypeConfig::Load(const char * filename)
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);
		char * pData = strdup(pIni->ReadString("name", "нч"));

		m_pList->Add(pData);
	}
	pIni->Free();
	return false;
}

char * SearchGroupTypeConfig::GetTxt( int nPos )
{
	if (nPos<=m_pList->Count)
	{
		char * szText =(char *)m_pList->Items[nPos];
		return szText;
	}
	return NULL;
}