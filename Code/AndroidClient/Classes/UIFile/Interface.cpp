#include "GameScene.h"
#include "Interface.h"
#include "xnExtstr.h"
#include "CtrlObj.h"
#include "InterfaceLayer.h"
#include "SoundCenter.h"
#include "MainMenu.h"
#include "GuideCenter.h"
#include "FaceConfig.h"


DInterface*	pMainInterface	= NULL;
DToopTipCenter*	pMainToopTip	= NULL;
xnList*		g_pListFPartCache	= NULL;

void DefaulEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->IBtnDefaulEvent(pEvent->pComponent->nMaxChar);

}

DComponent::DComponent( char* szN ):m_nOrder(0),m_bDraged(false),m_pTag(0),m_bChgPosbySpr(false)
{
	szName		= strdup(szN);
	szSpr		= NULL;
	szSelectSpr	= NULL;
	szCaption	= NULL;
	szCAPlist	= NULL;
	szCAFile	= NULL;
	nCATime		= 1000;
	nCTScale	= 100;
	pData		= NULL;
	nCaptionAlignment	= 0;
	m_nMark		= 0;
	bSound		= true;
	byInputType = INPUT_TYPE_NULL;

	eventFunc	= NULL;
	pParam		= NULL;
	pCbParam	= NULL;
	fnSetEditTxt	= NULL;
	fnGetEditTxt	= NULL;
	fnProgressSetInfo	= NULL;
	fnSetViewSpr	= NULL;
	fnSetViewSprByScale = NULL;
	fnSetSprColor	= NULL;
	fnSetCartoon	= NULL;
	fnSetSelectSpr	= NULL;
	fnSetShader		= NULL;
	fnSetTouchMoveSpr	= NULL;
	eventActionCallback = NULL;
	fnSetSprRotation	= NULL;
	fnSetScale = NULL;
	fnSetOpacity = NULL;
	m_pActionParam = NULL;
	szEdit		= NULL;
	nProVal		= 100;
	nProMax		= 100;
	bVisable	= true;
	bEnable		= true;
	bLocked		= false;
	bClickLocked= false;
	bEffChg		= false;
	byChgScaleY	= 0;
	bChgPos		= false;
	nCPTime		= 0;
	nR			= 255;
	nG			= 255;
	nB			= 255;
	m_nFontSize	= DEFAULT_FONT_SIZE;

	pSTextCenter	= NULL;
	pSpecialDataEx	= NULL;
	pListEff		= xnList::Create ();
	pListEffNum		= xnList::Create ();
	//wEffect[0]		= 0;
	//wEffect[1]		= 0;
	//wEffect[2]		= 0;

	byRunAction		= 0;

	bAlignX			= 1;
	bAlignY			= 1;

	m_pListASprTag	= xnList::Create ();
	fnAddSprByTag	= NULL;

	m_pNodeBatch	= NULL;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
}

DComponent::~DComponent()
{
	while(m_pListASprTag->Count)
	{
		_CDataAddSprTag* pSprT	= (_CDataAddSprTag*)m_pListASprTag->Delete (0);
		free(pSprT->szSpr);
		delete pSprT;

	}
	m_pListASprTag->Free ();

	while(pListEff->Count)
	{
		DCompEffect* pDE	= (DCompEffect*)pListEff->Delete (0);
		delete pDE;

	}
	pListEff->Free ();

	while(pListEffNum->Count)
	{
		DCompNumEff* pDE	= (DCompNumEff*)pListEffNum->Delete (0);
		delete pDE;

	}
	pListEffNum->Free ();

	SAFE_FREE(szName);
	SAFE_FREE(szSpr);
	SAFE_FREE(szSelectSpr);
	SAFE_FREE(szCaption);
	SAFE_DELETE(pSTextCenter);
}

void DComponent::SetEvent( void (*EventFunc)(InterfaceEvent * pEvent,void * param), void * para /*= NULL*/ )
{
	if(this == NULL)
		return;
	eventFunc	= EventFunc;
	pCbParam	= para;
}

char* DComponent::GetEdit()
{
	if(fnGetEditTxt)
		return (char*)fnGetEditTxt (pParam);
	else if(szEdit)
		return szEdit;
	else
		return "";
}

void DComponent::SetEdit( char* szTxt )
{
	if(this == NULL)
		return;
	if(szEdit)
		free(szEdit);
	szEdit	= strdup(szTxt);

	if(fnSetEditTxt) 
		fnSetEditTxt (szEdit,0,pParam);
}

void DComponent::SetProgress( int nVal, int nMax,char* szTxt/*=""*/ )
{
	if(this == NULL)
		return;
	nProMax	= nMax;
	nProVal	= nVal;
	if (szTxt)
	{
		if(szCaption)
			free(szCaption);
		szCaption	= strdup(szTxt);
	}
	if(fnProgressSetInfo)
		fnProgressSetInfo (nVal,nMax,szCaption,pParam);
}

