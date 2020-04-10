#include "PacketCenter.h"
#include "GlobalFunction.h"
#include "string"
#include <stdlib.h>
#ifdef WIN32
#include "MainMenu.h"
#include ".\iconv\iconv.h"
#include <io.h>
#else
#include "iconv.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//DJ
   #include "platform/platform.h"
#else
#include "platform.h"
#endif
#include "UserData.h"
#include <math.h>

using namespace cocos2d;

#ifdef WIN32
int		g_nDFScreenWidth	= 960;
int		g_nDFScreenHeight	= 640;
int		g_nDFScreenPix	= 1;
#else
int		g_nDFScreenWidth	= 480;
int		g_nDFScreenHeight	= 320;
int		g_nDFScreenPix	= 2;
#endif
int nowScreenHeight=640;
int nowScreenWidth=960;
float nowRateW=0.0f;
float nowRateH=0.0f;
IosThirdPlat*	g_pTPObj	= NULL;
BYTE	g_byWarSpeed	= 1;
BYTE	g_bySvrLockClient	= 0;
int     g_nClientAreaLock   = -1;

bool IConvConvert( const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen )
{
	iconv_t cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return false;
#ifdef WIN32
	const char **pin = &inbuf;
#else
	char **pin = (char**)&inbuf;
#endif
	
	char **pout = &outbuf;
	memset(outbuf,0,outlen);
	size_t ret = iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
	iconv_close(cd);
	return ret == (size_t)(-1) ? false : true;
}

char* IConvConvert_GBKToUTF8( const char* str )
{
	static char textOut[4096];//gb2312
	bool ret = IConvConvert("GBK", "utf-8", str, strlen(str),textOut, 4096);
	return 1 ? textOut:(char*)str;
}


int _mbsnextc( char* p )
{
	int nRtl	= 0;
	if(p[0] > 0x80 && p[0] != '\0')
	{
		nRtl +=	 p[0]<< 8;
		p++;
	}
	nRtl	+= p[0];
	return nRtl;
}		

char* IConvConvert_UTF8ToGBK( const char* str )
{
	static char textOut[4096];
	bool ret = IConvConvert( "utf-8","GBK", str, strlen(str),textOut, 4096);
	return 1 ? textOut:(char*)str;
}

bool gfun_ReplaceStr( char* szDeal )
{
	char* szError	= strchr(szDeal,'%');
	while(szError)
	{
		szError[0]	= '#';
		szError++;
		if(szError == 0)
			break;
		szError	= strchr(szError,'%');
	}

	return true;
}

int CalcDistance(int nX1,int nY1,int nX2,int nY2)
{
	return (int)sqrt((float)((nX1-nX2)*(nX1-nX2)+(nY1-nY2)*(nY1-nY2)));
}

int CalcDistance2(int nX1,int nY1,int nX2,int nY2)
{
	return (nX1-nX2)*(nX1-nX2)+(nY1-nY2)*(nY1-nY2);
}

unsigned long xnGetTickCount()
{
	static struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now,NULL);
	return now.tv_sec*1000+now.tv_usec/1000;
}

int	_calc360Direction(int nX1,int nY1,int nX2,int nY2)
{
	int dx = nX2 - nX1;
	int dy = nY2 - nY1;
	if ((dx == 0) && (dy == 0))
		return 180;
	if (dx != 0)
	{
		double r = atan2((float)dy, (float)dx);
		int angle;
		if (r >= 0.0)
			angle = (int)(180.0f * r / 3.141593f + 22.5f);
		else
			angle = (int)(180.0f + 180.0f * (3.141593f + r) / 3.141593f + 22.5f);
		return (angle+68)%360;
	}
	else
	{
		if (dy >= 0)
			return 180;
		else 
			return 0;
	}
	return 0;
}

int	_calc8Direction(int nX1,int nY1,int nX2,int nY2)
{
	int dx = nX2 - nX1;
	int dy = nY2 - nY1;
	if ((dx == 0) && (dy == 0))
		return 4;
	if (dx != 0)
	{
		double r = atan2((float)dy, (float)dx);
		int angle;
		if (r >= 0.0)
			angle = (int)(180.0f * r / 3.141593f + 22.5f);
		else
			angle = (int)(180.0f + 180.0f * (3.141593f + r) / 3.141593f + 22.5f);
		return ((angle / 45 + 2) % 8);
	}
	else
	{
		if (dy >= 0)
			return 4;
		else 
			return 0;
	}
	return 0;
}

