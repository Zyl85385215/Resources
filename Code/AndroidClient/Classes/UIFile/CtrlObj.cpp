#include "cocos2d.h"
#include "CtrlObj.h"
#include "Interface.h"
#include "InterfaceLayer.h"
#include "SoundCenter.h"

STextObj::STextObj()
{
	bChgLine	= false;
	byR			= 222;
	byG			= 222;
	byB			= 222;
	byType		= 0;			//0文本, 1图片
	szContent	= NULL;    //内容 
	bReset		= true;
	nOffX		= 0;
	nOffY		= 0;
	nViewPosY	= 0;
	nFontSize	= 20;
	bTouchEvent	= false;
}

STextObj::~STextObj()
{
	if(szContent)
		free(szContent);
}

void SpecialSlideEvent(InterfaceEvent* pEvent,void* param)
{
	if(param)
	{
		SpecialTextCenter* pST = (SpecialTextCenter*)param;
		switch(pEvent->event)
		{
		case INTERFACE_EVENT_DOWN:
		case INTERFACE_EVENT_MOVE:
			{
				if(!pST->m_bMoveSwitch)
					return;
			//	pST->m_nSChangeY = pEvent->y - (pST->m_pSlideMove->nY + pST->m_pSlideMove->nH/2);
			//	pST->bNeedRefresh = true;
				int nPosY = pEvent->y + pST->m_pSlideMove->nH;
				if(nPosY < pST->m_pSlideBack->nY + pST->m_pSlideMove->nH/2)
					nPosY = pST->m_pSlideBack->nY + pST->m_pSlideMove->nH/2;
				else if(nPosY > pST->m_pSlideBack->nY + pST->m_pSlideBack->nH - pST->m_pSlideMove->nH/2)
					nPosY = pST->m_pSlideBack->nY + pST->m_pSlideBack->nH - pST->m_pSlideMove->nH/2;
				pST->m_pSlideMove->SetPos(pST->m_pSlideMove->nX,nPosY- pST->m_pSlideMove->nH/2);
				pST->m_SameSwitch = true;
			}
			break;
		}
	}
}

SpecialTextCenter::SpecialTextCenter()
{
	m_pList		= xnList::Create();
	bNeedRefresh	= false;
	bDisableTouch	= false;
	bAutoBottom		= false;
	m_byFontSize	= 20;
	m_pSlideBack	= NULL;
	m_pSlideMove	= NULL;
	m_SameSwitch	= false;
	m_bSetBottom	= false;
	m_bMoveSwitch	= false;
	m_bReSetPosY    = true;
}

SpecialTextCenter::~SpecialTextCenter()
{
	while(m_pList->Count)
	{
		delete (STextObj*)m_pList->Delete (0);
	}
	m_pList->Free ();
}

void SpecialTextCenter::SetSlide(DComponent* pBack,DComponent* pMove, bool bResetPY)
{
	if(pBack == NULL || pMove == NULL)
		return;
	m_bReSetPosY = bResetPY;
	m_pSlideBack = pBack;
	m_pSlideMove = pMove;
	m_pSlideBack->SetEvent(SpecialSlideEvent,this);
	m_pSlideMove->SetPos(m_pSlideMove->nX,m_pSlideBack->nY);
}

STextObj* SpecialTextCenter::AddSText( int nPID,BYTE bType,bool bChgLine,char* szContent, BYTE R, BYTE G, BYTE B,void* pParam /*= NULL*/,void (*Fun)(void* pParam)/*=NULL*/, int nOffY)
{
	STextObj*	pSTO	= new STextObj();
	pSTO->nPID			= nPID;
	pSTO->bChgLine		= bChgLine;
	pSTO->byType		= bType;
	pSTO->byR			= R;
	pSTO->byG			= G;
	pSTO->byB			= B;
	pSTO->pParam		= pParam;
	pSTO->nOffY			= nOffY;
	pSTO->funCBLink		= Fun;
	pSTO->nFontSize		= m_byFontSize;

	while(strlen (szContent) > 65)
	{
		//需要从头开始找,不能中间截断
		int nCheckPos	= 0;
		while(nCheckPos < 64)
		{
			if(szContent[nCheckPos] >= 0x80 || szContent[nCheckPos] < 0)
				nCheckPos+=2;
			else
				nCheckPos+=1;
		}
		char	szTmp[256];
		strncpy (szTmp,szContent,nCheckPos);
		szTmp[nCheckPos]	= 0;
		szContent	+= nCheckPos;
		if(nPID >= 10000000 )
			AddSText(nPID + 10000000,bType,false,szTmp,R,G,B,pParam,Fun);
		else
			AddSText(nPID + 10000000,bType,bChgLine,szTmp,R,G,B,pParam,Fun,nOffY);

		nPID+=10000000;
		pSTO->bChgLine	= false;
		pSTO->nOffY		= 0;
	}

	pSTO->szContent		= strdup(szContent);
	m_pList->Add (pSTO);


	bNeedRefresh	= true;

	return pSTO;
}

void SpecialTextCenter::AnalyText( char* szTxt )
{


	bNeedRefresh	= true;
}

void SpecialTextCenter::RemoveSText( int	nPIDF,int nPIDT )
{
	for (int i = m_pList->Count-1; i>= 0; i--)
	{
		STextObj*	pSTO	= (STextObj*)m_pList->Items[i];
		if(pSTO->nPID	>= nPIDF && pSTO->nPID <= nPIDT)
		{
			m_pList->Delete (i);

			delete pSTO;
		}
	}

	bNeedRefresh	= true;
}

void SpecialTextCenter::ClearSText()
{
	while(m_pList->Count)
	{
		delete (STextObj*)m_pList->Delete (0);
	}

	bNeedRefresh	= true;
}