void DComponent::SetCaption(const char* szTxt,int nAlignment )
{
	if (!this)
		return;
	if(szTxt == NULL)
		return;
	if(szCaption)
	{
		if(strcmp (szTxt,szCaption) == 0)
			return;
		free(szCaption);
	}
	szCaption	= strdup(szTxt);
	nCaptionAlignment	= nAlignment;
	if(fnSetEditTxt)
		fnSetEditTxt (szCaption,nCaptionAlignment,pParam);
}

void DComponent::SetCaptionEx(const char* szFormat,int nAlignment,... )
{
	char msg[1024];
	va_list argptr;
	va_start(argptr,nAlignment);
	vsprintf(msg, szFormat, argptr);
	SetCaption(msg,nAlignment);
	va_end(argptr);
}

void DComponent::SetViewSpr(const char* szFile )
{
	if(this == NULL)
		return;

	if(szSpr)
	{
		if(strcmp(szFile,szSpr)==0)
			return;
		free(szSpr);
	}
	szSpr	= strdup(szFile);

	m_fScaleX = 1;
	m_fScaleY = 1;
	if(fnSetViewSpr)
		fnSetViewSpr ((char*)szFile,pParam);

	while(m_pListASprTag->Count)
	{
		_CDataAddSprTag* pSprT	= (_CDataAddSprTag*)m_pListASprTag->Delete(0);
		
		if(fnAddSprByTag)
			fnAddSprByTag("",0,0,pSprT->nTag,pParam,100,0,10);

		free(pSprT->szSpr);
		delete pSprT;
	}


	//if(strlen (szSpr) == 0)
	//{//清理光效
	//	if(szSelectSpr && strlen(szSelectSpr))
	//		SetSelectSpr ("",false);

	//	SetEffect (0,0);
	//	SetEffect (0,1);
	//	SetEffect (0,2);
	//}
}

void DComponent::SetSelectSpr( char* szFile,bool bTop )
{
	if(this == NULL)
		return;
	if(szSelectSpr)
	{
		if(strcmp(szFile,szSelectSpr)==0)
			return;
		free(szSelectSpr);
	}
	szSelectSpr	= strdup(szFile);
	bSelSprTop	= bTop;
	if(fnSetSelectSpr )
		fnSetSelectSpr (szFile,bTop,pParam);
}

void DComponent::SetCartoonAction( char * szPlist, char* szFilePre,int nNeedTime, int nScale,bool ifFlip )
{
	if(szCAPlist && szCAFile && strcmp (szPlist,szCAPlist) == 0
		&& strcmp (szFilePre,szCAFile) == 0)
		return;

	if(szCAPlist)
	{
		free(szCAPlist);
		free(szCAFile);
	}
	szCAPlist	= strdup(szPlist);
	szCAFile	= strdup(szFilePre);
	nCATime		= nNeedTime;
	nCTScale	= nScale;
	if(fnSetCartoon)
		fnSetCartoon (szPlist,szFilePre,nNeedTime,pParam,nScale,ifFlip);
}

bool DComponent::SetPos( int nPosX,int nPosY, int nMillSecond )
{
	if(this == NULL)
		return false;
	if(nPosX == nX && nPosY == nY)
		return false;

	bChgPos = true;
	nX = nPosX;
	nY = nPosY;
	nCPTime	= nMillSecond;
	return true;
}

void DComponent::SetSprColor( int npR,int npG,int npB )
{
	nR = npR;
	nG = npG;
	nB = npB;
	if (fnSetSprColor)
	{
		fnSetSprColor(npR,npG,npB,pParam);
	}
}

void DComponent::SetVisible( bool bVsb )
{
	if (!this)
		return;
	if(bVisable != bVsb)
	{
		bVisable	= bVsb;
		bChgPos		= true;
	}
	//if(bVsb)
	//{
	//	SetPos (nX,nY%10000);
	//}
	//else
	//	SetPos (nX,nY%10000+10000);
}


void DComponent::AddNumEffect( int nNum, BYTE bType, BYTE bVAct )
{
	DCompNumEff*	pObjDE	= new DCompNumEff();
	pObjDE->nNum	= nNum;
	pObjDE->bType	= bType;
	pObjDE->bVAct	= bVAct;
	pListEffNum->Add (pObjDE);
}

void DComponent::ClearEffect()
{
	while(pListEff->Count)
	{
		delete (DCompEffect*)pListEff->Delete (0);
	}
	bEffChg = true;
}

DCompEffect* DComponent::SetEffect( int nVal, int nPos )
{
	DCompEffect* pObjDE	= NULL;
	for (int i = 0; i < pListEff->Count; i++)
	{
		DCompEffect* pDE	= (DCompEffect*)pListEff->Items[i];
		if(pDE->nPos == nPos && nPos != -1)
			pObjDE	= pDE;
	}
	
	if(pObjDE && pObjDE->nEffId	== nVal)
		return pObjDE;


	bEffChg	= true;
	if(pObjDE)
		pObjDE->nEffId	= nVal;
	else
	{
		pObjDE	= new DCompEffect();
		pObjDE->nPos	= nPos;
		pObjDE->nEffId	= nVal;
		pListEff->Add (pObjDE);
	}
	return pObjDE;
}