int	_calc4Direction(int nX1,int nY1,int nX2,int nY2)
{
	int dx = nX2 - nX1;
	int dy = nY2 - nY1;
	if ((dx == 0) && (dy == 0))
		return 4;
	int nRtl	= 3;
	if (dx != 0)
	{
		double r = atan2((float)dy, (float)dx);
		int angle;
		if (r >= 0.0)
			angle = (int)(180.0f * r / 3.141593f + 45);
		else
			angle = (int)(180.0f + 180.0f * (3.141593f + r) / 3.141593f + 45);
		nRtl	= ((angle / 90 + 1) % 4);
	}
	else
	{
		if (dy >= 0)
			return 4;
		else 
			return 0;
	}
	if(nRtl == 1)
		nRtl	= 2;
	else if(nRtl == 2)
		nRtl	= 4;
	else if(nRtl == 3)
		nRtl	= 6;

	return nRtl;
}

int	_calc4LeanDirection(int nX1,int nY1,int nX2,int nY2)
{
	int dx = nX2 - nX1;
	int dy = nY2 - nY1;
	if ((dx == 0) && (dy == 0))
		return 5;
	int nRtl	= 1;
	if (dx != 0)
	{
		double r = atan2((float)dy, (float)dx);
		int angle;
		if (r >= 0.0)
			angle = (int)(180.0f * r / 3.141593f + 45);
		else
			angle = (int)(180.0f + 180.0f * (3.141593f + r) / 3.141593f + 45);
		nRtl	= ((angle / 90 + 1) % 4);
	}
	else
	{
		if (dy >= 0)
			return 5;
		else 
			return 1;
	}
	if(nRtl == 0)
		nRtl = 1;
	if(nRtl == 1)
		nRtl	= 3;
	else if(nRtl == 2)
		nRtl	= 5;
	else if(nRtl == 3)
		nRtl	= 7;

	return nRtl;
}

int CalcDirection( int nX1,int nY1,int nX2,int nY2, char bType )
{
	if(bType == 2  || bType == 3)
		return _calc4Direction (nX1,nY1,nX2,nY2);
	else if(bType == 100)
		return _calc360Direction (nX1,nY1,nX2,nY2);
	else if(bType == 200)
		return _calc4LeanDirection(nX1,nY1,nX2,nY2);
	else
		return _calc8Direction (nX1,nY1,nX2,nY2);
		 
}

void GetBattleGPos( int& nX, int& nY,int nDestX, int nDestY, int nDir , int nPos)
{
	int	nChgH	= nPos%2*60;
	int	nChgV	= nPos/2*60;
	int	nChgSH	= nPos/2*60-nPos%2*60;
	int	nChgSV	= nPos/2*30+nPos%2*30;

	switch(nDir)
	{
	case 0:
		nX	= nDestX + nChgH;
		nY	= nDestY - nChgV;
		break;
	case 2:
		nX	= nDestX - nChgV;
		nY	= nDestY + nChgH;
		break;
	case 4:
		nX	= nDestX - nChgH;
		nY	= nDestY + nChgV;
		break;
	case 6:
		nX	= nDestX + nChgV;
		nY	= nDestY - nChgH;
		break;
	case 1:
		nX	= nDestX - nChgSH;
		nY	= nDestY + nChgSV + 1;
		break;
	case 3:
		nX	= nDestX - nChgSH;
		nY	= nDestY - nChgSV - 1;
		break;
	case 5:
		nX	= nDestX + nChgSH;
		nY	= nDestY - nChgSV - 1;
		break;
	case 7:
		nX	= nDestX + nChgSH;
		nY	= nDestY + nChgSV + 1;
		break;
	}
}

char*   itoa( int nVal , char* szTmp, int nLimit)
{
    sprintf(szTmp, "%d",nVal);
    return szTmp;
}

