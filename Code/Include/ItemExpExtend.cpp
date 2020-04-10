#include "itemexpextend.h"
#include "ItemExpExtendConfig.h"

CItemExpExtend::CItemExpExtend()
{
	m_bLevel			= 0;
	m_dwExp				= 0;
	m_dwNextExp			= 0;
	m_dwTempExp			= 0;
	m_pExtendData		= NULL;
}

CItemExpExtend::CItemExpExtend(CItemExpExtend& xExpExtend)
{
	m_bLevel			= xExpExtend.m_bLevel;
	m_dwExp				= xExpExtend.m_dwExp;
	m_dwNextExp			= xExpExtend.m_dwNextExp;
	m_dwTempExp			= xExpExtend.m_dwTempExp;

	if(xExpExtend.m_pExtendData)
	{
		m_pExtendData= new ExtendData;
		memcpy(m_pExtendData, xExpExtend.m_pExtendData, sizeof(ExtendData));
	}
	else
	{
		m_pExtendData= NULL;
	}
}

CItemExpExtend::~CItemExpExtend()
{
	SAFE_DELETE(m_pExtendData);
}

void CItemExpExtend::LoadItemExpExtendConfig(DWORD wItemId, BYTE bLevel)
{
	CItemExpExtendConfig * pItemExpExtendConfig = CItemExpExtendConfig::GetInstance();
	ItemExpExtendInfo xExtendInfo;

	//��õ�ǰ�ȼ������Լӳ�
	if(pItemExpExtendConfig->GetExpExtendInfo(wItemId, bLevel, xExtendInfo))
	{
		SetExtendData(xExtendInfo.xExtendData);
	}

	//�����������һ������Ҫ�ľ���
	if(!pItemExpExtendConfig->GetExpExtendInfo(wItemId, bLevel+1, xExtendInfo))
	{
		//0�ͱ�ʾ�޷�����
		m_dwNextExp = 0;
		m_dwExp		= 0;
	}
	else
	{
		m_dwNextExp = xExtendInfo.dwExp;
	}
}

void CItemExpExtend::SetExtendData(ExtendData& xExtendData)
{
	if(!m_pExtendData)
	{
		m_pExtendData	= new ExtendData;
	}
	memcpy(m_pExtendData, &xExtendData, sizeof(ExtendData));
}