void DComponent::SetEnable( bool bEnable1 )
{
	if (!this)
		return;
	if (bEnable!=bEnable1)
	{
		bEnable	= bEnable1;
		if(fnSetShader) fnSetShader(bEnable?SHADER_NULL:SHADER_GRAY,pParam);
	}
	
}

void DComponent::SetAddSprByTag( char* szFile,int nPosX, int nPosY,int nTag, int nScalc, int nRota ,int nZorder)
{
	bool bFind	= false;
	for (int i = 0; i < m_pListASprTag->Count; i++)
	{
		_CDataAddSprTag* pSprT	= (_CDataAddSprTag*)m_pListASprTag->Items[i];
		if(pSprT->nTag	== nTag)
		{
			if(strlen (szFile) == 0)
			{
				free(pSprT->szSpr);
				delete pSprT;
				m_pListASprTag->Delete (i);
				break;
			}
			else
			{
				if(strcmp(pSprT->szSpr,szFile) == 0)
				{
					if(pSprT->nPosX == nPosX && pSprT->nPosY == nPosY && pSprT->nScalc == nScalc && pSprT->nRota == nRota)
					{
						return;
					}
				}
				free(pSprT->szSpr);
				pSprT->szSpr	= strdup(szFile);
				pSprT->nPosX	= nPosX;
				pSprT->nPosY	= nPosY;
				pSprT->nScalc	= nScalc;
				pSprT->nRota	= nRota;
				bFind	= true;
				break;
			}
		}
	}
	if(!bFind && strlen (szFile))
	{
		_CDataAddSprTag* pSprT	= new _CDataAddSprTag();
		pSprT->szSpr	= strdup(szFile);
		pSprT->nPosX	= nPosX;
		pSprT->nPosY	= nPosY;
		pSprT->nTag		= nTag;
		pSprT->nZorder	= nZorder;
		pSprT->nScalc	= nScalc;
		pSprT->nRota	= nRota;
		m_pListASprTag->Add (pSprT);
	}
	if(fnAddSprByTag)
		fnAddSprByTag(szFile,nPosX,nPosY,nTag,pParam,nScalc,nRota,nZorder);
}

void DComponent::SetLock( bool bVal )
{
	if(this == NULL)
		return;
	if (bVal==bLocked)
		return;
	bLocked	= bVal;

	SetSprColor(nR,nG,nB);
}

int DComponent::GetAlignmentX()
{
	if(byType == COMPONENT_LINK)
		return nX;
	return	nX+nW*bAlignX/2;
}

int DComponent::GetAlignmentY()
{
	if(byType == COMPONENT_LINK)
		return nY;
	return	nY+nH*bAlignY/2;
}

DComponent* DComponent::BeCopy()
{
	DComponent*	pCopy	= new DComponent(this->szName);
	pCopy->pParent	= NULL;
	pCopy->byType	= this->byType;
	if (pCopy->byType == COMPONENT_EDIT)
	{
		pCopy->byInputType = INPUT_TYPE_ALL;
	}
	else if(pCopy->byType == COMPONENT_LINK)
		pCopy->pSTextCenter	= new SpecialTextCenter();

	if(this->szSpr)
		pCopy->szSpr	= strdup(this->szSpr);
	else
		pCopy->szSpr	= NULL;

	pCopy->szSelectSpr	= NULL;

	pCopy->nX	= this->nX;
	pCopy->nY	= this->nY;
	pCopy->nW	= this->nW;
	pCopy->nH	= this->nH;
	
	pCopy->nR	= this->nR;
	pCopy->nG	= this->nG;
	pCopy->nB	= this->nB;
	pCopy->nMaxChar	= this->nMaxChar;
	pCopy->pParent	= this->pParent;
	pCopy->bAutoChgLine	= this->bAutoChgLine;
	pCopy->bAlignX	= this->bAlignX;
	pCopy->bAlignY	= this->bAlignY;
	pCopy->m_nFontSize	= this->m_nFontSize;
	
	if(this->szCaption)
		pCopy->szCaption	= strdup(this->szCaption);
	else
		pCopy->szCaption	= NULL;

	return pCopy;
}

void DComponent::ActionCallBack( void* pParam )
{
	if (eventActionCallback)
	{
		eventActionCallback(pParam,m_pActionParam);
	}
	

}

void DComponent::SetActionCallback( void (*EventFunc)(void * param,void* pActionParam), void * para /*= NULL*/ )
{
	if(this == NULL)
		return;
	eventActionCallback	= EventFunc;
	m_pActionParam = para;
}

void DComponent::SetViewSprByScale( char* szFile,float scalex,float scaley )
{
	if(this == NULL)
		return;
	m_fScaleX = scalex;
	m_fScaleY = scaley;
	if(szSpr)
	{
		if(strcmp(szFile,szSpr)==0)
			return;
		free(szSpr);
	}
	szSpr	= strdup(szFile);
	if(fnSetViewSprByScale)
		fnSetViewSprByScale (szFile,pParam,scalex,scaley);
}

void DComponent::SetTouchMoveSpr( char* szFilePrev,char* szFileNext )
{
	if(fnSetTouchMoveSpr)
		fnSetTouchMoveSpr(szFilePrev,szFileNext,pParam);
}

