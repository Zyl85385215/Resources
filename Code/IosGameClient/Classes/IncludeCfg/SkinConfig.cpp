#include "SkinConfig.h"
#include "xnExtstr.h"

CreateSingleton(CfgObjSkin);


CfgObjSkin::CfgObjSkin()
{
	m_pList		= xnList::Create ();
}

CfgObjSkin::~CfgObjSkin()
{
	while(m_pList->Count)
	{
		ObjSkinInfo*	pSkin	= (ObjSkinInfo*)m_pList->Delete (0);
		delete[]		pSkin->pnVal;
		free(pSkin->szSoundEx);
		delete	pSkin;
	}
	m_pList->Free ();
}

void CfgObjSkin::InitFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);

	int nIndex=0;
	while(pStr->SetSection (nIndex++))
	{
		ObjSkinInfo*	pSkin	= new ObjSkinInfo();
		m_pList->Add (pSkin);

		pSkin->nId	= atoi (pStr->sectionName);
		pSkin->nRealID	= atoi(pStr->ReadString("realid", "0"));
		if(pSkin->nRealID == 0)
			pSkin->nRealID	= pSkin->nId;
		pSkin->nXPer	= atoi(pStr->ReadString("cx", "0"));
		pSkin->nYPer	= atoi(pStr->ReadString("cy", "0"));
		pSkin->nTitleH	= atoi(pStr->ReadString("titleH", "140"));
		pSkin->bPartCnt	= atoi(pStr->ReadString("partcnt", "1"));
		pSkin->bRectCnt		= atoi(pStr->ReadString("rctcnt", "1"));
		pSkin->nHead		= atoi (pStr->ReadString ("head","-1"));
		pSkin->bTotalFrm		= atoi (pStr->ReadString ("tfrm","8"));
		pSkin->nScale		= atoi (pStr->ReadString ("scale","100"));
		pSkin->nShadowSize		= atoi (pStr->ReadString ("shadow","100"));
		pSkin->bExPart			= atoi (pStr->ReadString ("expart","0"));
		pSkin->szSoundEx			= strdup(pStr->ReadString ("sound",""));
		char	szTmp[64];
		pSkin->pnVal	= new int[pSkin->bRectCnt*4];
		for (int i = 0; i < pSkin->bRectCnt; i++)
		{
			sprintf(szTmp,"left%d",i+1);
			pSkin->pnVal[i*4]	= atoi (pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"top%d",i+1);
			pSkin->pnVal[i*4+1]	= atoi (pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"width%d",i+1);
			pSkin->pnVal[i*4+2]	= atoi (pStr->ReadString(szTmp,"1"))+pSkin->pnVal[i*4];
			sprintf(szTmp,"height%d",i+1);
			pSkin->pnVal[i*4+3]	= atoi (pStr->ReadString(szTmp,"1"))+pSkin->pnVal[i*4+1];
		}
	}
	pStr->Free();


}

ObjSkinInfo* CfgObjSkin::GetObjSkin( int nID )
{
	//if(nID /1000 == 1)
	//	nID	= 1004;
	for (int i = 0; i < m_pList->Count; i++)
	{
		ObjSkinInfo*	pSkin	= (ObjSkinInfo*)m_pList->Items[i];

		if(pSkin->nId	== nID)
			return pSkin;
	}
	return (ObjSkinInfo*)m_pList->Items[0];	//Ä¬ÈÏÆ¤·ô
}