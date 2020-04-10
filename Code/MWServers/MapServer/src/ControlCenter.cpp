#include "stdafx.h"

#include "ControlCenter.h"


ControlList*	g_pCtrlList	= NULL;

ControlList::ControlList()
{

}

ControlList::~ControlList()
{
	HMapCtrlObjPtr	itor	= mapControlers.begin();
	while(itor	!= mapControlers.end())
	{//新建
		delete itor->second;
		mapControlers.erase(itor++);
	}
}

void ControlList::RecvPacket( ZPacket* pPacket, DWORD dwKey )
{
	if(pPacket->bCmdGroup != MAPCMD)
		return;

	HMapCtrlObjPtr	itor	= mapControlers.find(dwKey);
	//ControlObj*	pCtrl	= NULL;
	//if(itor	== mapControlers.end())
	//{//新建
	//	pCtrl	= new ControlObj(dwKey);
	//	mapControlers[dwKey]	= pCtrl;
	//}
	//else
	//	pCtrl	= itor->second;

	if(itor	!= mapControlers.end())
		itor->second->RecvPacket(pPacket);
}

void ControlList::Update()
{
	HMapCtrlObjPtr	itor	= mapControlers.begin();
	while(itor	!= mapControlers.end())
	{//新建
		itor->second->Update();
		itor++;
	}
}

void ControlList::UpdateSec()
{

}

ControlObj* ControlList::GetCtrl( DWORD dwKey, bool bCreated /*= false*/ )
{
	HMapCtrlObjPtr	itor	= mapControlers.find(dwKey);
	ControlObj*	pCtrl	= NULL;
	if(itor	== mapControlers.end())
	{//新建
		if(bCreated	== false)
			return NULL;
		pCtrl	= new ControlObj(dwKey);
		mapControlers[dwKey]	= pCtrl;
	}
	else
		pCtrl	= itor->second;

	return pCtrl;
}

bool ControlList::RemoveCtrl( DWORD dwKey )
{
	HMapCtrlObjPtr	itor	= mapControlers.find(dwKey);
	if(itor != mapControlers.end())
	{
		delete itor->second;
		mapControlers.erase(itor);
		return true;
	}
	return false;
}

void ControlList::SendMapPacket( ZPacket* pPacket, int nMIndex )
{
	HMapCtrlObjPtr	itor	= mapControlers.begin();
	while(itor	!= mapControlers.end())
	{//新建
		if(itor->second->m_nMIndex == nMIndex)
			itor->second->SendPacket(pPacket);
		itor++;
	}
}

void ControlList::CheckCtrlRealseMap( void* pMap )
{
	HMapCtrlObjPtr	itor	= mapControlers.begin();
	while(itor	!= mapControlers.end())
	{//新建
		if(itor->second->m_pRunMap == pMap)
			itor->second->m_pRunMap	= NULL;
		itor++;
	}
}