void DComponent::SetSprRotation( int nRotation )
{
	if(fnSetSprRotation)
		fnSetSprRotation(nRotation,pParam);
}

void DComponent::SetPosX( int nPosX, int nMillSecond/*=0*/ )
{
	if(this == NULL)
		return;
	if(nPosX == nX)
		return;

	bChgPos = true;
	nX = nPosX;
	nCPTime	= nMillSecond;
}

void DComponent::SetScale( float scalex,float scaley )
{
	if(this == NULL)
		return;
	m_fScaleX = scalex;
	m_fScaleY = scaley;

	if(fnSetScale)
		fnSetScale (pParam,scalex,scaley);
}

void DComponent::SetOpacity(BYTE byType)
{
	if(this == NULL)
		return;

	if(fnSetOpacity)
		fnSetOpacity (pParam,byType);
}

void DComponent::SetAlignmentX( int nPosX )
{
	if(byType == COMPONENT_LINK)
		nX = nPosX;
	else
		nX = nPosX-nW*bAlignX/2;
	bChgPos = true;
}

void DComponent::SetAlignmentY( int nPosY )
{
	if(byType == COMPONENT_LINK)
		nY = nPosY;
	else	nY = nPosY-nH*bAlignY/2;
	bChgPos = true;
}


DForm::DForm( char* szN ):nW(0),nH(0),pFormLayer(NULL)
{
	szName	= strdup(szN);
	szSpr	= NULL;
	bOpened	= false;
	bOpenStyle	= 1;
	pListComps	= xnList::Create ();
	bChgPos	= false;
	nCPTime		= 0;
	bUnTrhough	= false;
	bZorde		= FORM_ZORDER_WARCOPY;
	pCFrame		= NULL;
	bCocosVisabel	= false;
}

DForm::~DForm()
{
	SAFE_FREE(szName);
	SAFE_FREE(szSpr);
	SAFE_DELETE(pCFrame);

	while(pListComps->Count)
	{
		delete (DComponent*)pListComps->Delete (0);
	}
	pListComps->Free ();

}

void DForm::Open(BYTE bEffet)
{
	if(bOpened == false)
	{
		 ClientLuaManager::sharedHD()->FormOpen(this);
		bOpenStyle	= bEffet;
		bOpened	= true;
		pMainInterface->RequestOpen (this);

		//g_pSoundCenter->PlayerEffect ("Sound/form_open");
	}
}

DComponent* DForm::GetComponent( char* szNID )
{
	if(!this)
		return NULL;
	DComponent * pComp	= NULL;
	for (int i = 0; i < pListComps->Count; i++)
	{
		pComp	= (DComponent *)pListComps->Items[i];
		if(strcmp (pComp->szName,szNID) == 0)
			return pComp;
	}
	return NULL;
}

void DForm::Close()
{
	if(bOpened)
	{
		 ClientLuaManager::sharedHD()->FormClose(this);
		bOpened	= false;
		//g_pSoundCenter->PlayerEffect ("Sound/form_close");
	}
}

void DForm::SetPos( int nX, int nY, int nMillSecond )
{
	nPosX	= nX;
	nPosY	= nY;
	bChgPos	= true;
	nCPTime	= nMillSecond;
}

void VirCloseEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pEvent->pComponent->pParent->Close ();
}

