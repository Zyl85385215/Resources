#pragma once

#include "xnExtstr.h"
#include "xnlist.h"
#include "cocos2d.h"
#include "GlobalFunction.h"
#include "GlobalDefine.h"

using namespace cocos2d;

struct StrData {
	char * key;
	char * data;
};

struct gStrData {
	xnList * strList;
	char * groupName;
};

struct xnExtstrData:xnExtstr {
	int		bufSize;
	char * strBuf;
	xnList * groupLink;
	gStrData * section;
};

static int sectionCompare( const void *arg1, const void *arg2 )
{
	gStrData * p1, *p2;
	p1 = *(gStrData **)arg1;
	p2 = *(gStrData **)arg2;
   /* Compare all of both strings: */
   return strcmp(p1->groupName, p2->groupName);
}

static StrData * ReadStrData(char * data)
{
	StrData * k = (StrData *)malloc(sizeof(StrData));
	k->key = data;
	char * p = data;

	int b;
	while (((b = _mbsnextc((char *)p)) != '.') && (b != '='))
	{
		if ((b >> 8) > 0)
			p+=2;
		else
			p++;
	}
	p++;
	while (*p == ' ')
		p++;
	k->data = p;
	return k;
}

static void FixKey(StrData * pk)
{
	char * p = pk->key;
	int b;
	while (((b = _mbsnextc((char *)p)) != '.') && (b != '='))
	{
		if ((b >> 8) > 0)
			p+=2;
		else
			p++;
	}
    *p = '\0';
    p--;
	while (*p == ' ')
    {
        *p = '\0';
		p--;
    }
}

xnExtstr * xnExtstr::ImportFromMemory(void * buffer, int bufSize)
{
	char *strBuf = (char*)buffer;
	xnExtstrData * pExtStr = (xnExtstrData *)malloc(sizeof(xnExtstrData));
	pExtStr->strBuf = strBuf;
	pExtStr->bufSize = bufSize;
	pExtStr->section = NULL;
	pExtStr->sectionCount = 0;
	for (int i=0; i<bufSize; i++)
	{
		if (((BYTE)strBuf[i] == 0x0D) || ((BYTE)strBuf[i] == 0x0A))
			strBuf[i] = 0;
	}
	pExtStr->groupLink = xnList::Create();
	char *p = strBuf;
	gStrData * gp = NULL;
	while (p < strBuf + bufSize)
	{
		// 清除行首空格
		while(*p  == ' ') p++;
		// 处理行
		if(*p == '#' || *p == 0)
			; // 注释或者行注释
		else if (*p == '[')
		{
			gp = (gStrData*)malloc(sizeof(gStrData));
			pExtStr->groupLink->Add(gp);
			gp->strList = xnList::Create();
			gp->groupName = p+1;
			int b;
			while ((b = _mbsnextc((char *)p)) != ']')
			{
				if ((b >> 8) > 0)
					p+=2;
				else
					p++;
			}
			*p = 0;	
			p++;
		}
		else
		{
			if (gp != NULL)
			{
				StrData * kkk = ReadStrData(p);
				FixKey(kkk);
				gp->strList->Add(kkk);
				p = kkk->data;
			}
		}
		// 下一行
		while ((p < strBuf + bufSize))
		{	
			// 碰到行尾了
			if(*p == 0) {
				p++;
				break;
			}
			p++;
		}
	}
	pExtStr->sectionCount = pExtStr->groupLink->Count;
	pExtStr->SetSection(0);
	return pExtStr;
}

xnExtstr * xnExtstr::Import(const char * fileName)
{
	DWORD bufSize;
	char * szTemp = (char *)CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName),"rb", &bufSize);
	if (szTemp == NULL)
		return NULL;

	//WCHAR* wszStr = (WCHAR*)szStrBuf;
	//if(*wszStr == 0xFEFF)
	//{
	//	int nCharLen = (bufSize>>1)-1;
	//	szTemp		 = (char*)wszStr;
	//	szStrBuf = (char *)malloc(bufSize+2);
	//	memcpy(szStrBuf, szTemp, bufSize);
	//	szStrBuf[bufSize]	= 0;
	//	szStrBuf[bufSize+1]	= 0;

	//	wszStr = (WCHAR*)szStrBuf;		// 新建内存

	//	szStrBuf = (char *)malloc(bufSize+2);
	//	memset(szStrBuf, 0, bufSize+2);

	//	BOOL bValue;
	//	WideCharToMultiByte(936, 0, wszStr+1, nCharLen, szStrBuf, bufSize+2, "?", &bValue);

	//	bufSize = strlen(szStrBuf);
	//	free(wszStr);
	//}
	//else
	//{
		//char * szStrBuf = (char *)malloc(bufSize/*+2*/);
		//memcpy(szStrBuf,szTemp,bufSize);
		//szStrBuf[bufSize]	= 0;
		//szStrBuf[bufSize+1]	= 0;	
	//}

	char* szStrBuf = (char *)malloc(bufSize+1);
	memcpy(szStrBuf,szTemp,bufSize);
	szStrBuf[bufSize]	= 0;
	free(szTemp);
	return xnExtstr::ImportFromMemory(szStrBuf, bufSize+1);
}

