#include "stdafx.h"

#include "NASaleCenter.h"
#include "DBControl.h"
#include "TimeControl.h"
#include "UserData.h"
#include "ios_nasale_packet.h"
#include "FsGameCenter.h"
#include "ItemManage.h"
#include "MailCenter.h"

NASaleCenter::NASaleCenter()
{
	m_dwFilterMaxCnt = 0;
	m_dwMaxNAIndex	= 1;
	m_pListRequests	= xnList::Create();
	m_pListSales	= xnList::Create();
}

NASaleCenter::~NASaleCenter()
{
	while(m_pListSales->Count)
		delete (NASaleObj*)m_pListSales->Delete(0);
	m_pListSales->Free();
	while(m_pListRequests->Count)
		delete (NASaleObj*)m_pListRequests->Delete(0);
	m_pListRequests->Free();
}

void NASaleCenter::DB_Init(DBControl* pDB)
{
	m_pDB	= pDB;
	m_pDB		= pDB;

	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_NASALE_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		NASaleObj*		pSale	= new NASaleObj();
		pSale->dwNAIndex		= pCmd->GetDWORD("naindex");
		if(pSale->dwNAIndex >= m_dwMaxNAIndex)
			m_dwMaxNAIndex	= pSale->dwNAIndex+1;
		pSale->dwFUin			= pCmd->GetDWORD("uinfrom");
		pSale->byType			= pCmd->GetDWORD("type");
		pSale->dwStartTickSec	= pCmd->GetDWORD("startsec");
		pSale->nPrice			= pCmd->GetDWORD("price");

		pSale->wItemID			= pCmd->GetInt("itemid");
		pSale->wItemCnt			= pCmd->GetInt("itemcnt");
		pSale->dwItemInfo		= pCmd->GetInt("iteminfo");
		pSale->pBase			= ItemConfig::GetInstance().GetItemBase(pSale->wItemID);
		if(pSale->byType == 0)
			m_pListSales->Add(pSale);
		else
			m_pListRequests->Add(pSale);
	}
}

void NASaleCenter::UpdateMinute()
{
	DWORD	dwCurSec	= CMTime::GetCurTickCount();
	for (int i = m_pListSales->Count-1; i >=0; i--)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListSales->Items[i];
		if(dwCurSec - pSale->dwStartTickSec > 86400)
		{//拍卖超时
			pSale->byType	= 9;
			_updateDB(pSale);
			m_pListRequests->Add(pSale);
			m_pListSales->Delete(i);
		}
	}
}

DWORD NASaleCenter::RequestItem( WORD wItem, DWORD dwUin, int nPrice,WORD wCnt )
{


	return 0;
}

bool NASaleCenter::AnswerItem( UserData* pAnswer, DWORD dwNAIndex,ItemObj* pItem,WORD wCnt )
{


	return true;
}

DWORD NASaleCenter::AddNewSale( ItemObj* pItem, DWORD dwUin, int nPrice )
{
	NASaleObj*	pSale	= new NASaleObj();
	pSale->byType		= 0;
	pSale->dwFUin		= dwUin;
	pSale->nPrice		= nPrice;
	if(pItem->m_pBase->byType == ITEMTYPE_EQUIP)
	{

		pSale->dwItemInfo	= g_pGameCenter->m_pItemManage->AddItemObj(pItem,true);
		pSale->wItemCnt		= 1;
	}
	else
	{
		pSale->dwItemInfo	= 0;
		pSale->wItemCnt		= pItem->GetDataVal(ITEM_AD_NUM);
	}
	pSale->dwNAIndex	= m_dwMaxNAIndex++;
	pSale->dwStartTickSec	= CMTime::GetCurTickCount();
	pSale->wItemID		= pItem->GetItemID();
	
	pSale->pBase		= pItem->m_pBase;
	m_pListSales->Add(pSale);

	_insertDB(pSale);
	return pSale->dwNAIndex;
}