bool AnalyDesc( char * szSrc, int nVal,const char * szClr, char * szOut )
{
	//$"X"  "+" "-" "*" "/"
	char* strB = strstr(szSrc,"$");
	if(strB == NULL)	return false;
	strB++;
	char* strE = strstr(strB,"$");
	if(strE == NULL)	return false;
	char	szTmp[32];
	//strE[0]	= 0;
	int nLen	= strE - strB;

	int nRtl	= 0, nValTmp = 0;
	int i = 0;

	int nCharVal	= GetValByChar(nVal,strB[i],strB[i+1]);
	if(nCharVal!=-1)
	{
		nRtl	= 	nCharVal;
		i++;
	}
	else
	{
		int nValTmp	= atoi(strB+i);
		nRtl	= nValTmp;
		i += strlen(itoa(nValTmp,szTmp,10));
	}
	while(i < nLen)
	{
		switch(strB[i])
		{
		case '+':
			{
				i++;
				int nCharVal	= GetValByChar(nVal,strB[i],strB[i+1]);
				if(nCharVal != -1)
				{
					nRtl	+= nCharVal;
					i++;
				}
				else
				{
					int nValTmp	= atoi(strB+i);
					nRtl	+= nValTmp;
					i += strlen(itoa(nValTmp,szTmp,10));
				}
			}
			break;
		case '-':
			{
				i++;
				int nCharVal	= GetValByChar(nVal,strB[i],strB[i+1]);
				if(nCharVal != -1)
				{
					nRtl	-= nCharVal;
					i++;
				}
				else
				{
					int nValTmp	= atoi(strB+i);
					nRtl	-= nValTmp;
					i += strlen(itoa(nValTmp,szTmp,10));
				}
			}
			break;
		case '*':
			{
				i++;
				int nCharVal	= GetValByChar(nVal,strB[i],strB[i+1]);
				if(nCharVal != -1)
				{
					nRtl	*= nCharVal;
					i++;
				}
				else
				{
					int nValTmp	= atoi(strB+i);
					nRtl	*= nValTmp;
					i += strlen(itoa(nValTmp,szTmp,10));
				}
			}
			break;
		case '/':
			{
				i++;
				int nCharVal	= GetValByChar(nVal,strB[i],strB[i+1]);
				if(nCharVal != -1)
				{
					nRtl	/= nCharVal;
					i++;
				}
				else
				{
					int nValTmp	= atoi(strB+i);
					nRtl	/= nValTmp;
					i += strlen(itoa(nValTmp,szTmp,10));
				}
			}
			break;
		case '%':
			{
				i++;
				int nCharVal	= GetValByChar(nVal,strB[i],strB[i+1]);
				if(nCharVal != -1)
				{
					nRtl	%= nCharVal;
					i++;
				}
				else
				{
					int nValTmp	= atoi(strB+i);
					nRtl	%= nValTmp;
					i += strlen(itoa(nValTmp,szTmp,10));
				}
			}
			break;
		case '!':
			i++;
			break;
		default:
			return false;
		}
	}
	memset(szOut,0,min(strlen(szSrc),1024));
	strncpy(szOut,szSrc,(strB-szSrc-1));
	szOut[strB-szSrc]	= 0;
	sprintf(szTmp,"%d",nRtl);
	//strcat(szOut,"\x001(0,255,0)");
	strcat(szOut,szTmp);
	strcat(szOut,szClr);
	strcat(szOut,strE+1);
	char* szCheck	= strdup(szOut);
	AnalyDesc(szCheck,nVal,szClr,szOut);

	free(szCheck);

	return true;
}

int GetValByChar( int nDefVal, int nChar, int nCharEx )
{
	int nRtl	= -1;
	if(nChar == 'X' || nChar == 'x')
		nRtl	= nDefVal;
	if(nChar == 'L' || nChar == 'l')
		nRtl	= pChar->m_byLvl;

	if(nCharEx == '!')
	{
		for (int i = nRtl-1; i > 0; i--)
		{
			nRtl	+= i;
		}
	}
	
	return nRtl;
}

unsigned long xnGetSecCount()
{
	return time(NULL);
}

bool gfun_ReplaceTxtName( char* szTxt, char* szOut )
{
	if(strlen (szTxt) > 500)
	{
		return false;
	}

	char* szFind	= strstr(szTxt,"$");
	if(szFind )
	{
		int nKey	= atoi (szFind+1);
		char	szReplace[32];
		if(strncmp(szFind+1,"playername",10) == 0)
			sprintf(szReplace,pChar->m_szNick);
		else
			return false;
		
		strncpy (szOut,szTxt,(szFind-szTxt));

		sprintf(szOut+strlen (szOut),"%s",szReplace);


		char* szEnd	= strstr(szFind+1,"$");
		if(szEnd == 0)
			return false;

		char	szSecond[1024];
		memset(szSecond,0,1024);
		if(gfun_ReplaceTxtName (szEnd+1,szSecond))
		{
			sprintf(szOut+strlen(szOut),"%s",szSecond);
		}
		else
			sprintf(szOut+strlen(szOut),"%s",szEnd+1);
		return true;
	}
	return false;
}

void* xnGetDayTime()
{
	unsigned long long timestamp = time(NULL);
	return localtime((time_t*)&timestamp);
	//char tmp[100] = {0};
	//memset(tmp, 0x0, 100);
	//strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", ptm);
}

xnList*		s_pListSprCache	= xnList::Create();
bool gfun_CheckSprCache( char* szFile ,bool bCache)
{
	if(bCache)
	{
		bool	bFind	= false;
		for (int i = 0; i < s_pListSprCache->Count; i++)
		{
			char*	szTmp	= (char*)s_pListSprCache->Items[i];
			if(strcmp(szTmp,szFile) == 0)
			{
				bFind	= true;
				break;
			}
		}
		if(!bFind)
			s_pListSprCache->Add(strdup(szFile));
	}
	return true;
}

void gFun_ClearSpriteCache()
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	while(s_pListSprCache->Count)
	{
		char*	szTmp	= (char*)s_pListSprCache->Delete(0);
		cache->removeSpriteFramesFromFile(szTmp);
		free(szTmp);
	}
}

CCostTime::CCostTime()
{
	ulTime = xnGetTickCount();
}

CCostTime::~CCostTime()
{
	ulTime = xnGetTickCount()-ulTime;
	CCLOG("%dms",ulTime);
}