bool SpecialTextCenter::ChgSText( int nPID, char* szContent, BYTE R, BYTE G, BYTE B )
{
	for (int i = m_pList->Count-1; i>= 0; i--)
	{
		STextObj*	pSTO	= (STextObj*)m_pList->Items[i];
		if(pSTO->nPID	== nPID)
		{
			pSTO->byR			= R;
			pSTO->byG			= G;
			pSTO->byB			= B;
			if(pSTO->szContent)
				free(pSTO->szContent);
			pSTO->szContent		= strdup(szContent);
			pSTO->bReset	= true;
			return true;
		}
	}
	return false;
}

STextObj*	SpecialTextCenter::GetSText(int nPID)
{
	for (int i = m_pList->Count-1; i>= 0; i--)
	{
		STextObj*	pSTO	= (STextObj*)m_pList->Items[i];
		if(pSTO->nPID	== nPID)
			return pSTO;
	}
	return NULL;
}

ToopTip9Spr::ToopTip9Spr( int nX, int nY, int nW, int nH,BYTE  bType,char* sz9Spr )
{
	m_pDataSTC	= new SpecialTextCenter();
	m_nX		= nX;
	m_nY		= nY;
	m_nW		= nW;
	m_nH		= nH;
	m_bIsDel	= false;
	m_bRequestDel	= false;
	m_bChgPos	= false;
	m_bType		= bType;
	if(m_bType & 0x02)
		m_dwMarkVal	= xnGetTickCount ()+5000;
	else
		m_dwMarkVal	= 0;
	m_sz9Spr	= strdup(sz9Spr);
	m_szTip		= NULL;
}

ToopTip9Spr::~ToopTip9Spr()
{
	delete m_pDataSTC;
	free(m_sz9Spr);
	if(m_szTip)
		free(m_szTip);
}

CPopMenuCtrl::CPopMenuCtrl( int nX, int nY, int nW, int nH, void (*cbMenuClick)(void* param,int nMenuIndex),void* param )
{
	m_pListMItem	= xnList::Create ();
	Func_MenuClick	= cbMenuClick;
	m_pParam		= param;

	m_pTipData	= new ToopTip9Spr(nX, nY, nW, nH, 0,"9spr2");
	m_nMICount	= 0;
	m_pTipData->m_bIsDel	= true;
	//g_pSoundCenter->PlayerEffect ("Sound/form_open");
}

CPopMenuCtrl::~CPopMenuCtrl()
{
	while(m_pListMItem->Count)
		delete (MenuItemData*)(m_pListMItem->Delete (0));
	m_pListMItem->Free ();
	if(pMainToopTip->m_pTipPopMenu	== m_pTipData)
		pMainToopTip->m_pTipPopMenu	= NULL;
	delete m_pTipData;
	//g_pSoundCenter->PlayerEffect ("Sound/form_close");
}

void fnCBPopMenuClick(void* pParam)
{
	MenuItemData*	pMI	= (MenuItemData*)pParam;
	pMI->pParat->Func_MenuClick (pMI->pParat->m_pParam,pMI->nIndex);
	pMI->pParat->m_pTipData->m_bIsDel	= true;
}

void CPopMenuCtrl::AddMenuItem( char* szMItem )
{
	MenuItemData*	pMI	= new MenuItemData();
	pMI->pParat	= this;
	pMI->nIndex	= m_nMICount;
	m_pListMItem->Add (pMI);

	m_pTipData->m_pDataSTC->AddSText(m_nMICount,0,true,szMItem,55,233,33,pMI,fnCBPopMenuClick,m_nMICount?20/POINT_PIX_CNT:0);
	m_nMICount++;
}

void CPopMenuCtrl::PopMenu(BYTE bCloseType)
{
	if(m_pTipData->m_bIsDel)
	{
		pMainToopTip->ShowPopMenus (m_pTipData);
	}
}

void CPopMenuCtrl::ClearMItem()
{
	while(m_pListMItem->Count)
		delete (MenuItemData*)(m_pListMItem->Delete (0));

	m_pTipData->m_pDataSTC->ClearSText();
	m_nMICount	= 0;
}

void RadioCBEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	CRadioParam*	pRP	= (CRadioParam*)param;
	pRP->pParent->SetSel(pRP->nPos);
}

CCtrlRadio::CCtrlRadio( DComponent* pComp[], int nCnt )
{
	m_ppRadioParams	= new CRadioParam*[nCnt];
	m_nCnt			= nCnt;
	for (int i = 0; i < nCnt; i++)
	{
		m_ppRadioParams[i]	= new CRadioParam();
		m_ppRadioParams[i]->pParent	= this;
		m_ppRadioParams[i]->nPos	= i;
		m_ppRadioParams[i]->pComp	= pComp[i];
		pComp[i]->SetEvent(RadioCBEvent,m_ppRadioParams[i]);
	}
	SetSel(0);
}

CCtrlRadio::~CCtrlRadio()
{

}

void CCtrlRadio::SetSel( int nSel )
{
	for (int i = 0; i < m_nCnt; i++)
	{
		if( i != nSel )
			m_ppRadioParams[i]->pComp->SetViewSpr("");
		else
			m_ppRadioParams[i]->pComp->SetViewSpr("BT_GOU.png");
	}
	m_nSelRadio = nSel;
}

void fnCCehckEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	CCtrlCheck* pCCheck	= (CCtrlCheck*)param;

	if(!pCCheck->m_ifEnable)
		return;
	pCCheck->SetCheck (!pCCheck->GetCheck ());
}