DInterface::DInterface( char* szFile )
{
	m_pListForms	= xnList::Create ();
	m_pListRequest	= xnList::Create ();
	g_pListFPartCache	= xnList::Create();

	//bool bIsIphone5	= false;
	//if(nowScreenWidth == 1136)
	//{
	//	bIsIphone5	= true;
	//}

	xnExtstr * pStr = xnExtstr::Import(szFile);
	int i = 0;
	DForm * pForm = NULL;
	do {
		pStr->SetSection(i);
		i++;

		if (pStr->sectionName != NULL)
		{
			char * stringBuf;
			if (strncmp("INTERFACE_", pStr->sectionName, 10) == 0)//form
			{
				stringBuf = pStr->ReadString("sprName", NULL);
				pForm =	new DForm(&pStr->sectionName[10]);
				if(stringBuf)
					pForm->szSpr	= strdup(stringBuf);
				else
					pForm->szSpr	= NULL;


				sscanf(pStr->ReadString("Position", "0,0"), "%d,%d", &pForm->nX, &pForm->nY);
				sscanf(pStr->ReadString("PositionPer", "0,0"), "%d,%d", &pForm->nCX, &pForm->nCY);
				sscanf(pStr->ReadString("Size", "0,0"), "%d,%d", &pForm->nW, &pForm->nH);

				//if(bIsIphone5)
				//	pForm->nY	-= 88;

				pForm->nPosX	= SCREEN_SIZE_W*POINT_PIX_CNT*pForm->nCX/100 + pForm->nX;
				pForm->nPosY	= SCREEN_SIZE_H*POINT_PIX_CNT*pForm->nCY/100 + pForm->nY;
				pForm->bLocked = atoi(pStr->ReadString("Lock", "0"))==0 ? false : true;
				pForm->bUnTrhough = atoi(pStr->ReadString("Clicked", "0"))==0 ? false : true;
				m_pListForms->Add(pForm);
			}
			else if (strncmp("COMPONENT_", pStr->sectionName, 10) == 0)//component
			{
				
				int k=strlen(pStr->sectionName)-1;
				while (pStr->sectionName[k] != '_')
					k--;

				DComponent * pComp = new DComponent(&pStr->sectionName[k+1]);
				pComp->pParent	= pForm;
				stringBuf = pStr->ReadString("type", "0");
				pComp->byType	= atoi (stringBuf);
				if (pComp->byType == COMPONENT_EDIT)
				{
					pComp->byInputType = INPUT_TYPE_ALL;
				}
				else if(pComp->byType == COMPONENT_LINK)
					pComp->pSTextCenter	= new SpecialTextCenter();
				stringBuf	= pStr->ReadString("sprName", NULL);
				if(stringBuf)
					pComp->szSpr	= strdup(stringBuf);
				else
					pComp->szSpr	= NULL;

				pComp->szSelectSpr	= NULL;

				sscanf(pStr->ReadString("Position", "0,0"), "%d,%d", &pComp->nX, &pComp->nY);
				sscanf(pStr->ReadString("Size", "0,0"), "%d,%d", &pComp->nW, &pComp->nH);
				pComp->m_nOrder = atoi(pStr->ReadString("Order", "0"));
				sscanf(pStr->ReadString("CaptionColor", "255,255,255"), "%d,%d,%d", &pComp->nR, &pComp->nG, &pComp->nB);
				pComp->nMaxChar = atoi(pStr->ReadString("CaptionMaxLength", "0"));

				if(pComp->nMaxChar && pComp->byType == COMPONENT_BUTTON)
				{
					pComp->SetEvent(DefaulEvent);
				}

				pComp->bAutoChgLine	= true;
				pComp->nObjPixWidth	= 0;
				pComp->nSprPixHeight= 0;
				pComp->nSprPixWidth = 0;
				stringBuf = pStr->ReadString("Caption", "");
				if(stringBuf)
					pComp->szCaption	= strdup(stringBuf);
				else
					pComp->szCaption	= NULL;
				pForm->pListComps->Add(pComp);

				if(strcmp (pComp->szName,"close") == 0)
					pComp->SetEvent (VirCloseEvent,NULL);
			}
		}
	} while (pStr->sectionName != NULL);
	pStr->Free();


	m_pLockForm	= NULL;
	m_pHideBackForm	= NULL;
	m_bLockBtnEff = false;
}

DInterface::~DInterface()
{
	while(m_pListForms->Count)
	{
		delete (DForm*)m_pListForms->Delete (0);
	}
	m_pListForms->Free ();

	while(g_pListFPartCache->Count)
	{
		delete (CCtrlFPart*)g_pListFPartCache->Delete(0);
	}
	g_pListFPartCache->Free();

	m_pListRequest->Free ();
}

DForm	* DInterface::GetDForm( char* szNID )
{
	DForm * pForm	= NULL;
	for (int i = 0; i < m_pListForms->Count; i++)
	{
		pForm	= (DForm *)m_pListForms->Items[i];
		if(strcmp (pForm->szName,szNID) == 0)
			return pForm;
	}
	return NULL;
}

void DInterface::CloseAllForm()
{
	DForm * pForm	= NULL;
	for (int i = 0; i < m_pListForms->Count; i++)
	{
		pForm	= (DForm *)m_pListForms->Items[i];
		if(pForm->bZorde != 2)	//提示界面不关闭
			pForm->Close ();
	}
}

DComponent* DInterface::GetComponent( char* szNF, char* szNC )
{
	DForm* pF	= GetDForm (szNF);
	if(pF)
		return pF->GetComponent (szNC);
	return NULL;
}


void DInterface::RequestOpen( DForm* pF )
{
	m_pListRequest->Add (pF);
}

void  DInterface::Update()
{
	if(m_pLockForm && m_pLockForm->bOpened == false)
	{
		m_pLockForm	= NULL;
		pMainMenu->m_pMessageBox->m_pLastLockForm		= NULL;
	}
	if(m_pHideBackForm && m_pHideBackForm->bOpened == false)
	{
		m_pHideBackForm	= NULL;
	}

	DWORD	dwCurTick	= xnGetTickCount();
	for (int i = g_pListFPartCache->Count-1; i >= 0; i--)
	{
		CCtrlFPart* pTmpFP	= (CCtrlFPart*)g_pListFPartCache->Items[i];
		if( dwCurTick-pTmpFP->m_dwTick > 3000)
		{
			g_pListFPartCache->Delete(i);
			delete pTmpFP;
		}
	}
}

void DInterface::CloseAllFormBut( int* pZorder,int nCount )
{
	DForm * pForm	= NULL;
	for (int i = 0; i < m_pListForms->Count; i++)
	{
		pForm	= (DForm *)m_pListForms->Items[i];
		bool bJump = false;
		for(int j=0;j<nCount;j++){
			if(pForm->bZorde == pZorder[j]){
				bJump = true;//提示界面不关闭
				break;
			}
		}
		if(bJump) continue;
		pForm->Close ();
	}
}

