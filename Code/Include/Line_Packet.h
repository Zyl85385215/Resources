#ifndef _MONITER_PACKET_H_
#define _MONITER_PACKET_H_
#define LINEMONITERCMD 70

enum
{
	GETINFO,
	SETINFO,//centerserver Info
	SETLINESTOP,
	GETLINESTOPRESULT,
};

#pragma pack (push)
#pragma pack (1)

struct LineInfo
{
	char szIP[16];
	char szName[256];
	WORD wIndex;
	int	 nCnt;
};
struct	LINEMONITERCMD_GETINFO_Data
{//设置获取的服务器INDEX
	int nIndex;
};
struct	LINEMONITERCMD_SETINFO_Data
{//从GetInfo中获取服务器在监控列表中的位置
	int			nIndex;
	char		szIP[16];
	char		szName[256];
	int			nTotal;
	int			nLineCnt;
	LineInfo    lineInfo[1];
};

struct LINEMONITERCMD_SETLINESTOP_Data
{
	int wIndex;
};

struct LINEMONITERCMD_GETLINESTOPRESULT_Data
{
	WORD wResult;
};

#pragma pack (pop)

#endif