#pragma once 

#define HTTP_DLL_API __declspec(dllimport)

typedef void (*DownLoadFinish)(void* param);

//#pragma comment(lib,"DownLoadLib.lib")

class HTTP_DLL_API HttpDownload
{
public:
	bool DownLoadUrl(char* szUrl,char* path,char* szFullPath);

	void SetFinishCBFun(DownLoadFinish fnFinish,void* param);

	DownLoadFinish m_fnFinish;
	void*		   m_param;
};