CCtrlCheck::CCtrlCheck( DComponent* pComp, BYTE bType )
{
	m_pComp		= pComp;
	m_bType		= bType;
	m_pComp->SetEvent (fnCCehckEvent,this);
	SetCheck (false);
	SetEnable(true);
}

CCtrlCheck::~CCtrlCheck()
{

}

void CCtrlCheck::SetEnable(bool bOn)
{
	m_ifEnable = bOn;
	m_pComp->SetEnable(bOn);
}

bool CCtrlCheck::GetCheck()
{
	return m_bCheck;
}

void CCtrlCheck::SetCheck( bool bCheck )
{
	char	szSpr[256];
	switch(m_bType)
	{
	case 0:
		if(bCheck)
			strcpy(szSpr,"BT_CW_GOU.png");
		else
			strcpy(szSpr,"");
		break;
	case 1:
		if(bCheck)
			strcpy(szSpr,"BT_KG.png");
		else
			strcpy(szSpr,"BT_KG2.png");
		break;
	}
	m_bCheck	= bCheck;
	m_pComp->SetViewSpr (szSpr);
}

void fnCSlideMEvent(InterfaceEvent * pEvent,void * param)
{
	CCtrlSlide* pSlide	= (CCtrlSlide*)param;
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_DOWN:
	case INTERFACE_EVENT_MOVE:
		{//改控件位置后相对位置变了会弹回来
			int	nChgTo	= pEvent->x+pSlide->m_pCompBack->nX-pSlide->m_pCompMove->nW/2;
			if(nChgTo < pSlide->m_pCompBack->nX-pSlide->m_pCompMove->nW/2)
				nChgTo	= pSlide->m_pCompBack->nX-pSlide->m_pCompMove->nW/2;
			if(nChgTo > pSlide->m_pCompBack->nX+pSlide->m_pCompBack->nW-pSlide->m_pCompMove->nW/2)
				nChgTo	= pSlide->m_pCompBack->nX+pSlide->m_pCompBack->nW-pSlide->m_pCompMove->nW/2;
			pSlide->m_pCompMove->SetPos (nChgTo,pSlide->m_pCompMove->nY);
			//s_nslideX	= pEvent->x;
		}
		break;
	}
}

CCtrlSlide::CCtrlSlide( DComponent* pCompBack, DComponent* pCompMove )
{
	m_pCompBack		= pCompBack;
	m_pCompMove		= pCompMove;

	m_pCompBack->SetEvent(fnCSlideMEvent,this);
	SetSlide(50);
}

CCtrlSlide::~CCtrlSlide()
{

}

void CCtrlSlide::SetSlide(int nVal)
{
	if(nVal > 100)	return;
	if(nVal < 0)	return;

	int nPos	= nVal*m_pCompBack->nW/100;
	m_pCompMove->SetPos (m_pCompBack->nX+nPos-m_pCompMove->nW/2,m_pCompMove->nY);
}

int CCtrlSlide::GetSlide()
{
	return (m_pCompMove->nX+m_pCompMove->nW/2 - m_pCompBack->nX)*100/m_pCompBack->nW;
}


_CLItemTxt::~_CLItemTxt()
{
	if(szTxt) 
		free(szTxt);
}

_CtrlListItem::~_CtrlListItem()
{
	std::map<int,_CLItemTxt*>::iterator	itor	= mapChildren.begin ();
	while(itor != mapChildren.end ())
	{
		delete itor->second;
		mapChildren.erase (itor++);
	}
}

_CLItemTxt* _CtrlListItem::GetItemTxt( int nRow )
{
	std::map<int,_CLItemTxt*>::iterator	itor	= mapChildren.find (nRow);
	if(itor == mapChildren.end ())
	{
		mapChildren[nRow]	= new _CLItemTxt();
	}
	return mapChildren[nRow];
}

CCtrlList::CCtrlList( DComponent* pCpLink )
{
	m_szSelSpr	= NULL;
	m_pCompLink	= pCpLink;
	m_pListItems	= xnList::Create ();
	m_pnRow		= NULL;
	m_nRowCnt	= 1;
	m_bTxtCenter	= false;
}

CCtrlList::~CCtrlList()
{
	while(m_pListItems->Count)
		delete (_CtrlListItem*)m_pListItems->Delete (0);
	m_pListItems->Free ();

	if(m_pnRow)
		delete[] m_pnRow;
}

void CCtrlList::SetRowCnt( DComponent* ppCP[],int nCnt )
{
	m_nRowCnt	= nCnt;
	m_pnRow		= new int[nCnt];
	int nStartX	= ppCP[0]->nX;
	for (int i = 0; i < nCnt; i++)
	{
		m_pnRow[i]	= ppCP[i]->nX-nStartX;
	}
}

void CCtrlList::SetTxtCenter( bool bCenter )
{
	m_bTxtCenter	= bCenter;
}

int CCtrlList::InsertItem( char* szTxt, int nPos/*=-1*/ )
{
	_CtrlListItem* pItem	= new _CtrlListItem();
	_CLItemTxt* pTxt		= pItem->GetItemTxt (0);
	pTxt->bType		= 0;
	pTxt->szTxt		= strdup(szTxt);
	int nRtl	= 0;
	if(nPos == -1)
		nRtl	= m_pListItems->Add (pItem);
	else
		nRtl	= m_pListItems->Insert (nPos,pItem);

	UpdateView ();
	return nRtl;
}

void CCtrlList::SetItemText( int nPos, int nRow,char* szTxt )
{
	if(nRow >= m_nRowCnt)
		return;
	if(nPos >= m_pListItems->Count)
		return;

	_CtrlListItem*	pItem	= (_CtrlListItem*)m_pListItems->Items[nPos];
	_CLItemTxt*	pTxt	= pItem->GetItemTxt (nRow);
	if(pTxt->szTxt)
		free(pTxt->szTxt);

	pTxt->szTxt		= strdup(szTxt);
	pTxt->bType		= 0;

	UpdateView ();
}