void CB_DTTipFrameEvent(int nPos,char* szName)
{
	if(strncmp(szName,"icon",4) == 0)
	{
		pMainToopTip->m_pFormGetTip->GetComponent("txt")->SetCaption((char*)pMainToopTip->m_pFormGetTip->pCFrame->GetFPTag(nPos),0);
	}
}

void DTGTipCloseEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	while(pMainToopTip->m_pListGTips->Count)
		delete (GetTipInfo*)pMainToopTip->m_pListGTips->Delete(0);

	pMainToopTip->m_pFormGetTip->pCFrame->SetFPCnt(0);
	pMainToopTip->m_pFormGetTip->Close();
}

DToopTipCenter::DToopTipCenter()
{
	m_pListToopTips		= xnList::Create ();
	m_pListTipMsg		= xnList::Create ();
	m_pListRequest		= xnList::Create ();
	m_pListFace			= xnList::Create();
	m_pTipMsgCJ		= NULL;
	m_pTipCornerMsg = NULL;
	m_pWaitingSpr	= NULL;
	m_pWaitingSpr2	= NULL;
	m_pShowComp		= NULL;
	m_pTipPopMenu	= NULL;

	m_nGFOpened		= -1;

	//m_pFormGetTip	= pMainInterface->GetDForm ("gettips");
	//m_pFormGetTip->bZorde	= 50;
	//m_pFormGetTip->pCFrame	= new CCtrlFrame(85,60,440,200);


	//DComponent*	pComp	= m_pFormGetTip->GetComponent ("d");
	//
	//DComponent*	ppComps[3];
	//ppComps[0]		= m_pFormGetTip->GetComponent ("name");
	//ppComps[0]->SetSprColor(120,155,45);
	//ppComps[1]		= m_pFormGetTip->GetComponent ("icon");
	//ppComps[2]		= m_pFormGetTip->GetComponent ("num");

	//m_pFormGetTip->pCFrame->nSpace=2;
	//m_pFormGetTip->pCFrame->nDisStep	= 120;
	//m_pFormGetTip->pCFrame->InitFParts (pComp,ppComps,3);
	//m_pFormGetTip->pCFrame->SetEvent(CB_DTTipFrameEvent);
	//
	//m_pListGTips	= xnList::Create();
	//m_pFormGetTip->GetComponent("ok")->SetEvent(DTGTipCloseEvent,this);
	//m_pFormGetTip->GetComponent("txt")->SetSprColor(120,155,45);

	m_bGTipMarkChg	= false;
}

DToopTipCenter::~DToopTipCenter()
{
	while(m_pListToopTips->Count)
	{
		delete (ToopTip9Spr*)m_pListToopTips->Delete (0);
	}
	m_pListToopTips->Free ();
	while(m_pListTipMsg->Count)
	{
		delete (ToopTip9Spr*)m_pListTipMsg->Delete (0);
	}
	while(m_pListFace->Count)
		m_pListFace->Delete(0);
	m_pListFace->Free();
	m_pListTipMsg->Free ();
	m_pListRequest->Free ();

	while(m_pListGTips->Count)
		delete (GetTipInfo*)m_pListGTips->Delete(0);
	m_pListGTips->Free();
}

void DToopTipCenter::ShowInterfaceTip( char* szTip)
{
	ToopTip9Spr*	pTipSpr	= NULL;
	for (int i = 0; i < m_pListTipMsg->Count; i++)
	{
		pTipSpr		= (ToopTip9Spr*)m_pListTipMsg->Items[i];
		if(strcmp(pTipSpr->m_szTip,szTip)==0)	//放下面函数会导致原偏移位置不断增加
			return;
	}
	
	for (int i = 0; i < m_pListTipMsg->Count; i++)
	{
		pTipSpr		= (ToopTip9Spr*)m_pListTipMsg->Items[i];

		pTipSpr->m_nX+= 10;
		pTipSpr->m_nY+= 15;
		pTipSpr->m_bChgPos	= true;
	}

	pTipSpr	= new ToopTip9Spr(SCREEN_SIZE_W*POINT_PIX_CNT/3,SCREEN_SIZE_H*POINT_PIX_CNT/3,300,100,3,"9spr2");
	pTipSpr->m_pDataSTC->AddSText (1,0,0,szTip,244,222,44);
	pTipSpr->m_szTip	= strdup(szTip);
	m_pListTipMsg->Add (pTipSpr);
	m_pListRequest->Add (pTipSpr);
	//return pTipSpr;
}

