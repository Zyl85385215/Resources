#pragma once

#include "IOSThirdPlat.h"
//bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen);

char* IConvConvert_GBKToUTF8(const char* str);
char* IConvConvert_UTF8ToGBK(const char* str);

int		_mbsnextc(char* p);

int CalcDistance(int nX1,int nY1,int nX2,int nY2);
int CalcDistance2(int nX1,int nY1,int nX2,int nY2);
int CalcDirection(int nX1,int nY1,int nX2,int nY2, char bType);	//2  4方向 1 8方向
void GetBattleGPos( int& nX, int& nY,int nDestX, int nDestY, int nDir , int nPos);

unsigned long xnGetTickCount();
void*			xnGetDayTime();
unsigned long xnGetSecCount();		//1970年开始到现在的秒数

bool	AnalyDesc(char * szSrc, int nVal,const char * szClr, char * szOut);
int		GetValByChar(int nDefVal, int nChar, int nCharEx);
bool	gfun_ReplaceTxtName(char* szTxt, char* szOut);
bool	gfun_ReplaceStr(char* szDeal);
bool	gfun_CheckSprCache(char*	szFile,bool bCache);
void	gFun_ClearSpriteCache();
class CCostTime{
public:
	CCostTime();
	~CCostTime();
private:
	unsigned long ulTime;
};
extern	IosThirdPlat*	g_pTPObj;