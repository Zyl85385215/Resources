#include "stdafx.h"
#include "TestLibraryCfg.h"
#include "xnExtstr.h"
#include <stdio.h>

CreateSingleton(TestLibraryCfg);

void FixWords(char* szStr)
{
	if(szStr == NULL || strlen(szStr) == 0)
		return;
	std::string strWords(szStr);
	std::vector<std::string> vec;
	StringSplit(strWords,"\n",vec);
	int nLen = 0;
	for(int i = 0; i < vec.size(); i ++)
	{
		strcpy(szStr+nLen,vec[i].c_str());
		nLen = strlen(szStr);
		if(i == vec.size()-1)
			*(szStr+nLen) = '\0';
		else
			*(szStr+nLen) = '\n';
		nLen ++;
	}
}

TestLibraryCfg::TestLibraryCfg()
{
	m_pList = xnList::Create();
}

TestLibraryCfg::~TestLibraryCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (TestObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void TestLibraryCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex ++))
	{
		TestObj* pObj = new TestObj();
		pObj->nID = atoi(pStr->sectionName);
		pObj->pQuestion = strdup(pStr->ReadString("question","问题"));
		FixWords(pObj->pQuestion);
		pObj->pAnswer = strdup(pStr->ReadString("answer","选项"));
		FixWords(pObj->pAnswer);
		char szName[16];
		for(int i = 0; i < 3; i ++)
		{
			sprintf(szName,"choosen%d",i+1);
			pObj->pChoosen[i] = strdup(pStr->ReadString(szName,"选项"));
			FixWords(pObj->pChoosen[i]);
		}
		pObj->nStar = atoi(pStr->ReadString("star","1"));
		m_pList->Add((void*)pObj);
	}
	pStr->Free();
}

TestObj* TestLibraryCfg::GetTest(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		TestObj* pObj = (TestObj*)m_pList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}

TestObj* TestLibraryCfg::RandTest()
{
	if(m_pList->Count == 0)
		return NULL;
	int nPos = rand()%m_pList->Count;
	return (TestObj*)m_pList->Items[nPos];
}