void CCtrlList::SetItemData( int nPos, void* pData )
{
	if(nPos >= m_pListItems->Count)
		return;

	_CtrlListItem*	pItem	= (_CtrlListItem*)m_pListItems->Items[nPos];
	pItem->param	= pData;
}

void CCtrlList::UpdateView()
{
	int nIndex	= 0;
	m_pCompLink->pSTextCenter->ClearSText ();
	bool bChgLine	= true;
	STextObj*	pSTO	= NULL;
	for (int i = 0; i < m_pListItems->Count; i++)
	{
		_CtrlListItem* pItem	= (_CtrlListItem*)m_pListItems->Items[i];
		bChgLine	= true;
		std::map<int,_CLItemTxt*>::iterator	itor	= pItem->mapChildren.begin ();
		while(itor != pItem->mapChildren.end ())
		{

			_CLItemTxt* pTxt	= itor->second;
			pSTO	= m_pCompLink->pSTextCenter->AddSText (nIndex++,pTxt->bType,bChgLine,pTxt->szTxt,pTxt->R,pTxt->G,pTxt->B);
			pSTO->nOffX	= 100000+m_pnRow[itor->first];
			bChgLine	= false;
			itor++;
		}
	}
}

void TMCSelPage(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	int		nPos	= 0;
	sscanf (pEvent->pComponent->szName,"page%d",&nPos);
	nPos--;
	
	TabMenuCtrl* pTMC	= (TabMenuCtrl*)param;
	
	pTMC->Open (1,nPos);
}

void TMCCloseEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	TabMenuCtrl* pTMC	= (TabMenuCtrl*)param;
	pTMC->Close ();
}


TabMenuCtrl::TabMenuCtrl( TabMenuObj* pObjs[], int nCnt )
{
	m_ppChildren	= new TabMenuObj*[nCnt];
	memcpy(m_ppChildren,pObjs,4*nCnt);
	m_nCnt		= nCnt;

	char	szTmp[32];
	for (int i = 0; i < nCnt; i++)
	{
		sprintf(szTmp,"page%d",i+1);
		for (int j = 0; j < nCnt; j++)
		{
			if(m_ppChildren[j])
				m_ppChildren[j]->m_pForm->GetComponent (szTmp)->SetEvent (TMCSelPage,this);
		}
		if(m_ppChildren[i])
			m_ppChildren[i]->m_pForm->GetComponent ("close")->SetEvent (TMCCloseEvent,this);
	}
	m_nCurSel	= 0;
	m_nOpenType	= 1;
	funCheckLimint = NULL;
}

TabMenuCtrl::~TabMenuCtrl()
{
	delete[]	m_ppChildren;
}


void TabMenuCtrl::Open( int nType, int nPos /*= -1*/)
{
	if(nPos == -1)
		nPos	= m_nCurSel;
	if(nPos >= m_nCnt)
		return;
	if(nType > 1)
		m_nOpenType	= nType;

	if(m_ppChildren[nPos]==NULL || m_ppChildren[nPos]->m_pForm->bOpened)
		return;

	if(funCheckLimint != NULL && funCheckLimint(nPos))
	{
		return;
	}
	if(m_ppChildren[nPos]->CheckCanOpen()==false)
		return;
	if(m_ppChildren[m_nCurSel]->m_pForm->bOpened)
	{
		m_ppChildren[m_nCurSel]->m_pForm->bOpenStyle	= 1;
		m_ppChildren[m_nCurSel]->Close ();
		//m_ppChildren[m_nCurSel]->m_pForm->Close ();
	}
	m_ppChildren[nPos]->Open (nType);
	char sztxt[32];

	for(int i = 0; i < m_nCnt; i ++)
	{
		if(m_ppChildren[i] && m_ppChildren[i]->m_pForm)
		{
			if(!m_ppChildren[i]->CheckColor(m_nCnt))
			{
				for(int j = 0; j < m_nCnt; j ++)
				{
					sprintf(sztxt,"page%d",j+1);
					DComponent* pComp = m_ppChildren[i]->m_pForm->GetComponent(sztxt);
					if(pComp)
					{
						if(j == nPos)
							pComp->SetSprColor(255,255,255);
						else
							pComp->SetSprColor(127,127,127);
					}
					
				}
			}
		}
	}	
	m_nCurSel	= nPos;

}

void TabMenuCtrl::Close()
{
	if(m_ppChildren[m_nCurSel]->m_pForm->bOpened)
	{
		m_ppChildren[m_nCurSel]->m_pForm->bOpenStyle	= m_nOpenType;
		m_ppChildren[m_nCurSel]->Close ();
	}
	else
	{
		for(int i = 0; i < m_nCnt; i ++)
		{
			if(m_ppChildren[i] && m_ppChildren[i]->m_pForm)
			{
				if(m_ppChildren[i]->m_pForm->bOpened)
				{
					m_ppChildren[i]->m_pForm->bOpenStyle	= m_nOpenType;
					m_ppChildren[i]->Close ();
				}
			}
		}	
	}
}

bool TabMenuCtrl::IsOpened()
{
	if(m_ppChildren[m_nCurSel] && m_ppChildren[m_nCurSel]->m_pForm->bOpened)
		return true;
	return false;
}

void CCombDownEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	CCtrlComb* pComb	= (CCtrlComb*)param;

	pComb->ClickDown ();
}

void fnCBPopMComb(void* param,int nMenuIndex)
{
	CCtrlComb* pCComb	= (CCtrlComb*)param;
	pCComb->SetCurSel (nMenuIndex);
}

