#include "StateData.h"
#include "MapAIObj.h"

StateCenter::StateCenter(MapAIObj * pObj)
{
	m_pListState	= xnList::Create();
	m_bUnVisable	= false;
	m_pObj			= pObj;
}

StateCenter::~StateCenter()
{
	m_pListState->Free();

}

void StateCenter::AddState( WORD wState )
{
	StateBase*	pBase	= StateConfig::GetInstance().GetStateBase(wState);

	if(!pBase)
		return;
	if(m_pListState->IndexOf(pBase) == -1)
	{
		m_pListState->Add(pBase);
		m_pObj->AddEff(pBase->wEff);
		m_pObj->AddEff(pBase->wEffex);
	}
}

void StateCenter::RemoveState( WORD wState )
{
	for (int i = 0; i < m_pListState->Count; i++)
	{
		StateBase*	pBase	= (StateBase*)m_pListState->Items[i];
		if(pBase->wStateID == wState)
		{
			m_pListState->Delete(i);
			m_pObj->RemoveEff(pBase->wEff);
			m_pObj->RemoveEff(pBase->wEffex);
			return;
		}
	}
}

bool StateCenter::CheckState(WORD wState)
{
	for (int i = 0; i < m_pListState->Count; i++)
	{
		StateBase*	pBase	= (StateBase*)m_pListState->Items[i];
		if(pBase->wStateID == wState)
			return true;
	}
	return false;
}

void StateCenter::ClearState()
{
	while(m_pListState->Count)
	{
		StateBase*	pBase	= (StateBase*)m_pListState->Delete(0);
		m_pObj->RemoveEff(pBase->wEff);
		m_pObj->RemoveEff(pBase->wEffex);
	}
}

void StateCenter::Update()
{
	m_bUnVisable	= false;
	for (int i = 0; i < m_pListState->Count; i++)
	{
		StateBase*	pBase	= (StateBase*)m_pListState->Items[i];

		if(pBase->szbSpEff[7])
			m_bUnVisable	= true;
	}
}