bool NASaleCenter::RqBuyItem(UserData* pBuyer, DWORD dwNAIndex,WORD wCnt )
{
	if(wCnt == 0)
		return false;
	for (int i = 0; i < m_pListSales->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListSales->Items[i];
		if(pSale->dwNAIndex == dwNAIndex)
		{//拍卖超时

			if(pSale->dwFUin == pBuyer->m_dwUin)
			{
				pBuyer->SendWarnMsg("不可购买自己的物品");
				return false;
			}

			if( pSale->byType == 0 )
			{
				if(wCnt != pSale->wItemCnt)	//只能整组买卖
					return false;
				if(pSale->nPrice > pBuyer->m_xUWealth[UW_GOLD])
					return false;
				//给玩家扣钱直接发物品,给卖家发钱的邮件
				pBuyer->m_xUWealth.SubWealth(UW_GOLD,pSale->nPrice);
				//g_pGameCenter->m_pMailCenter->RequestSendMail(pSale->dwFUin,pSale->dwFUin,"寄售邮件","获得金钱",pSale->nPrice*wCnt);
				int nSalItemID = 0;
				if(pSale->dwItemInfo)
				{//装备
					ItemObj* pItem = g_pGameCenter->m_pItemManage->RemoveItemObj(pSale->dwItemInfo,true);
					if (pItem)
					{
						nSalItemID = pItem->GetItemID();

						if (pBuyer->m_pItemCenter->GetBlankBag() == 0)
						{
							pBuyer->SendWarnMsg("包裹已满，所购装备将通过邮件发送给您！");
							int nItemIndex[4] = {0,0,0,0};
							nItemIndex[0] = g_pGameCenter->m_pItemManage->AddItemObj(pItem,true);
							g_pGameCenter->m_pMailCenter->RequestSendMail(0,pBuyer->m_dwUin,"寄售邮件","寄售行购买物品签收！",0,nItemIndex);
						}
						else
						{
							g_pGameCenter->m_pItemManage->SetItemOwner(pBuyer,pItem,1);
							pBuyer->m_pItemCenter->AddItem(pItem);
						}
					}
				}
				else
				{
					nSalItemID = pSale->wItemID;
					pBuyer->m_pItemCenter->AddItemByID(pSale->wItemID,wCnt,0,ITEM_LOG_NASALE);
				}

				UserData* pOwner	= g_pGameCenter->GetPlayer(pSale->dwFUin);
				if (pOwner)
				{
					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nSalItemID);
					if (pBase)
					{
						char szTmp[128];
						sprintf(szTmp,"%s购买了您寄售的%s",pBuyer->m_szNick,pBase->szName);
						pOwner->SendWarnMsg("%s",szTmp);
						pOwner->SendMsg("%s",szTmp);
					}
				}
				//pSale->wItemCnt -= wCnt;

				pSale->dwItemInfo	= 0;
				pSale->byType		= 8;		//已售

				//if(pSale->wItemCnt == 0)
				//{
				//	m_pListSales->Delete(i);
				//	_removeDB(pSale->dwNAIndex);
				//	delete pSale;
				//	//删除itemmanager
				//}
				//else
				{
					_updateDB(pSale);
				}

				return true;
			}
			else
				return false;
		}
	}

	return false;
}
bool NASaleCenter::RqGetMyItem(UserData* pBuyer, DWORD dwNAIndex )
{
	if(pBuyer->m_pItemCenter->GetBlankBag() == 0)
	{
		pBuyer->SendWarnMsg("包裹已满!");
		return false;
	}
	for (int i = 0; i < m_pListSales->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListSales->Items[i];
		if(pSale->dwNAIndex == dwNAIndex && pSale->byType != 8)
		{//拍卖超时

			if(pSale->dwFUin == pBuyer->m_dwUin)
			{
				if(pSale->dwItemInfo)
				{//装备
					ItemObj* pItem = g_pGameCenter->m_pItemManage->RemoveItemObj(pSale->dwItemInfo,true);
					if (pItem)
					{
						g_pGameCenter->m_pItemManage->SetItemOwner(pBuyer,pItem,1);
						pBuyer->m_pItemCenter->AddItem(pItem);
					}
				}
				else
				{
					pBuyer->m_pItemCenter->AddItemByID(pSale->wItemID,pSale->wItemCnt,0,ITEM_LOG_NASALE_LONGTIME);
				}
			}


			m_pListSales->Delete(i);
			_removeDB(pSale->dwNAIndex);
			delete pSale;

			return true;
		}
	}
	for (int i = 0; i < m_pListRequests->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListRequests->Items[i];
		if(pSale->dwNAIndex == dwNAIndex && pSale->byType == 9)
		{//拍卖超时

			if(pSale->dwFUin == pBuyer->m_dwUin)
			{
				if(pSale->dwItemInfo)
				{//装备
					ItemObj* pItem = g_pGameCenter->m_pItemManage->RemoveItemObj(pSale->dwItemInfo,true);
					if (pItem)
					{
						g_pGameCenter->m_pItemManage->SetItemOwner(pBuyer,pItem,1);
						pBuyer->m_pItemCenter->AddItem(pItem);
							
					}
				}
				else
				{
					pBuyer->m_pItemCenter->AddItemByID(pSale->wItemID,pSale->wItemCnt,0,ITEM_LOG_NASALE_LONGTIME);
				}
			}


			m_pListRequests->Delete(i);
			_removeDB(pSale->dwNAIndex);
			delete pSale;

			return true;
		}
	}
	return false;
}