CCtrlComb::CCtrlComb( DComponent* pTxt, DComponent* pDown )
{
	m_pCmpTxt	= pTxt;

	pDown->SetEvent (CCombDownEvent,this);

	m_nSelPos	= -1;

	m_pList		= xnList::Create ();
	m_pPopMenuCtrl	= new CPopMenuCtrl(pTxt->nX + pTxt->pParent->nPosX,pTxt->nY+pTxt->pParent->nPosY+20,pTxt->nW+60,200,fnCBPopMComb,this);
}

CCtrlComb::~CCtrlComb()
{
	while(m_pList->Count)
	{
		_CtrlCombObj* pCObj	= (_CtrlCombObj*)m_pList->Delete (0);
		free(pCObj->szTxt);
		delete pCObj;
	}
	m_pList->Free ();
	delete m_pPopMenuCtrl;
}

void CCtrlComb::AddString( char* szStr, int nParam )
{
	_CtrlCombObj*	pCObj	= new _CtrlCombObj();
	pCObj->nParam			= nParam;
	pCObj->szTxt			= strdup(szStr);
	m_pList->Add (pCObj);
}

void CCtrlComb::ClearString()
{
	while(m_pList->Count)
	{
		_CtrlCombObj* pCObj	= (_CtrlCombObj*)m_pList->Delete (0);
		free(pCObj->szTxt);
		delete pCObj;
	}
}

int CCtrlComb::GetSelParam()
{
	if(m_nSelPos == -1)
		return -1;

	_CtrlCombObj* pCObj	= (_CtrlCombObj*)m_pList->Items[m_nSelPos];
	return pCObj->nParam;
}

char* CCtrlComb::GetSelTxt()
{
	if(m_nSelPos == -1)
		return NULL;

	_CtrlCombObj* pCObj	= (_CtrlCombObj*)m_pList->Items[m_nSelPos];
	return pCObj->szTxt;
}

void CCtrlComb::ClickDown()
{
	if(m_pList->Count == 0)
		return;

	m_pPopMenuCtrl->ClearMItem ();

	for (int i = 0; i < m_pList->Count; i++)
	{
		_CtrlCombObj*	pCObj	= (_CtrlCombObj*)m_pList->Items[i];
		m_pPopMenuCtrl->AddMenuItem (pCObj->szTxt);
	}
	m_pPopMenuCtrl->PopMenu (0);

}

void CCtrlComb::SetCurSel( int nSel )
{
	if(nSel >= m_pList->Count)
		return;
	m_nSelPos	= nSel;
	m_pCmpTxt->SetCaption (GetSelTxt (),1);
}

void TabMenuObj::Close()
{
	m_pForm->Close ();
}

CCtrlFPart::CCtrlFPart(DComponent* pCB, DComponent** ppComp, int nCnt ):m_pTag(NULL)
{
	bCopyed		= true;
	pListComps	= xnList::Create ();
	pCompBase	= pCB->BeCopy ();

	for (int i = 0; i < nCnt; i++)
	{
		if(ppComp[i])
		{
			DComponent* pComp	= ppComp[i]->BeCopy ();
			pComp->nX	-= pCompBase->nX;
			pComp->nY	-= pCompBase->nY;
			pListComps->Add (pComp);
		}
	}
	nNewSpace		= 0;
	pCompBase->nX	= 0;
	pCompBase->nY	= 0;
	m_dwTick	= 0;
	
}

CCtrlFPart::CCtrlFPart( CCtrlFPart* pCF )
{
	bCopyed		= true;
	pCompBase	= pCF->pCompBase->BeCopy ();
	pListComps	= xnList::Create ();
	nNewSpace	= pCF->nNewSpace;
	for (int i = 0; i < pCF->pListComps->Count; i++)
	{
		DComponent* pComp	= (DComponent*)pCF->pListComps->Items[i];
		DComponent* pMC	= pComp->BeCopy ();
		pListComps->Add (pMC);
	}
	m_dwTick	= 0;
}


CCtrlFPart::~CCtrlFPart()
{
	if(bCopyed)
	{
		while(pListComps->Count)
		{
			delete (DComponent*)pListComps->Delete (0);
		}
	}
	pListComps->Free ();
}

void CCtrlFPart::MoveXY( int nX, int nY, int nMTime )
{
	pCompBase->SetPos (pCompBase->nX + nX,pCompBase->nY + nY,nMTime);
	for (int i = 0; i < pListComps->Count; i++)
	{
		DComponent* pComp	= (DComponent*)pListComps->Items[i];
		pComp->SetPos (pComp->nX+nX,pComp->nY+nY,nMTime);
	}
}

void	CCtrlFPart::SetXY(int nX,int nY,int nMTime)
{
	int nTempX = nX-pCompBase->nX;
	int nTempY = nY-pCompBase->nY;
	pCompBase->SetPos (nX,nY,nMTime);
	for (int i = 0; i < pListComps->Count; i++)
	{
		DComponent* pComp	= (DComponent*)pListComps->Items[i];
		pComp->SetPos (pComp->nX+nTempX,pComp->nY+nTempY,nMTime);
	}
}

void CCtrlFPart::SetVisable( bool bVsb )
{
	for (int i = 0; i < pListComps->Count; i++)
	{
		DComponent* pComp	= (DComponent*)pListComps->Items[i];
		pComp->SetVisible(bVsb);
	}
}