void xnExtstr::Free(void)
{
	if (this == NULL)
		return;
	xnExtstrData * pExtStr = (xnExtstrData *)this;
	while (pExtStr->groupLink->Count > 0)
	{
		gStrData * gp = (gStrData *)pExtStr->groupLink->Items[0];
		for (int i=0; i<gp->strList->Count; i++)
			free(gp->strList->Items[i]);
		gp->strList->Free();
		free(gp);
		pExtStr->groupLink->Delete(0);
	}
	pExtStr->groupLink->Free();
	//free(pExtStr->strBuf);
	free(pExtStr);
}

static int sectionFind( const void *arg1, const void *arg2 )
{
	char * p1;
	gStrData * p2;
	p1 = (char *)arg1;
	p2 = *(gStrData **)arg2;
   /* Compare all of both strings: */
   return strcmp(p1, p2->groupName);
}

bool xnExtstr::SetSection(char * section)
{
	if (this == NULL)
		return FALSE;
	xnExtstrData * pExtStr = (xnExtstrData *)this;

//	gStrData ** ppg = (gStrData **)(lfind(section, pExtStr->groupLink->Items, (unsigned int *)(&pExtStr->groupLink->Count), sizeof(void *), sectionFind));
	for (int i = 0; i<pExtStr->groupLink->Count; i++)
	{
		gStrData * ppg = (gStrData*)pExtStr->groupLink->Items[i];
		if (strcmp(ppg->groupName, section) == 0)
		{
			pExtStr->section = ppg;
			pExtStr->sectionIndex = i;
			pExtStr->keyCount = pExtStr->section->strList->Count;
			pExtStr->sectionName = pExtStr->section->groupName;
			return TRUE;
		}
	}
	pExtStr->section = NULL;
	pExtStr->sectionIndex = -1;
	pExtStr->keyCount = -1;
	pExtStr->sectionName = NULL;
	return FALSE;
}

bool xnExtstr::SetSection(int sectionIndex)
{
	if (this == NULL)
		return FALSE;
	xnExtstrData * pExtStr = (xnExtstrData *)this;
	if (sectionIndex < pExtStr->groupLink->Count)
	{
		pExtStr->section = (gStrData *)pExtStr->groupLink->Items[sectionIndex];
		pExtStr->sectionIndex = sectionIndex;
		pExtStr->keyCount = pExtStr->section->strList->Count;
		pExtStr->sectionName = pExtStr->section->groupName;
	}
	else
	{
		pExtStr->section = NULL;
		pExtStr->sectionIndex = -1;
		pExtStr->keyCount = -1;
		pExtStr->sectionName = NULL;
	}
	return (pExtStr->section != NULL);
}

char *	xnExtstr::ReadString(char * key, char * defaultResult)
{
	if (this == NULL)
		return defaultResult;
	xnExtstrData * pExtStr = (xnExtstrData *)this;
	if (pExtStr->section == NULL)
		return defaultResult;
	gStrData * pg = pExtStr->section;
	for (int i=0; i<pg->strList->Count; i++)
	{
		StrData * k = (StrData *)pg->strList->Items[i];
		if (strcmp(k->key, key) == 0)
			return k->data;
	}
	return defaultResult;
}

char *	xnExtstr::ReadString(int index, char * defaultResult)
{
	if (this == NULL)
		return defaultResult;
	xnExtstrData * pExtStr = (xnExtstrData *)this;
	if (pExtStr->section == NULL)
		return defaultResult;
	gStrData * pg = pExtStr->section;
	if (index < pg->strList->Count)
	{
		StrData * k = (StrData *)pg->strList->Items[index];
		return k->data;		
	}
	return defaultResult;
}

char *  xnExtstr::GetKey(int index, char * defaultResult)
{
	if (this == NULL)
		return defaultResult;
	xnExtstrData * pExtStr = (xnExtstrData *)this;
	if (pExtStr->section == NULL)
		return defaultResult;
	gStrData * pg = pExtStr->section;
	if (index < pg->strList->Count)
	{
		StrData * k = (StrData *)pg->strList->Items[index];
		return k->key;
	}
	return defaultResult;
}

void StringSplit(std::string s,char* szC,std::vector<std::string>& vec)
{
	if(vec.size() > 0)
		vec.clear();
	int nStart = 0;
	int nLen = 0;
	nLen = s.find(szC,nStart);
	while(nLen != std::string::npos)
	{
		vec.push_back(s.substr(nStart,nLen-nStart));
		nStart = nLen+strlen(szC);
		nLen = s.find(szC,nStart);
	}
	nLen = s.size();
	int n = s.length();
	vec.push_back(s.substr(nStart,nLen-nStart));
}

void StringReplace(std::string& s,char C1,char C2)
{
	int nStart = 0;
	int nLen = 0;
	nLen = s.find(C1,nStart);
	while(nLen != std::string::npos)
	{
		s[nLen] = C2;
		nStart = nLen+1;
		nLen = s.find(C1,nStart);
	}
}

void FixDescString(char* pMsg)
{
	int nLen = strlen(pMsg);
	char* p = pMsg;
	while(*p != '\0')
	{
		if(*p == '/' && *(p+1) == '/')
		{
			*p = '\0';
		}
		else if(*p == ' ' || *p == '\t')
		{
			strcpy(p,p+1);
			//*(p+nLen) = '\0';
			//nLen--;
		}
		else
			p++;
	}
}