void DToopTipCenter::Update()
{
	for (int i = m_pListTipMsg->Count-1; i >= 0; i--)
	{
		ToopTip9Spr*	pTipSpr		= (ToopTip9Spr*)m_pListTipMsg->Items[i];
		if(pTipSpr->m_bIsDel)
		{
			m_pListTipMsg->Delete (i);
			delete pTipSpr;
		}
	}
	UpdateFace();
	//if(pMainMenu->m_pFightMenu->m_pForm->bOpened)
	//	m_pFormGetTip->Close();
	//else if(m_pFormGetTip->bOpened == false && m_pListGTips->Count)
	//	m_pFormGetTip->Open();

	if(m_bGTipMarkChg/* && !pMainMenu->m_pFightMenu->m_pForm->bOpened*/)
	{
		m_bGTipMarkChg	= false;
		m_pFormGetTip->pCFrame->SetFPCnt(m_pListGTips->Count,true);
		
		for (int i = 0; i < m_pListGTips->Count; i++)
		{
			GetTipInfo* pGTI	= (GetTipInfo*)m_pListGTips->Items[i];

			m_pFormGetTip->pCFrame->GetFPComp(i,"name")->SetCaption(pGTI->szName,1);
			char	szTmp[64];
			if(pGTI->bType	== 3)
				sprintf(szTmp,"item3/%d.png",pGTI->nSpr);
			else
				sprintf(szTmp,"item1/%d.png",pGTI->nSpr);
			m_pFormGetTip->pCFrame->GetFPComp(i,"icon")->SetViewSpr(szTmp);
			m_pFormGetTip->pCFrame->GetFPComp(i,"name")->SetCaption(pGTI->szName,0);
			m_pFormGetTip->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",1,pGTI->nNum);
			m_pFormGetTip->pCFrame->SetFPTag(i,pGTI->szTxt);

			if(i == 0 )
				m_pFormGetTip->GetComponent("txt")->SetCaption(pGTI->szTxt,1);
		}
	}
}

void DToopTipCenter::ShowPopMenus( ToopTip9Spr* pTipD )
{
	if(m_pTipPopMenu && m_pTipPopMenu->m_bIsDel == false)
		m_pTipPopMenu->m_bRequestDel	= true;

	m_pTipPopMenu	= pTipD;
	pTipD->m_bIsDel	= false;
	pTipD->m_pDataSTC->bNeedRefresh	= true;
	m_pListRequest->Add (pTipD);
}

#define GET_ADD_TXT_BASE(val,txt)	\
	if(val)	\
	{	\
		sprintf(szTmp,"%s:% 5d ",txt,val);	\
		pTipSpr->m_pDataSTC->AddSText (nIndex++,0,true,szTmp,255,255,255);	\
		bChgLine	= !bChgLine;	\
	}

#define GET_ADD_TXT_STAR(val,txt)	\
	if(val)	\
{	\
	sprintf(szTmp,"%s:% 5d ",txt,val);	\
	pTipSpr->m_pDataSTC->AddSText (nIndex++,0,true,szTmp,0,255,255);	\
	bChgLine	= !bChgLine;	\
}


void DToopTipCenter::ShowCornerSysMsg(char * szMsg)
{
	DComponent* pComp = pMainInterface->GetComponent("main4","gonggao");
	int nInfoLayerLen = 300;
	int nRX	= pComp->pParent->nPosX+pComp->nX;
	int nRY = pComp->pParent->nPosY+pComp->nY;
	m_pTipMsgCJ	= new ToopTip9Spr(nRX,nRY,pComp->nW,pComp->nH,2,"9spr2");
	m_pTipMsgCJ->m_pDataSTC->AddSText (1,0,true,szMsg,244,222,44);
	m_pListTipMsg->Add (m_pTipMsgCJ);
	m_pListRequest->Add (m_pTipMsgCJ);
}

void DToopTipCenter::ShowMsgTipOnComp(char * szMsg, DComponent* pComp)
{
/*	if(m_pTipMsgCJ)
		m_pTipMsgCJ->m_bRequestDel	= true;*/
	int nInfoLayerLen = 300;
	int nRX	= pComp->pParent->nPosX+pComp->nX;
	if (nRX-nInfoLayerLen<0)
	{
		nRX+=pComp->nW;
	}
	else
	{
		nRX-=nInfoLayerLen;
	}
	int nRY = pComp->pParent->nPosY+pComp->nY;

	m_pTipMsgCJ	= new ToopTip9Spr(nRX,nRY,300,100,3,"9spr2");
	m_pTipMsgCJ->m_pDataSTC->AddSText (1,0,0,szMsg,244,222,44);
	m_pListTipMsg->Add (m_pTipMsgCJ);
	m_pListRequest->Add (m_pTipMsgCJ);
}

void DToopTipCenter::CloseAllInterfaceTip()
{
	if(this == NULL)
		return;
	for (int i = 0; i< m_pListTipMsg->Count; i++)
	{
		ToopTip9Spr* pTip	= (ToopTip9Spr*)m_pListTipMsg->Items[i];
		pTip->m_bRequestDel	= true;
	}
}

void DToopTipCenter::AddGetTipObj( char*	szName, char* szTxt, int nSpr, int nNum, BYTE bType )
{
	if(pChar->m_dwTickJoin==0 || xnGetTickCount() - pChar->m_dwTickJoin < 8000)
		return;

	//if(pMainMenu->m_pFightMenu->m_pForm->bOpened)
	//	return;

	GetTipInfo*	pAddGTI	= NULL;
	for (int i = 0; i < m_pListGTips->Count; i++)
	{
		GetTipInfo*	pGTI	= (GetTipInfo*)m_pListGTips->Items[i];
		if(strcmp(pGTI->szName,szName) == 0)
		{
			pAddGTI		= pGTI;
			break;
		}
	}
	if(pAddGTI)
	{
		pAddGTI->nNum	+= nNum;
	}
	else
	{
		pAddGTI				= new GetTipInfo();
		pAddGTI->szName		= szName;
		pAddGTI->szTxt		= szTxt;
		pAddGTI->bType		= bType;
		pAddGTI->nNum		= nNum;
		pAddGTI->nSpr		= nSpr;
		m_pListGTips->Insert(0,pAddGTI);
	}

	m_bGTipMarkChg	= true;
}