void NASaleCenter::CalcMySale(UserData* pBuyer)
{
	int nTotalMoney	= 0;
	for (int i = 0; i < m_pListSales->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListSales->Items[i];
		if(pSale->byType == 8 && pSale->dwFUin == pBuyer->m_dwUin)
		{
			nTotalMoney	+= pSale->nPrice;

			m_pListSales->Delete(i);
			_removeDB(pSale->dwNAIndex);
			delete pSale;
			i--;
		}
	}
	for (int i = 0; i < m_pListRequests->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListRequests->Items[i];
		if(pSale->byType == 8 && pSale->dwFUin == pBuyer->m_dwUin)
		{
			nTotalMoney	+= pSale->nPrice;

			m_pListRequests->Delete(i);
			_removeDB(pSale->dwNAIndex);
			delete pSale;
			i--;
		}
	}
	if(nTotalMoney)
	{
		int nRevenue = nTotalMoney*5/100;
		pBuyer->SendMsg("收取寄售税额%d金币",nRevenue);
		int nCanGetMonet = nTotalMoney - nRevenue;
		pBuyer->m_xUWealth.AddWealth(UW_GOLD,nCanGetMonet,1,NULL,UW_GOLD_NASALE);
	}
}

int NASaleCenter::RqViewMyItem( BYTE bNAType,DWORD dwUin, xnList* pOutList )
{
	pOutList->Clear();
	for (int i = 0; i < m_pListSales->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListSales->Items[i];
 		if(pSale->byType != 9 && pSale->byType != 8 && pSale->byType != bNAType)
 			continue;
		if(pSale->dwFUin == dwUin)
		{//拍卖超时
			pOutList->Add(pSale);
		}
	}
	for (int i = 0; i < m_pListRequests->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListRequests->Items[i];
		if(pSale->dwFUin == dwUin)
		{//拍卖超时
			pOutList->Add(pSale);
		}
	}
	return pOutList->Count;
}

int NASaleCenter::RqViewItem( BYTE bNAType, BYTE bType, BYTE bJob,BYTE bLvSel, int nStartPos, xnList* pOutList )
{
	pOutList->Clear();

	m_dwFilterMaxCnt = 0;
	int nRmCnt	= 30;
	int	nRtl	= 0;
	for (int i = 0; i < m_pListSales->Count; i++)
	{
		NASaleObj* pSale	= (NASaleObj*)m_pListSales->Items[i];

		if(pSale->byType != bNAType)
			continue;
		
		if (bJob && (!(pSale->pBase->nJob&(1<<bJob))))
		{
			continue;
		}
		if (bType)
		{
			if (bType == 20)
			{
				if (pSale->pBase->byType!=0&&pSale->pBase->byType!=1)
					continue;
			}else{
				if (bType != pSale->pBase->byType)
					continue;
			}
		}
		if(pSale->byType != 0)
			continue;
		if(bLvSel != 0)
		{
			if(pSale->pBase->byLv < (bLvSel-1)*10 || pSale->pBase->byLv >= bLvSel*10)
				continue;
		}
		if (i > nStartPos-1&&nRmCnt > 0)
		{
			nRmCnt--;
			pOutList->Add(pSale);
			if(nRmCnt == 0)
			{
				nRtl	= i;
			}
		}
		m_dwFilterMaxCnt++;
	}

	return nRtl;
}

