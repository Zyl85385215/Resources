#pragma once

#include "dxalphachannel.h"

#ifdef _DEBUG
#pragma comment(lib, "dxalphachannel_d.lib")
#else
#pragma comment(lib, "dxalphachannel.lib")
#endif

class CDxAlphaChannel_Helper
{
public:
	CDxAlphaChannel_Helper(IDxAlphaChannel_Render * pRender = 0);
	~CDxAlphaChannel_Helper(void);

	HDXACINFO ParseFile(const char * szFile);
	HDXACINFO ParseMemory(const char * szFile, BYTE * pbyData, int nSize);

	HDXACINFO Load(const char * szFile, int nSize = 0);
	bool Save(HDXACINFO hInfo, const char * szFile);
	void Render(HDXACINFO hInfo, int nPosx, int nPosy, bool bTranslucent = false);
	void RenderStretch(HDXACINFO hInfo, int nPosx, int nPosy, int w, int h, bool bTranslucent = false);
	void ReleaseInfo(HDXACINFO hInfo);

	int GetWidth(HDXACINFO hInfo);
	int GetHeight(HDXACINFO hInfo);
	int GetCenterPosx(HDXACINFO hInfo);
	int GetCenterPosy(HDXACINFO hInfo);

	void SetCenterPos(HDXACINFO hInfo, int nPosx, int nPosy);

private:
	IDxAlphaChannelCenter * m_pDxAlphaChannelCenter;
};