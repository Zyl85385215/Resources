#include "EquipData.h"
#include "UserData.h"
#include "MainMenu.h"

EquipObj* EquipObj::Create( AmzDataBase* pData )
{
	EquipBase*		pEquipType	= EquipConfig::GetInstance ().GetEquipBase ((*pData)[EQUIP_AD_TYPE]);
	if(pEquipType == NULL)
	{
		pData->bStateFlag	= AMZ_STATE_RELEASE;
		return NULL;
	}
	EquipObj* pEquip	= new EquipObj(pData);

	pEquip->pEquipBase		= pEquipType;
	pEquip->RecalcParam ();
	return pEquip;
}

EquipObj::EquipObj( AmzDataBase* pData )
{
	pAmzData	= pData;
	pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
	//memset(m_nParams,0,WARS_PARAM_MAX*sizeof(int));
}

EquipObj::~EquipObj()
{
	pChar->m_pDataCenter->RemoveAmzData (pAmzData->bModelType,pAmzData->nModelID,true);
}

bool EquipObj::Update()
{
	if(pAmzData->bStateFlag == AMZ_STATE_DELETE)
	{
		return false;
	}

	if(pAmzData->bStateFlag	== AMZ_STATE_CHGED)
	{
		if((*pAmzData)[EQUIP_AD_TYPE] != pEquipBase->nTypeID)
			pEquipBase	= EquipConfig::GetInstance ().GetEquipBase ((*pAmzData)[EQUIP_AD_TYPE]);

		if(pEquipBase == NULL)
			return false;

		RecalcParam ();

		pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
	}

	return true;
}

void EquipObj::RecalcParam()
{
	//等级的基础属性

}

int EquipObj::GetVal( BYTE bEnum )
{
	if(bEnum >= EQUIP_AD_MAX)
		return 0;
	return (*pAmzData)[bEnum];
}

EquipCenter::EquipCenter( AmzDataManager* pDC )
{
	m_pListEquips	= xnList::Create ();
	pDataCenter		= pDC;
	m_bChgNumber	= false;
}

EquipCenter::~EquipCenter()
{
	while(m_pListEquips->Count)
		delete (EquipObj*)m_pListEquips->Delete (0);
	m_pListEquips->Free ();
}
static int SortEquip(const void * Item1, const void * Item2)
{
	EquipObj * pData1 = *(EquipObj **)Item1;
	EquipObj * pData2 = *(EquipObj **)Item2;
// 	if (pData1->GetVal(EQUIP_AD_SLOT)&&!pData2->GetVal(EQUIP_AD_SLOT))
// 	{
// 		return -100;
// 	}else if (!pData1->GetVal(EQUIP_AD_SLOT)&&pData2->GetVal(EQUIP_AD_SLOT))
// 	{
// 		return 100;
// 	}
	return pData2->pEquipBase->nStar-pData1->pEquipBase->nStar;
	//return (pData1->dwUin - pData2->dwUin);
}

void EquipCenter::Update()
{
	//检测船只数据,刷新
	for (int i = m_pListEquips->Count-1; i >= 0; i--)
	{
		EquipObj* pEquip	= (EquipObj*)m_pListEquips->Items[i];
		if(pEquip->Update () == false)
		{
			delete pEquip;
			m_pListEquips->Delete (i);
			//pMainMenu->m_pEquipMenu->m_bUpdate = true;
		}
	}

	//新增船只处理
	AmzDataBase* pAD	= pDataCenter->PopAmzData (AMZ_DATA_EQUIPEX);
	while(pAD)
	{
		ItemObj* pObj = pChar->m_pItemCenter->GetItemByIndex(pAD->GetVal(EQUIPEX_AD_ITEMID));
		if(pObj){
			pObj->SetExData(ITEMEXDATA_EQUIPEX,pAD);
			if (pChar->m_dwTickJoin && xnGetTickCount() -pChar->m_dwTickJoin>=10000){   
				pMainMenu->m_pFastEquipMenu->GetEquip(pObj);
			}
		}
		else{
			int aa=123;
		}
// 		EquipObj* pEquip	= EquipObj::Create(pAD);
// 		if(pEquip)
// 		{
// 			pMainToopTip->AddGetTipObj(pEquip->pEquipBase->szName,pEquip->pEquipBase->szDesc,pEquip->pEquipBase->nSpr,1,1);
// 
// 			m_pListEquips->Add (pEquip);
// 		}
// 		m_pListEquips->Sort(SortEquip);
		pAD		= pDataCenter->PopAmzData (AMZ_DATA_EQUIPEX);
	}
}

EquipObj* EquipCenter::GetEquipByID( int nID )
{
	for (int i = 0; i < m_pListEquips->Count; i++)
	{
		EquipObj* pEquip	= (EquipObj*)m_pListEquips->Items[i];
		if(pEquip->GetVal(EQUIP_AD_ID) == nID)
			return pEquip;
	}
	return NULL;
}

EquipObj* EquipCenter::GetEquipByPosSlot( int nPos,BYTE bSlot )
{
	for (int i = 0; i < m_pListEquips->Count; i++)
	{
		EquipObj* pEquip	= (EquipObj*)m_pListEquips->Items[i];
		if(bSlot == 0 || bSlot == pEquip->pEquipBase->bSlot)
		{
			if(nPos == 0)
				return pEquip;

			nPos--;
		}
	}
	return NULL;
}