bool NASaleCenter::RecvPacket( ZPacket* pPacket,DWORD	dwUin )
{
	if(pPacket->bCmdGroup != NASALE)
		return false;

	switch(pPacket->bCmd)
	{
	case NA_CALC_MYMONEY:
		{
			UserData* pBuyer	= g_pGameCenter->GetPlayer(dwUin);
			if(pBuyer)
			{
				CalcMySale(pBuyer);
			}
		}
		return true;
	case NA_MY_LIST:
		{
			SET_DATA(pData, NASALE, NA_MY_LIST, pPacket);
			xnList*	pList	= xnList::Create();
			int nEndPos	= RqViewMyItem(0,dwUin,pList);

			BuildPacketEx(pSendPacket, NASALE, NA_MY_LIST, buf, 4096);
			SET_DATA(pSend, NASALE, NA_MY_LIST, pSendPacket);
			pSend->wCnt		= pList->Count;
			DWORD	dwCurSec	= CMTime::GetCurTickCount();
			for (int i = 0; i < pList->Count; i++)
			{
				NASaleObj* pSale	= (NASaleObj*)pList->Items[i];
				pSend->szData[i].byType	= pSale->byType;
				pSend->szData[i].dwNaKey	= pSale->dwNAIndex;
				pSend->szData[i].wItemID	= pSale->wItemID;
				pSend->szData[i].nPrice		= pSale->nPrice;
				pSend->szData[i].wCnt		= pSale->wItemCnt;
				pSend->szData[i].dwExInfo	= pSale->dwItemInfo;
				pSend->szData[i].nRmSec		= pSale->dwStartTickSec+86400-dwCurSec;
			}

			pSendPacket->wSize	+= sizeof(NAItem_Data)*pSend->wCnt;
			TCP_SendPacketToUin(dwUin,pSendPacket);

			pList->Free();
		}
		return true;
	case NA_SEARCH_SELL:
		{
			SET_DATA(pData, NASALE, NA_SEARCH_SELL, pPacket);
			xnList*	pList	= xnList::Create();
			int nEndPos	= RqViewItem(0,pData->bType,pData->bJob,pData->bLvStep,pData->nStartPos,pList);
			
			BuildPacketEx(pSendPacket, NASALE, NA_SELL_LIST, buf, 4096);
			SET_DATA(pSend, NASALE, NA_SELL_LIST, pSendPacket);
			pSend->nEndPos	= nEndPos;
			pSend->wCnt		= pList->Count;
			pSend->wPageCnt		= m_dwFilterMaxCnt>0?(m_dwFilterMaxCnt-1)/30:0;
			DWORD	dwCurSec	= CMTime::GetCurTickCount();
			for (int i = 0; i < pList->Count; i++)
			{
				NASaleObj* pSale	= (NASaleObj*)pList->Items[i];
				pSend->szData[i].byType	= pSale->byType;
				pSend->szData[i].dwNaKey	= pSale->dwNAIndex;
				pSend->szData[i].wItemID	= pSale->wItemID;
				pSend->szData[i].nPrice		= pSale->nPrice;
				pSend->szData[i].wCnt		= pSale->wItemCnt;
				pSend->szData[i].dwExInfo	= pSale->dwItemInfo;
				pSend->szData[i].nRmSec		= pSale->dwStartTickSec+86400-dwCurSec;
			}

			pSendPacket->wSize	+= sizeof(NAItem_Data)*pSend->wCnt;
			TCP_SendPacketToUin(dwUin,pSendPacket);

			pList->Free();
		}
		return true;
	case NA_BUY_ITEM:
		{
			SET_DATA(pData, NASALE, NA_BUY_ITEM,pPacket);
			UserData* pBuyer	= g_pGameCenter->GetPlayer(dwUin);
			if(pBuyer)
			{
				if(pData->wCnt)
					RqBuyItem(pBuyer,pData->dwNAIndex,pData->wCnt);
				else
				{//取回自己物品
					RqGetMyItem(pBuyer,pData->dwNAIndex);
				}
			}

		}
		return true;
	case NA_SELL_ITEM:
		{
			SET_DATA(pData, NASALE, NA_SELL_ITEM,pPacket);
			if(pData->nPrice <= 0)
				return true;
			UserData*	pUser	= g_pGameCenter->GetPlayer(dwUin);
			if(pUser)
			{
				ItemObj*	pItem	= pUser->m_pItemCenter->GetItemByIndex(pData->dwIndex);
				if(pItem && pItem->GetDataVal(ITEM_AD_BIND)==0)
				{
					ItemObj*	pItem1 =pUser->m_pItemCenter->CutItemByIndex(pItem,pItem->m_pBase->byType == ITEMTYPE_EQUIP?1:pItem->GetDataVal(ITEM_AD_NUM));
					AddNewSale(pItem1,dwUin,pData->nPrice);
					if (pItem1->m_pBase->GetItemFlag(ITEM_FLAG_LOG))
					{
						int nItemNum = pItem1->m_pBase->byType == ITEMTYPE_EQUIP?1:pItem1->GetDataVal(ITEM_AD_NUM);
						pUser->m_pUserRecord->AddItemLog(pItem1->m_pBase->nTypeID,nItemNum*(-1),ITEM_LOG_NASALE_NEW);
					}
					if (pItem1->m_pBase->byType != ITEMTYPE_EQUIP)
					{
						pItem1->Release();
					}
					//pUser->m_pItemCenter->RemoveItemByIndex(pData->dwIndex);
				}
			}
		}
		return true;
	case NA_SELL_ITEM_EX:
		{
			SET_DATA(pData, NASALE, NA_SELL_ITEM_EX,pPacket);
			if(pData->nPrice <= 0)
				return true;
			UserData*	pUser	= g_pGameCenter->GetPlayer(dwUin);
			if(pUser)
			{
				ItemObj*	pItem	= pUser->m_pItemCenter->GetItemByIndex(pData->dwIndex);
				if(pItem && pItem->GetDataVal(ITEM_AD_BIND)==0)
				{
					ItemObj*	pItem1 =pUser->m_pItemCenter->CutItemByIndex(pItem,pItem->m_pBase->byType == ITEMTYPE_EQUIP?1:pData->nCount);
					AddNewSale(pItem1,dwUin,pData->nPrice);
					if (pItem1->m_pBase->GetItemFlag(ITEM_FLAG_LOG))
					{
						int nItemNum = pItem1->m_pBase->byType == ITEMTYPE_EQUIP?1:pItem1->GetDataVal(ITEM_AD_NUM);
						pUser->m_pUserRecord->AddItemLog(pItem1->m_pBase->nTypeID,nItemNum*(-1),ITEM_LOG_NASALE_NEW);
					}
					if (pItem1->m_pBase->byType != ITEMTYPE_EQUIP)
					{
						pItem1->Release();
					}
					//pUser->m_pItemCenter->RemoveItemByIndex(pData->dwIndex);
				}
			}
		}
		return true;
	}

	return true;
}

void NASaleCenter::_removeDB( DWORD dwNaIndex )
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_NASALE_TABLE" where naindex=%d",dwNaIndex);
}

void NASaleCenter::_insertDB( NASaleObj* pNAObj )
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_NASALE_TABLE" (naindex, type,uinfrom, startsec, price, itemid,itemcnt,iteminfo) values (%d,%d, %d, %d, %d,%d,%d,%d)", 
		pNAObj->dwNAIndex, pNAObj->byType,pNAObj->dwFUin,pNAObj->dwStartTickSec,pNAObj->nPrice,pNAObj->wItemID,pNAObj->wItemCnt,pNAObj->dwItemInfo);
}

void NASaleCenter::_updateDB( NASaleObj* pNAObj )
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_NASALE_TABLE" set itemcnt=%d,iteminfo=%d,type=%d where naindex=%d", pNAObj->wItemCnt,pNAObj->dwItemInfo,pNAObj->byType,pNAObj->dwNAIndex);

}