void DToopTipCenter::ShowTalkTip( ToopTip9Spr* pTTip,char* szTip )
{
	pTTip->m_pDataSTC->ClearSText ();
	pTTip->m_pDataSTC->m_byFontSize	= 16;
	pTTip->m_pDataSTC->AddSText (1,0,false,szTip,180,210,30);
	pTTip->m_dwMarkVal	= xnGetTickCount ()+5000;
	pTTip->m_pDataSTC->bDisableTouch = true;
	pTTip->m_bIsDel	= false;
}

void DToopTipCenter::ShowTalkTip(ToopTip9Spr* pTTip,ChatInfo* pMsg)
{
	pTTip->m_pDataSTC->ClearSText ();
	pTTip->m_pDataSTC->m_byFontSize	= 16;
	BYTE byR = 180;
	BYTE byG = 210;
	BYTE byB = 30;
	while(m_pListFace->Count > 0)
		m_pListFace->Delete(0);
	std::string strWords(pMsg->szMsg);
	std::vector<std::string> vec;
	StringSplit(strWords,"/#",vec);
	int nCount = 0;
	int nIndex = 0;
	for(int j = 0; j < vec.size(); j ++)
	{
		if(j == 0)
		{
			pTTip->m_pDataSTC->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),byR,byG,byB);
			continue;
		}
		if(vec[j].size() < 4)
			pTTip->m_pDataSTC->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),byR,byG,byB);
		std::string strKey = vec[j].substr(0,4);
		if(strKey.compare("!WP$") == 0)		//物品
		{
			if(nCount < pMsg->nCnt)
			{
				ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pMsg->xInfo[nCount].nItemType);				
				if(pItem)
				{
					char szItemName[32];
					sprintf(szItemName,"[%s]",pItem->szName);
					pTTip->m_pDataSTC->AddSText(nIndex++,0,false,szItemName,nItemColor[pItem->nStar][0],nItemColor[pItem->nStar][1],nItemColor[pItem->nStar][2]);
					vec[j].erase(0,4);
					pTTip->m_pDataSTC->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),byR,byG,byB);
				}
				nCount ++;
			}
			else
			{
				vec[j].erase(0,4);
				pTTip->m_pDataSTC->AddSText(nIndex++,0,true,(char*)vec[j].c_str(),byR,byG,byB);
			}
		}
		else			//表情
		{
			int nIcon = atoi(strKey.c_str());
			char szIcon[32];
			sprintf(szIcon,"ui/face/%d/1.png",nIcon);
			STextObj* pTextObj = pTTip->m_pDataSTC->AddSText(nIndex++,1,false,szIcon,255,255,255);
			pTextObj->nOffY = 1;
			m_pListFace->Add((void*)pTextObj);
			vec[j].erase(0,4);
			pTTip->m_pDataSTC->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),byR,byG,byB);
		}
	}
// 	pTTip->m_pDataSTC->AddSText (1,0,false,szTip,180,210,30);
 	pTTip->m_dwMarkVal	= xnGetTickCount ()+5000;
 	pTTip->m_pDataSTC->bDisableTouch = true;
 	pTTip->m_bIsDel	= false;
}

void	DToopTipCenter::ClearFace(ToopTip9Spr* pTTip)
{
	if(pTTip== NULL || pTTip->m_pDataSTC == NULL)
		return ;
	for(int i = 0; i < pTTip->m_pDataSTC->m_pList->Count; i ++)
	{
		m_pListFace->Remove(pTTip->m_pDataSTC->m_pList->Items[i]);
	}
}

void DToopTipCenter::UpdateFace()
{
	static DWORD dwFTick = 40;
	if(m_pListFace->Count == 0)
		return;
	for(int i = m_pListFace->Count-1; i >= 0; i --)
	{
		STextObj* pObj = (STextObj*)m_pListFace->Items[i];
		int nID = 0;
		int nFrm = 0;
		if(pObj->szContent)
		{
			sscanf(pObj->szContent,"ui/face/%d/%d.png",&nID,&nFrm);
			FaceObj* pFace = FaceConfig::GetInstance().GetFace(nID);
			if(pFace && (dwFTick %pFace->nSpeed) == 0)
			{
				nFrm ++;
				if(nFrm > pFace->nTotalFrm)
					nFrm = 1;
				char szIcon[32];
				sprintf(szIcon,"ui/face/%d/%d.png",nID,nFrm);
				free(pObj->szContent);
				pObj->szContent = strdup(szIcon);
				pObj->bReset = true;
			}
		}
	}
	dwFTick += 40;
}