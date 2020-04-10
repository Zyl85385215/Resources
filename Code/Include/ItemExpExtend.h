#ifndef _ITEM_EXP_EXTEND_H
#define _ITEM_EXP_EXTEND_H

#include "item_packet.h"

class CItemExpExtend
{
public:
	CItemExpExtend();
	CItemExpExtend(CItemExpExtend& xExpExtend);
	~CItemExpExtend();

	void LoadItemExpExtendConfig(DWORD wItemId, BYTE bLevel);

	void SetExtendData(ExtendData& xExtendData);

	DWORD				m_dwExp;
	DWORD				m_dwNextExp;
	BYTE				m_bLevel;
	ExtendData *		m_pExtendData;
	DWORD				m_dwTempExp;
};

#endif