void CCtrlFPart::SetEvent( void (*EventFunc)(InterfaceEvent * pEvent,void * param), void * para /*= NULL*/ )
{
	if(pCompBase->byType == COMPONENT_BUTTON)
		pCompBase->SetEvent (EventFunc,para);

	for (int i = 0; i < pListComps->Count; i++)
	{
		DComponent* pComp	= (DComponent*)pListComps->Items[i];
		if(pComp->byType	== COMPONENT_BUTTON)
			pComp->SetEvent (EventFunc,para);
	}
}

DComponent* CCtrlFPart::GetDComp( char* szName )
{
	if(strcmp (pCompBase->szName,szName) == 0)
		return pCompBase;

	for (int i = 0; i < pListComps->Count; i++)
	{
		DComponent* pComp	= (DComponent*)pListComps->Items[i];
		if(strcmp(pComp->szName,szName) == 0)
			return pComp;
	}
	return NULL;
}

//-----------------------------------------------------------
void MoveFrmLeft(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	CCtrlFrame* pCFrame	= (CCtrlFrame*)param;
	if(pCFrame->bIsHoriz)
		pCFrame->nChgX	= pCFrame->nDisStep;
	else
		pCFrame->nChgY	= pCFrame->nDisStep;
	pCFrame->byChgFrom	= 200;
}

void MoveFrmRight(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	CCtrlFrame* pCFrame	= (CCtrlFrame*)param;
	if(pCFrame->bIsHoriz)
		pCFrame->nChgX	= -pCFrame->nDisStep;
	else
		pCFrame->nChgY	= -pCFrame->nDisStep;
	pCFrame->byChgFrom	= 200;
}


CCtrlFrame::CCtrlFrame(int nX, int nY, int nW, int nH):bLoop(false),pFrame(NULL)
{
	pListFP		= xnList::Create ();
	nFrmX		= nX;
	nFrmY		= nY;
	nFrmW		= nW;
	nFrmH		= nH;
	nFrmMaxW	= nW;
	nFrmMaxH	= nH;
	nSpace		= 0;
	nZorder		= 0;
	nDisStep	= 0;
	nChgX		= 0;
	nChgY		= 0;
	nSetX		= 0;
	nSetY		= 0;
	pCtrlFPBase	= NULL;
	fnEventFunc	= NULL;
	fnEventFuncEx= NULL;
	fnEventFuncAdv	= NULL;
	m_pParam	= NULL;
	fnStepEventFunc = NULL;
	bChged		= false;
	bIsCreateByForm = false;
	bLimitRect		= false;
	bSplitTouch		= false;
	bDisableTouch	= false;
	nOffChgPos		= 0;
}

CCtrlFrame::~CCtrlFrame()
{
	while(pListFP->Count)
	{
		CCtrlFPart* pTmpFP	= (CCtrlFPart*)pListFP->Delete (0);
		g_pListFPartCache->Add(pTmpFP);
		pTmpFP->m_dwTick	= xnGetTickCount();
	}
	pListFP->Free ();

	if(pCtrlFPBase)
	{
		g_pListFPartCache->Add(pCtrlFPBase);
		pCtrlFPBase->m_dwTick	= xnGetTickCount();
	}
}

bool CCtrlFrame::InitFParts( DComponent* pCB, DComponent** ppComp, int nCompCnt )
{

	pCtrlFPBase		= new CCtrlFPart(pCB,ppComp,nCompCnt);
	pCB->SetVisible(false);
	pCtrlFPBase->SetVisable (false);
	for (int i=0;i<nCompCnt;i++)
	{
		ppComp[i]->SetVisible(false);
	}
	if(nFrmW < pCB->nW)
		nFrmW	= pCB->nW;
	if(nFrmH < pCB->nH)
		nFrmH	= pCB->nH;
	return true;
	//for (int i = 0; i < nCnt; i++)
	//{
	//	CCtrlFPart*	pCopyFP	= new CCtrlFPart(pCF);
	//	pListFP->Add (pCopyFP);
	//}
}

void CBFramePClick(InterfaceEvent * pEvent,void * param)
{
	CCtrlFPart* pFPart	= (CCtrlFPart*)param;
	if(pFPart->pParent->fnEventFuncAdv)
		pFPart->pParent->fnEventFuncAdv (pEvent,pFPart->nID,pEvent->pComponent->szName,pFPart->pParent->m_pParam);

	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if(pFPart->pParent->fnEventFunc)
		pFPart->pParent->fnEventFunc (pFPart->nID,pEvent->pComponent->szName);
	if(pFPart->pParent->fnEventFuncEx && pFPart->pParent->m_pParam)
		pFPart->pParent->fnEventFuncEx(pFPart->nID,pEvent->pComponent->szName,pFPart->pParent->m_pParam);
}

void	CCtrlFrame::ResetParts()
{
	bChged		= true;
	int nMoveDistance = 0;
	if(bIsHoriz)
		nMoveDistance = pCtrlFPBase->pCompBase->nX;
	else
		nMoveDistance = pCtrlFPBase->pCompBase->nY;
	for (int i = 0; i < pListFP->Count; i++)
	{
		CCtrlFPart*	pCopyFP	= (CCtrlFPart*)pListFP->Items[i];

		if(pCopyFP->nNewSpace)
		{
			if(bIsHoriz)
			{
				pCopyFP->SetXY(nSpace/2+nMoveDistance,(nFrmH-pCtrlFPBase->pCompBase->nH)/2,0);
				nMoveDistance += pCopyFP->nNewSpace+nSpace;
			}
			else
			{
				pCopyFP->SetXY ((nFrmMaxW-pCtrlFPBase->pCompBase->nW)/2,nSpace/2+nMoveDistance,0);
				nMoveDistance += pCopyFP->nNewSpace+nSpace;
			}
		}
		else
		{
			if(bIsHoriz)
			{
				pCopyFP->SetXY (nSpace/2+nMoveDistance,(nFrmH-pCtrlFPBase->pCompBase->nH)/2,0);
				nMoveDistance += pCtrlFPBase->pCompBase->nW+nSpace;
			}
			else
			{
				pCopyFP->SetXY ((nFrmMaxW-pCtrlFPBase->pCompBase->nW)/2,nSpace/2+nMoveDistance,0);
				nMoveDistance += pCtrlFPBase->pCompBase->nH+nSpace;
			}
		}
	}
}

