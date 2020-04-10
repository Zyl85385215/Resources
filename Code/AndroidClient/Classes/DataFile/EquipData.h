#pragma once


#include "AmzDataBase.h"
#include "EquipConfig.h"
#include "xnlist.h"


class EquipObj
{
public:
	static	EquipObj* Create(AmzDataBase* pData);

	EquipObj(AmzDataBase* pData);
	~EquipObj();

	bool		Update();
	void		RecalcParam();
	int			GetVal(BYTE bEnum);
	//int			m_nParams[WARS_PARAM_MAX];
	EquipBase*		pEquipBase;
private:
	AmzDataBase*		pAmzData;

};

class EquipCenter
{
public:
	EquipCenter(AmzDataManager* pDC);
	~EquipCenter();

	void		Update();

	EquipObj*	GetEquipByID(int	nID);
	EquipObj*	GetEquipByPosSlot(int nPos,BYTE bSlot);

	bool		m_bChgNumber;
	xnList*				m_pListEquips;
private:
	
	AmzDataManager*		pDataCenter;
};