#pragma once

#include <Windows.h>
#include <list>

#define DXALPHACHANNEL_VERSION 0x101215

typedef DWORD HDXACINFO;

class IDxAlphaChannel_Render
{
public:
	virtual void Render(BYTE bAlpha, int nPosx, int nPosy, int nLen, bool bTranslucent = false) = 0;
	virtual RECT GetScrClip() = 0;
};

class IDxAlphaChannelParser
{
public:
	virtual void Release(void) = 0;

	virtual bool ParseFile(const char * szFile) = 0;
	virtual bool ParseMemory(BYTE * pbyData, int nSize) = 0;

	virtual BYTE GetAlpha(int nPosx, int nPosy) = 0;
	virtual int GetWidth(void) = 0;
	virtual int GetHeight(void) = 0;
	virtual int GetCenterPosx(void) = 0;
	virtual int GetCenterPosy(void) = 0;

	virtual IDxAlphaChannelParser * CreateInstance(void) = 0;
};

class IDxAlphaChannelCenter
{
public:
	virtual void Release(void) = 0;

	virtual void AddParser(const char * szFormat, IDxAlphaChannelParser * pParser) = 0;			//	��ʽ������
	virtual HDXACINFO ParseFile(const char * szFile) = 0;										//	֧�ֶ��ָ�ʽ
	virtual HDXACINFO ParseMemory(const char * szFile, BYTE * pbyData, int nSize) = 0;			//	֧�ֶ��ָ�ʽ

	virtual HDXACINFO Load(const char * szFile, int nSize = 0) = 0;								//	����dxac�ļ�
	virtual bool Save(HDXACINFO hInfo, const char * szFile) = 0;								//	����dxac�ļ�
	virtual void Render(HDXACINFO hInfo, int nPosx, int nPosy, bool bTranslucent) = 0;			//	����dxac�ļ�
	virtual void RenderStretch(HDXACINFO hInfo, int nPosx, int nPosy, int w, int h, bool bTranslucent) = 0;
	virtual void ReleaseInfo(HDXACINFO hInfo) = 0;

	virtual int GetWidth(HDXACINFO hInfo) = 0;
	virtual int GetHeight(HDXACINFO hInfo) = 0;
	virtual int GetCenterPosx(HDXACINFO hInfo) = 0;
	virtual int GetCenterPosy(HDXACINFO hInfo) = 0;

	virtual void SetCenterPos(HDXACINFO hInfo, int nPosx, int nPosy) = 0;
};

IDxAlphaChannelCenter * CreateDxAlphaChannelCenter(int nVersion, IDxAlphaChannel_Render * pRender);