bool CCtrlFrame::SetFPCnt( int nCnt,bool bHoriz,bool bLoop )
{
	if (nCnt == pListFP->Count && bHoriz == bIsHoriz&&!bLoop)
	{
		return false;
	}
	if (nCnt*2 == pListFP->Count && bHoriz == bIsHoriz&&bLoop)
	{
		return false;
	}
	nChgX		= 0;
	nChgY		= 0;
	nSetX		= 0;
	nSetY		= 0;
	byChgFrom	= 0;
	bChged		= true;
	bIsHoriz	= bHoriz;
	this->bLoop = bLoop;
	while(pListFP->Count)
	{
		CCtrlFPart* pTmpFP	= (CCtrlFPart*)pListFP->Delete (0);
		g_pListFPartCache->Add(pTmpFP);
		pTmpFP->m_dwTick	= xnGetTickCount();
	}
	int nMoveDistance = 0;
	for (int i = 0; i < nCnt*(bLoop?2:1); i++)
	{
		CCtrlFPart*	pCopyFP	= NULL;
		if(i >= pListFP->Count)
		{
			pCopyFP	= new CCtrlFPart(pCtrlFPBase);
			pListFP->Add (pCopyFP);
		}
		else
			pCopyFP	= (CCtrlFPart*)pListFP->Items[i];

		if(pCopyFP->nNewSpace)
		{
			if(bHoriz)
			{
				pCopyFP->MoveXY (nSpace/2+nMoveDistance,(nFrmH-pCtrlFPBase->pCompBase->nH)/2,0);
				nMoveDistance += pCopyFP->nNewSpace+nSpace;
			}
			else
			{
				pCopyFP->MoveXY ((nFrmMaxW-pCtrlFPBase->pCompBase->nW)/2,nSpace/2+nMoveDistance,0);
				nMoveDistance += pCopyFP->nNewSpace+nSpace;
			}
		}
		else
		{
			if(bHoriz)
			{
				pCopyFP->MoveXY (nSpace/2+nMoveDistance,(nFrmH-pCtrlFPBase->pCompBase->nH)/2,0);
				nMoveDistance += pCtrlFPBase->pCompBase->nW+nSpace;
			}
			else
			{
				pCopyFP->MoveXY ((nFrmMaxW-pCtrlFPBase->pCompBase->nW)/2,nSpace/2+nMoveDistance,0);
				nMoveDistance += pCtrlFPBase->pCompBase->nH+nSpace;
			}
		}


		pCopyFP->nID		= i;
		pCopyFP->pParent	= this;
		pCopyFP->SetEvent (CBFramePClick,pCopyFP);
	}

	for (int i = pListFP->Count-1; i >= nCnt*(bLoop?2:1); i--)
	{
		CCtrlFPart* pTmpFP	= (CCtrlFPart*)pListFP->Delete (i);
		g_pListFPartCache->Add(pTmpFP);
		pTmpFP->m_dwTick	= xnGetTickCount();
	}

	return true;
}

void CCtrlFrame::SetEventAdv( void (*EventFunc)(InterfaceEvent* pEvent, int nPos,char* szName,void* param),void* pParam )
{
	fnEventFuncAdv	= EventFunc;
	m_pParam		= pParam;
}

void CCtrlFrame::SetEventEx( void (*EventFunc)(int nPos,char* szName,void* param) ,void* pParam)
{
	fnEventFuncEx	= EventFunc;
	m_pParam		= pParam;
}

void CCtrlFrame::SetEvent( void (*EventFunc)(int nPos,char* szName) )
{
	fnEventFunc		= EventFunc;
}

void CCtrlFrame::SetFPNewSpace(int nFPID,int nVal)
{
	if(nFPID >= pListFP->Count)
		return;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	pFP->nNewSpace	= nVal;
}
void CCtrlFrame::SetFPCompSpr(int nFPID, char* szName, char* szSpr )
{
	if(nFPID >= pListFP->Count)
		return;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	DComponent* pComp	= pFP->GetDComp (szName);
	if(pComp)
		pComp->SetViewSpr (szSpr);
}

bool CCtrlFrame::InitByForm( DForm* pForm,bool bHoriz/*=false*/ )
{
	bIsCreateByForm = true;
	bChged		= true;
	bIsHoriz	= bHoriz;
	DComponent* pBK = pForm->GetComponent("bk");
	if (pBK)
	{
		nFrmMaxH = pBK->nH;
		nFrmMaxW = pBK->nW;
	}
	
	while(pListFP->Count)
	{
		delete (CCtrlFPart*)pListFP->Delete (0);
	}

	for (int i = 0; i < pForm->pListComps->Count; i++)
	{
		DComponent* pCom = (DComponent*)pForm->pListComps->Items[i];
		CCtrlFPart*	pCopyFP	= new CCtrlFPart(pCom,NULL,0);
		pListFP->Add (pCopyFP);

		pCopyFP->MoveXY (pCom->nX,pCom->nY,0);

		pCopyFP->nID		= i;
		pCopyFP->pParent	= this;
		pCopyFP->SetEvent (CBFramePClick,pCopyFP);
	}

	return true;
}

