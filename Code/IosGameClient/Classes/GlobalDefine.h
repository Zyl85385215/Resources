#pragma once

#define CLIENT_VERSION		5
#define RESOURCE_VERSION	14100100		//分包的客户端版本号设置为0, 更新文件第一个为主包
#define BASERES_VERSION		1
#define PACK_VERSION		"IOSAPP300"
//#define USE_ANDROID
//#define  TEST_LOGIN
#ifdef WIN32
#define SOUND_FILENAME		".wav"
#else
#ifdef USE_ANDROID
#define SOUND_FILENAME		".ogg"
#else
#define SOUND_FILENAME		".aac"
#endif
#endif

#ifdef WIN32
//#define DEFAULT_FONT	"微软雅黑"
//#define DEFAULT_FONT	"做字网改造文鼎简体颜体"
#define DEFAULT_FONT	"Droid Sans Fallback"
#else
#define DEFAULT_FONT	"STHeitiSC-Medium"
#endif


#ifdef USE_ANDROID
#define	RUN_FRAME		25
#define DEFAULT_FONT_SIZE	19
#define HTTP_UPDATE_FILE		"GameUpdate3"
#else
#define	RUN_FRAME		25
#define DEFAULT_FONT_SIZE	20
#define HTTP_UPDATE_FILE		"GameUpdate2"			//越狱"GameUpdate1"   正式"GameUpdate2"
#endif


#ifndef NULL
#define NULL	0
#endif
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef BYTE				 BOOLEAN;
// #ifndef BYTE
// #define BYTE	unsigned char
// #endif
// 
// #ifndef BOOLEAN
// #define BOOLEAN	BYTE
// #endif
// 
// #ifndef BOOL
// #define BOOL int
// #endif
// 
// #ifndef DWORD
// #define DWORD		unsigned long
// #endif
// 
// #ifndef WORD
// #define WORD		unsigned short
// #endif

#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef SAFE_FREE
	#define SAFE_FREE(p)	{if(p) free(p);p=NULL;}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{if(p) delete p;p=NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(ptr) {if(ptr){delete []ptr;ptr=NULL;}}
#endif

//#define USE_IN_IPAD
#define myabs(a)	((a)>0?(a):(-(a)))

extern int		g_nDFScreenWidth;
extern int		g_nDFScreenHeight;
extern int		g_nDFScreenPix;
extern int		nowScreenHeight;
extern int		nowScreenWidth;
extern float nowRateW;
extern float nowRateH;
extern	BYTE	g_byWarSpeed;
extern	BYTE	g_bySvrLockClient;
extern	int	g_nClientAreaLock;

#ifdef WIN32
#define	SCREEN_SIZE_W	g_nDFScreenWidth
#define	SCREEN_SIZE_H	g_nDFScreenHeight
#define	POINT_PIX_CNT	g_nDFScreenPix
#define UTEXT_CHINE(str)	IConvConvert_GBKToUTF8(str)
#define GBK_CHINE(str)	IConvConvert_UTF8ToGBK(str)
#else
typedef struct tagPOINT
{
	long  x;
	long  y;
} POINT;
#define min(a,b)	((a<b)?a:b)
#define max(a,b)	((a>b)?a:b)



#ifdef USE_IN_IPAD
#define	SCREEN_SIZE_W	g_nDFScreenWidth
#define	SCREEN_SIZE_H	g_nDFScreenHeight
#define	POINT_PIX_CNT	g_nDFScreenPix
#else
#define	SCREEN_SIZE_W	g_nDFScreenWidth
#define	SCREEN_SIZE_H	g_nDFScreenHeight
#define	POINT_PIX_CNT	g_nDFScreenPix
#endif

#define UTEXT_CHINE(str)	IConvConvert_GBKToUTF8(str)
#define GBK_CHINE(str)	IConvConvert_UTF8ToGBK(str)
#endif


//defalt XML
#define SaveStringToXML cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey  
#define SaveIntegerToXML cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey  
#define SaveBooleanToXML cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey  
#define SaveFloatToXML cocos2d::CCUserDefault::sharedUserDefault()->setFloatForKey  
#define SaveDoubleToXML cocos2d::CCUserDefault::sharedUserDefault()->setDoubleForKey
#define SaveToXMLOK cocos2d::CCUserDefault::sharedUserDefault()->flush

#define LoadStringFromXML cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey  
#define LoadIntegerFromXML cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey  
#define LoadBooleanFromXML cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey
#define LoadFloatFromXML cocos2d::CCUserDefault::sharedUserDefault()->getFloatForKey
#define LoadDoubleFromXML cocos2d::CCUserDefault::sharedUserDefault()->getDoubleForKey 

#include "GlobalFunction.h"