void CCtrlFrame::SetFPCompCaption( int nFPID, char* szName, char* szCaption,BYTE bAlign )
{
	if(nFPID >= pListFP->Count)
		return;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	DComponent* pComp	= pFP->GetDComp (szName);
	if(pComp)
		pComp->SetCaption(szCaption,bAlign);
}

DComponent* CCtrlFrame::GetFPComp( int nFPID, char* szName )
{
	if(nFPID >= pListFP->Count)
		return NULL;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	DComponent* pComp	= pFP->GetDComp (szName);
	return pComp;
}

int CCtrlFrame::GetStepPos()
{
	if (pFrame)
	{
		FrameLayer* pFrm = (FrameLayer*)pFrame;
		if (bIsHoriz)
		{
			int nDelta = -pFrm->getPositionX()*POINT_PIX_CNT + nFrmX;
			return (nDelta/nFrmW + 1);
		}else
		{
			int nDelta = pFrm->getPositionY()*POINT_PIX_CNT + nFrmY;
			return (nDelta/nFrmH + 1);
		}
	}

	return 0;
}

//DComponent* CCtrlFrame::GetFPComp( char* szName )
//{
//	for (int i=0;i<pListFP->Count;i++)
//	{
//		CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[i];
//		if(strcmp (pFP->pCompBase->szName,szName) == 0)
//			return pFP->pCompBase;
//	}
//	return NULL;
//}

bool CCtrlFrame::SetFPTag( int nFPID,void* pTag )
{
	if(nFPID >= pListFP->Count)
		return false;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	pFP->m_pTag = pTag;
	return true;
}

void* CCtrlFrame::GetFPTag( int nFPID )
{
	if(nFPID >= pListFP->Count || nFPID < 0)
		return NULL;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	return pFP->m_pTag;
}

void CCtrlFrame::SetStepEvent(void (*StepEventFunc)(int nStep) )
{
	fnStepEventFunc		= StepEventFunc;
}

DComponent* CCtrlFrame::GetFPCompBase( int nFPID )
{
	if(nFPID >= pListFP->Count)
		return NULL;
	CCtrlFPart* pFP	= (CCtrlFPart*)pListFP->Items[nFPID];
	return pFP->pCompBase;
}


CCtrlProgress::CCtrlProgress( DComponent* pComp )
{
	pCompPro	= pComp;
	m_nCurVal	= 0;
	m_nMaxVal	= 1;
	m_nAddSID	= 0;
	m_dwSTick	= 0;
	m_dwRunTick	= 0;
	m_bHideTxt	= false;
}

CCtrlProgress::~CCtrlProgress()
{
	if(m_nAddSID)
	{
		g_pSoundCenter->StopEffect(m_nAddSID);
		m_nAddSID	= 0;
	}
}

void CCtrlProgress::SetProgress( int nVal, int nMax,int nTick )
{
	int	nRunTick	= 500;
	int		nChgVal	= nVal-m_nCurVal;
	if(nMax	!= m_nMaxVal)
		nChgVal	+=  m_nMaxVal;


	if(nChgVal > 1000)
		nRunTick	= 2000;
	else if(nChgVal >100)
		nRunTick	= 1000;
	if (nTick != -1)
	{
		nRunTick = nTick;
		if (nTick == 0)
		{
			m_nMaxVal = nMax;
			m_nCurVal = nVal;
			return;
		}
		
	}
	//m_nCurVal		= nVal;
	//if(nMax != m_nMaxVal)
	//	m_nMaxVal		= nMax;

	AddProgress(nChgVal,nMax,nRunTick);
}

void CCtrlProgress::AddProgress( int nVal, int nNextMax, int nTick )
{
 	if(m_nAddVal == nVal && m_nNextVal == nNextMax && m_dwSTick!=0)
 		return;
	m_dwRunTick	= nTick;
	m_dwSTick	= xnGetTickCount();
	m_nAddVal	= nVal;
	m_nNextVal	= nNextMax;
	//m_nAddSID	= g_pSoundCenter->PlayerEffect("Sound/Upgrade",true);

}

void CCtrlProgress::Update()
{
	if(m_dwSTick	== 0)
		return;

	char	szTmp[64];
	int		nRuned	= xnGetTickCount() - m_dwSTick;
	if(nRuned < m_dwRunTick)
	{
		int		nCurAdd	= m_nAddVal*nRuned/m_dwRunTick;

		if(m_nCurVal+ nCurAdd >= m_nMaxVal)
		{
			m_nCurVal	= m_nCurVal - m_nMaxVal;
			m_nMaxVal	= m_nNextVal;
		}

		if(m_bHideTxt)
			sprintf(szTmp, " ");
		else
			sprintf(szTmp, "%d/%d", m_nCurVal+nCurAdd,m_nMaxVal);
		pCompPro->SetProgress(m_nCurVal+nCurAdd, m_nMaxVal,szTmp);
	}
	else
	{
		m_dwSTick	= 0;
		g_pSoundCenter->StopEffect(m_nAddSID);
		m_nAddSID	= 0;
		m_nCurVal	+= m_nAddVal;
		if(m_nCurVal >= m_nMaxVal)
		{
			m_nCurVal	= m_nCurVal - m_nMaxVal;
			m_nMaxVal	= m_nNextVal;
		}
		
		if(m_bHideTxt)
			sprintf(szTmp, " ");
		else
			sprintf(szTmp, "%d/%d", m_nCurVal,m_nMaxVal);
		pCompPro->SetProgress(m_nCurVal, m_nMaxVal,szTmp);
	}
	
}

CCtrlItem::CCtrlItem( DComponent* pCom ):m_pCom(pCom),m_pItem(NULL)
{

}

void CCtrlItem::SetItem( ItemObj* pItem )
{

}
