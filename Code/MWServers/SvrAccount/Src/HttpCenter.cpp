#include "stdafx.h"
#include "HttpCenter.h"
#include "ZLogFile.h"
#include "ServerCfg.h"
#include "md5Capi.h"
#include "AccountCenter.h"
#include "Base64.h"
#include "CX509.h"
#include "ABbase64.h"
#include "md5.h"
#include "rsa.h"
#include "TimeControl.h"
#include "hmacsha1.h"
#include "signature_verifier.h"

extern	ZLogFile*	g_pLogFile;
//------------------------------------------------------------------------------------------------

void Base64ToBinary(const char* b64str, DWORD str_size, std::string* bin_data){
	DWORD dwSkip;
	DWORD dwFlags;
	DWORD bin_size;
	CryptStringToBinaryA(b64str, str_size, CRYPT_STRING_BASE64,
		NULL, &bin_size, &dwSkip, &dwFlags);
	bin_data->resize(bin_size);
	CryptStringToBinaryA(b64str, str_size, CRYPT_STRING_BASE64,
		(BYTE*)bin_data->data(), &bin_size, &dwSkip, &dwFlags);
}

static int HexPairValue(const char * code) 
{
	int value = 0;
	const char * pch = code;
	for (;;)
	{
		int digit = *pch++;
		if (digit >= '0' && digit <= '9') 
		{
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') 
		{
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') 
		{
			value += digit - 'a' + 10;
		}
		else 
		{
			return -1;
		}
		if (pch == code + 2)
		{
			return value;
		}
		value <<= 4;
	}
}

int UrlDecode(const char *source, char *dest)
{
	char * start = dest;
	while (*source) 
	{
		switch (*source) 
		{
		case '+':
			*(dest++) = ' ';
			break;
		case '%':
			if (source[1] && source[2]) 
			{
				int value = HexPairValue(source + 1);
				if (value >= 0) 
				{
					*(dest++) = value;
					source += 2;
				}
				else 
				{
					*dest++ = '?';
				}
			}
			else 
			{
				*dest++ = '?';
			}
			break;
		default:
			*dest++ = *source;
		}
		source++;
	}
	*dest = 0;
	return dest - start;
}

char*	getRtlByKey(char* szKey, char* szFrom,char* szRtl)
{
	memset(szRtl,0,1024);
	char*	szPA	= strstr(szFrom,szKey);
	if(szPA	== NULL)
		return NULL;

	char*	szPB	= strstr(szPA,":");
	if(szPB == NULL)
		return NULL;
	if(szPB[1] == '"')
		szPB+=2;
	else
		szPB+=1;

	char*	szPC	= strstr(szPB,",");
	if(szPC == NULL)
	{
		szPC	= strstr(szPB,"}");
		if(szPC)
		{
			if(szPC[-1] == '"')
				memcpy(szRtl,szPB,szPC-szPB-1);
			else
				memcpy(szRtl,szPB,szPC-szPB);
		}
		else
		{
			if(szPB[strlen(szPB-1)] == '"')
				memcpy(szRtl,szPB,strlen(szPB-1));
			else
				memcpy(szRtl,szPB,strlen(szPB));
		}

		return szRtl;
	}
	else
	{
		if(szPC[-1] == '"')
			memcpy(szRtl,szPB,szPC-szPB-1);
		else
			memcpy(szRtl,szPB,szPC-szPB);
		return szRtl;
	}

	return NULL;
}

char*	getParamByKey(char*	szKey,char* szFrom,char* szRtl)
{
	memset(szRtl,0,1024);
	char*	szPA	= strstr(szFrom,szKey);
	if(szPA	== NULL)
		return szRtl;

	char*	szPB	= strstr(szPA,"=");
	if(szPB == NULL)
		return szRtl;
	szPB++;
	char*	szPC	= strstr(szPB,"&");
	if(szPC == NULL)
	{
		strcpy(szRtl,szPB);

		return szRtl;
	}
	else
	{
		memcpy(szRtl,szPB,szPC-szPB);
		return szRtl;
	}
	return szRtl;
}

int UrlEncode(const char *source, char *dest, unsigned max)
{
	static const char *digits = "0123456789ABCDEF";
	unsigned char ch;
	unsigned len = 0;
	char *start = dest;

	while (len < max - 4 && *source)
	{
		ch = (unsigned char)*source;
		if (*source == ' ') 
		{
			*dest++ = '+';
		}
		else if (isalnum(ch) || strchr("-_.!~*'()", ch)) 
		{
			*dest++ = *source;
		}
		else 
		{
			*dest++ = '%';
			*dest++ = digits[(ch >> 4) & 0x0F];
			*dest++ = digits[       ch & 0x0F];
		}
		source++;
	}
	*dest = 0;
	return start - dest;
}

//CRITICAL_SECTION g_xRequestLock;
//request("reg.163.com", "http://reg.163.com/CheckUser.jsp", "test");
//函数实现
//
//bool Base64Encode(const unsigned char* Data,int DataByte,OUT char* szOut)
//{
//	//编码表
//	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//	//返回值
//	//string strEncode;
//	unsigned char Tmp[4]={0};
//	int LineLength=0;
//	for(int i=0;i<(int)(DataByte / 3);i++)
//	{
//		Tmp[1] = *Data++;
//		Tmp[2] = *Data++;
//		Tmp[3] = *Data++;
//		szOut[strlen(szOut)]	= EncodeTable[Tmp[1] >> 2];
//		szOut[strlen(szOut)]	= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
//		szOut[strlen(szOut)]	= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
//		szOut[strlen(szOut)]	= EncodeTable[Tmp[3] & 0x3F];
//		if(LineLength+=4,LineLength==76) {strcat(szOut,"\r\n");LineLength=0;}
//	}
//	//对剩余数据进行编码
//	int Mod=DataByte % 3;
//	if(Mod==1)
//	{
//		Tmp[1] = *Data++;
//		szOut[strlen(szOut)]	= EncodeTable[(Tmp[1] & 0xFC) >> 2];
//		szOut[strlen(szOut)]	= EncodeTable[((Tmp[1] & 0x03) << 4)];
//		strcat(szOut,"==");
//	}
//	else if(Mod==2)
//	{
//		Tmp[1] = *Data++;
//		Tmp[2] = *Data++;
//		szOut[strlen(szOut)]	= EncodeTable[(Tmp[1] & 0xFC) >> 2];
//		szOut[strlen(szOut)]	= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
//		szOut[strlen(szOut)]	= EncodeTable[((Tmp[2] & 0x0F) << 2)];
//		strcat(szOut,"=");
//	}
//
//	return true;
//}

int	requestbyIp(char* szIp, int nPort, char* api)
{
	//EnterCriticalSection(&g_xRequestLock);
	sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons((unsigned short)nPort);
    sin.sin_addr.s_addr = inet_addr(szIp);//

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
		//LeaveCriticalSection(&g_xRequestLock);
        return -100;
    }

    //建立连接
    if (connect(sock, (const struct sockaddr *)&sin, sizeof(sockaddr_in) ) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
        return -101;
    }

    //初始化发送信息
    char send_str[4096] = {0};

    //头信息
    strcat(send_str, "GET ");
    strcat(send_str, api);
    strcat(send_str, " HTTP/1.1\r\n");
    strcat(send_str, "Host: ");
    strcat(send_str, szIp);
    strcat(send_str, "\r\n");
    strcat(send_str, "Connection: keep-alive\r\n");


    char content_header[100];
    sprintf(content_header,"Content-Length: %d\r\n", 1);
    strcat(send_str, content_header);
    strcat(send_str, "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2)\r\n");
    strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");

    //内容信息
    strcat(send_str, "\r\n");

	int zero = 500;				
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&zero, sizeof(int));
	
    if (send(sock, send_str, strlen(send_str),0) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
        return -101;
    }
	closesocket(sock);
	//zero = 1000;
	//setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&zero, sizeof(int));
 //   //获取返回信息
 //   char recv_str[4096] = {0};
 //   if (recv(sock, recv_str, sizeof(recv_str), 0) == -1)
 //   {
	//	LeaveCriticalSection(&g_xRequestLock);
 //       return -101;
 //   }
	//LeaveCriticalSection(&g_xRequestLock);
    return 0;
}

int request(char* hostname, char* api, char* parameters, OUT char* szOut)
{
    //WSADATA WsaData;
    //WSAStartup(0x0101, &WsaData);
	//EnterCriticalSection(&g_xRequestLock);
    ////初始化socket
    struct hostent* host_addr = gethostbyname(hostname);
    if (host_addr == NULL)
    {
		//LeaveCriticalSection(&g_xRequestLock);
    //    cout<<"Unable to locate host"<<endl;
        return -103;
    }


    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons((unsigned short)80);
    sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list);//inet_addr("192.168.1.100");//


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
		//LeaveCriticalSection(&g_xRequestLock);
        return -100;
    }


    //建立连接
    if (connect(sock, (const struct sockaddr *)&sin, sizeof(sockaddr_in) ) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
        return -101;
    }


    //初始化发送信息
    char send_str[4096] = {0};

	//strcat(send_str, "GET /Public/notify/apple/notify_url.php?receipt=12345 HTTP/1.1\r\n");
	//strcat(send_str, "Host: www.176sy.com\r\n");
	//strcat(send_str, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)	Gecko/20050225 Firefox/1.0.1\r\n");
	//strcat(send_str, "Connection: keep-alive");

    //头信息
    strcat(send_str, "POST ");
    strcat(send_str, api);
    strcat(send_str, " HTTP/1.1\r\n");
    strcat(send_str, "Host: ");
    strcat(send_str, hostname);
    strcat(send_str, "\r\n");
    strcat(send_str, "Connection: keep-alive\r\n");


    char content_header[100];
    sprintf(content_header,"Content-Length: %d\r\n", strlen(parameters));
    strcat(send_str, content_header);
 //   //strcat(send_str, "Cache-Control: max-age=0\r\n");
 //   //strcat(send_str, "Origin: https://sandbox.itunes.apple.com\r\n");
	// //strcat(send_str, "Origin: https://www.176sy.com\r\n");
    strcat(send_str, "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2)\r\n");
    strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");
	//strcat(send_str, "Accept: application/json, text/javascript, text/html, */*; q=0.01\r\n");

 //   //strcat(send_str, "Referer: http://sandbox.itunes.apple.com/\r\n");
	////strcat(send_str, "Referer: http://www.176sy.com/\r\n");
 //   strcat(send_str, "Accept-Encoding: gzip,deflate,sdch\r\n");
 //   strcat(send_str, "Accept-Language: zh-cn\r\n");

    //内容信息
    strcat(send_str, "\r\n");
    strcat(send_str, parameters);

	int zero = 500;				
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&zero, sizeof(int));
    if (send(sock, send_str, strlen(send_str),0) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
        return -101;
    }

	zero = 1000;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&zero, sizeof(int));

    //获取返回信息
    char recv_str[4096] = {0};
    if (recv(sock, recv_str, sizeof(recv_str), 0) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
        return -101;
    }
	char* szRtl	= strstr(recv_str,"product_id");
	if(szRtl)
		strcpy(szOut,szRtl);
	closesocket(sock);
	//LeaveCriticalSection(&g_xRequestLock);
    return 0;
}

int requestTPlat(bool bPost,char* hostname,int nPort, char* api, char* parameters, OUT char* szOut,BYTE bTP)
{
    //WSADATA WsaData;
    //WSAStartup(0x0101, &WsaData);
	//EnterCriticalSection(&g_xRequestLock);
    ////初始化socket
    struct hostent* host_addr = gethostbyname(hostname);
    if (host_addr == NULL)
    {
		//LeaveCriticalSection(&g_xRequestLock);
    //    cout<<"Unable to locate host"<<endl;
        return -103;
    }


    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons((unsigned short)nPort);
    sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list);//inet_addr("192.168.1.100");//


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
		//LeaveCriticalSection(&g_xRequestLock);
        return -100;
    }


    //建立连接
    if (connect(sock, (const struct sockaddr *)&sin, sizeof(sockaddr_in) ) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
		return -101;
    }


    //初始化发送信息
    char send_str[4096] = {0};

	//strcat(send_str, "GET /Public/notify/apple/notify_url.php?receipt=12345 HTTP/1.1\r\n");
	//strcat(send_str, "Host: www.176sy.com\r\n");
	//strcat(send_str, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)	Gecko/20050225 Firefox/1.0.1\r\n");
	//strcat(send_str, "Connection: keep-alive");

    //头信息
	if(bPost)
		strcat(send_str, "POST ");
	else
		strcat(send_str, "GET ");
    strcat(send_str, api);
    strcat(send_str, " HTTP/1.1\r\n");
    strcat(send_str, "Host: ");
    strcat(send_str, hostname);
    strcat(send_str, "\r\n");
    //strcat(send_str, "Connection: keep-alive\r\n");


    char content_header[100];
    sprintf(send_str+strlen(send_str),"Content-Length: %d\r\n", strlen(parameters));
    //strcat(send_str, content_header);
    //strcat(send_str, "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2)\r\n");

    strcat(send_str, "Content-Type: application/x-www-form-urlencoded\r\n");


    //内容信息
    strcat(send_str, "\r\n");
    strcat(send_str, parameters);

	int zero = 800;	
	if(bTP == TP_NULL || bTP == TP_IAP)	//广告验证超时设3秒
		zero = 3000;
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&zero, sizeof(int));
	if (send(sock, send_str, strlen(send_str),0) == -1)
	{
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
		return -101;
	}
	if(bTP == TP_IAP)
		zero	= 10000;
	else
		zero	= 1500;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&zero, sizeof(int));

    //获取返回信息
    char recv_str[2048] = {0};
    if (recv(sock, recv_str, sizeof(recv_str), 0) == -1)
    {
		closesocket(sock);
		//LeaveCriticalSection(&g_xRequestLock);
        return -101;
    }
	if(bTP == TP_PPSPAY)
	{
		char*	szTmp1	= strstr(recv_str,"status");
		if(szTmp1 == NULL)
		{
			recv(sock, szOut, 256, 0);
		}
		else
		{
			strcpy(szOut,szTmp1);
		}
	}
	else if(bTP == TP_91PAY || bTP == TP_ADR91)
	{
		char*	szTmp1	= strstr(recv_str,"ErrorCode");
		if(szTmp1 == NULL)
		{
			recv(sock, szOut, 256, 0);
		}
		else
		{
			strcpy(szOut,szTmp1);
		}
	}
	else if(bTP == TP_TBTPAY)
	{
		if(recv_str[strlen(recv_str)-2] == '-')
			strcpy(szOut,"-1");
		else if (recv_str[strlen(recv_str)-1] != '0' || (recv_str[strlen(recv_str)-2] >= '0' && recv_str[strlen(recv_str)-2] <= '9'))
			strcpy(szOut,"1");
		else
			strcpy(szOut,"0");
	}
	else if(bTP == TP_KYPAY)
	{
		char*	szTmp1	= strstr(recv_str,"code");
		if(szTmp1 == NULL)
		{
			recv(sock, szOut, 256, 0);
		}
		else
		{
			strcpy(szOut,szTmp1);
		}
	}
	else if(bTP == TP_IAP)
	{
		char*	szTmp1	= strstr(recv_str,"status");
		if(szTmp1 == NULL)
		{
			recv(sock, szOut, 1024, 0);
		}
		else
		{
			strcpy(szOut,szTmp1);
		}
	}
	else if(bTP == TP_DJOYPAY || bTP == TP_ADRDJOY)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"error_code");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRUC)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"state");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ITOOLPAY)
	{
		char*	szTmp1	= strstr(recv_str,"status");
		if(szTmp1 != NULL)
			strcpy(szOut,szTmp1);
		else
		{
			recv(sock, szOut, 1024, 0);
		}
	}
	else if(bTP == TP_ADROPPO)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"efUser");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRAZ)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"msg");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}

	else if(bTP	== TP_ADRLX)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"AccountID");
			if(szTmp1 != NULL)
			{
				strncpy(szOut,szTmp1,1000);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRWDJ)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"true");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRMZ)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"\"state\"");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRMZW)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"\"code\"");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRHUAWEI)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"userID");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADR360)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"access_token");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			szTmp1	= strstr(recv_str,"\"id\"");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRMMY)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"success");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRYYH)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"\"data\"");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRYL)
	{
		int nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"userName");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRJF)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"resultCode");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRMI)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"errcode");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	else if(bTP == TP_ADRBDDK)
	{
		int		nLoopLimit	= 5;
		while(nLoopLimit--)
		{
			char*	szTmp1	= strstr(recv_str,"error_code");
			if(szTmp1 != NULL)
			{
				strcpy(szOut,szTmp1);
				break;
			}
			memset(recv_str,0,2048);
			if(nLoopLimit == 0)
				break;
			recv(sock, recv_str, 1024, 0);
		}
	}
	closesocket(sock);
	//LeaveCriticalSection(&g_xRequestLock);
    return 0;
}

HttpCenter::HttpCenter()
{
	//InitializeCriticalSection(&g_xRequestLock);
	char	szRecv[256]	= {0};
	//requestPost("passport_i.25pp.com",8080,"/index?tunnel-command=2852126756","07d72e5200cb5c0031d988e618d90d5b",szRecv);
	//char*	szRtl	= getRtlByKey("status",szRecv);
	//AfxMessageBox(szRtl);
	//requestTPlat(true,"www.mejoygame.com",80,"/pay/adraz.php","data=Q08OIiG542RxRNITznk64%2Ftr53SLi4nmzYI%2B1tWZXSKpI7NifgsHYdQo1jpJsRqPevEIlhuyvyXV3VgwlZ2MTTQhJE5PG%2BxCP14XIhrR%2BiVjFenYsTbrw0Wy%2FVJJN53pbNTjLVu9Op%2FO6UFR7VQBYc9ToyjFMbovHz0McqqTLrxuUaNtdOdI6xoxUEjbNNPUJyJ1b9FsvfOAhwCa3VxR%2FiKTjgIPV9bZcyh8n2T%2BGp10O%2FPJFOTsvgsbdfZbgGi2%2FPikw6NssqlJtqRBIbmBRnRE94coG2v%2Ba7kxVCMuV%2BVwkOWxjFQJqGhFusfWGjXawu2UGHAvyLfNgj7W1ZldIvWOYPAJKZpgdlii2sR2o1oc3ncafoJJYQ%3D%3D",szRecv,TP_TBTPAY);
	//:0:oauth_token=e57e77e6cf7f7dd1ab091f1438367c1e&oauth_token_secret=f47ae031505e8bea879396fa1d1feb20
	//TPLogin("91427313","30F71D301E93C9B461IFLRL314F729E1",TP_ADRDJOY,szRecv,szRecv,szRecv);
	
	//TPAdrMMYDeal("orderID=014072205230910478&productPrice=6&productDesc=101#mmy5960126#DBEF8FAC72323369B7DD69E0E0399293#IOSAPP200-101-15023-1406020904&tradeState=success");
}

HttpCenter::~HttpCenter()
{
	//LeaveCriticalSection(&g_xRequestLock);
}

int HttpCenter::RecvHttpMsg( DWORD dwMark, char* szBuf,int nLen )
{
	
	char	szRecv[4096]	= {0};
	if(nLen > 4096)
	{
		memcpy(szRecv,szBuf,2000);
		g_pLogFile->OutPut("HTTP OVERLEN: %s",szRecv);
		return 0;
	}

	memcpy(szRecv,szBuf,nLen);
	char	szHttp[4096]	= {0};
	std::map<DWORD,char*>::iterator	itor	= mapHttpCache.find(dwMark);
	//累积收包信息,可能需要多次TCP拼接包
	if(itor	!= mapHttpCache.end())
	{
		if(strlen(itor->second)+nLen >= 4096)
		{
			free(itor->second);
			mapHttpCache.erase(itor);
			g_pLogFile->OutPut("HTTP RECV OVER4096: %s",szRecv);
			return 0;
		}
		memcpy(szHttp,itor->second,strlen(itor->second));
		memcpy(szHttp+strlen(szHttp),szRecv,nLen);
		if(strstr(szHttp,"\r\n\r\n") == 0)
		{//已完成
			free(itor->second);
			mapHttpCache.erase(itor);

		}
		else
		{
			free(itor->second);
			itor->second	= strdup(szHttp);
			g_pLogFile->OutPut("HTTP RECV BECONTINU: %s",szRecv);
			return 0;//未收完,下次继续
		}
	}
	else
	{
		//if(strstr(szRecv,"\r\n\r\n"))
		{//已完成
			memcpy(szHttp,szRecv,nLen);
		}
		//else
		//{
		//	memcpy(szHttp,szRecv,nLen);
		//	mapHttpCache[dwMark]	= strdup(szHttp);
		//	g_pLogFile->OutPut("HTTP RECV BECONTINU: %s",szRecv);
		//	return 0;	//未收完,下次继续
		//}
	}
	//这里要设计多线程部分


	//解析 szHttp
	char*	szCTmp	= strstr(szHttp,"HTTP");

	if(szCTmp&& szCTmp!= szHttp)
		szCTmp[-1]	= 0;

	//提取平台资料
	char*	szRtl	= strstr(szHttp,"?");
	if(szRtl)
	{
		szRtl[0] = 0;
		szRtl++;

		if(strstr(szHttp,"adrsky"))
		{//SK平台处理
			if(!SKPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrsky Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrsky Success: %s",szRtl);
		}
		else if(strstr(szHttp,"91pay"))
		{//91平台处理
			if(!TP91PayDeal(szRtl))
				g_pLogFile->OutPut("HTTP 91Pay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP 91Pay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"91adr"))
		{//91平台处理
			if(!TP91AdrDeal(szRtl))
				g_pLogFile->OutPut("HTTP 91adr Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP 91adr Success: %s",szRtl);
		}
		else if(strstr(szHttp,"bddkadr"))
		{//91平台处理
			if(!TPBDDKAdrDeal(szRtl))
				g_pLogFile->OutPut("HTTP bddkadr Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP bddkadr Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrhw"))
		{
			if(!TPAdrHWDeal(szRtl))
				g_pLogFile->OutPut("HTTP huawei Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP huawei Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrmzw"))
		{
			if(!TPAdrMZWDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrmzw Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrmzw Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrmz"))
		{
			if(!TPAdrMZDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrmz Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrmz Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrpps"))
		{
			if(!TPAdrPPSDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrpps Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrpps Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrjf"))
		{
			if(!TPAdrJFDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrjf Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrpps Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrmmy"))
		{
			if(!TPAdrMMYDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrmmy Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrmmy Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adryyh"))
		{
			if(!TPAdrYYHDeal(szRtl))
				g_pLogFile->OutPut("HTTP adryyh Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adryyh Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adryl"))
		{
			if(!TPAdrYLDeal(szRtl))
				g_pLogFile->OutPut("HTTP adryl Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adryl Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrwdj"))
		{//91平台处理
			if(!TPAdrWDJDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrWDJ Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrWDJ Success: %s",szRtl);
		}
		else if(strstr(szHttp,"oppoadr"))
		{//91平台处理
			if(!TPOppoAdrDeal(szRtl))
				g_pLogFile->OutPut("HTTP Oppoadr Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP Oppoadr Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adr360"))
		{//91平台处理
			if(!TPAdr360Deal(szRtl))
				g_pLogFile->OutPut("HTTP adr360 Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adr360 Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adraz"))
		{//91平台处理
			if(!TPAdrAZDeal(szRtl))
				g_pLogFile->OutPut("HTTP adraz Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adraz Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrmi"))
		{//91平台处理
			if(!TPAdrMiDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrmi Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrmi Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrlx"))
		{//91平台处理
			if(!TPAdrLXDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrlx Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrlx Success: %s",szRtl);
		}
		else if(strstr(szHttp,"ucpay"))
		{//91平台处理
			if(!UCPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP UCPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP UCPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"ppspay"))
		{//PPS平台
			if(!PPSPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP PPSPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP PPSPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"itool"))
		{//PPS平台
			if(!IToolPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP IToolPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP IToolPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"tbtpay"))
		{
			if(!TBTPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP TBTPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP TBTPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"abpay"))
		{
			if(!ABPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP ABPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP ABPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"djoypay"))
		{
			if(!DJoyPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP DJoyPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP DJoyPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"adrdjoy"))
		{
			if(!TPAdrDJoyDeal(szRtl))
				g_pLogFile->OutPut("HTTP adrdjoy Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP adrdjoy Success: %s",szRtl);
		}
		else if(strstr(szHttp,"kypay"))
		{
			if(!KYPayDeal(szRtl))
				g_pLogFile->OutPut("HTTP KYPay Failed: %s",szRtl);
			else
				g_pLogFile->OutPut("HTTP KYPay Success: %s",szRtl);
		}
		else if(strstr(szHttp,"dianru"))
		{
			if(!DianRuCheck(szRtl))
				g_pLogFile->OutPut("HTTP DianRuCheck Failed: %s",szRtl);
		}
		else if(strstr(szHttp,"youmi"))
		{
			if(!YouMiCheck(szRtl))
				g_pLogFile->OutPut("HTTP YouMiCheck Failed: %s",szRtl);
		}
		else if(strstr(szHttp,"adsage"))
		{
			if(!AdsageCheck(szRtl))
				g_pLogFile->OutPut("HTTP AdsageCheck Failed: %s",szRtl);
		}
		else if(strstr(szHttp,"duomeng"))
		{
			if(!DuoMengCheck(szRtl))
				g_pLogFile->OutPut("HTTP DuoMengCheck Failed: %s",szRtl);
		}
		else if(strstr(szHttp,"wanpu"))
		{
			if(!WanPuCheck(szRtl))
				g_pLogFile->OutPut("HTTP WanPuCheck Failed: %s",szRtl);
		}
		else if(strstr(szHttp,"limei"))
		{
			if(!LiMeiCheck(szRtl))
				g_pLogFile->OutPut("HTTP LiMeiCheck Failed: %s",szRtl);
		}
		else if(strstr(szHttp,"InviteCmd"))
		{//邀请码相关处理
			InviteDeal(szRtl);
		}
		else
		{//找不到平台来源
			g_pLogFile->OutPut("HTTP TP NOEXIST ERROR: %s",szHttp);
		}
	}
	else
	{
		g_pLogFile->OutPut("HTTP FIND NO ?MARK: %s",szHttp);
	}
	return 1;
}

bool HttpCenter::ABPayDeal(char* szParam)
{
	char	szCharRtl[1024]	= {0};
	char	szCharRtl2[1024]	= {0};
	char*	szTmp	= getParamByKey("transdata",szParam,szCharRtl);
	char	szUrlData[1024];
	strcpy(szUrlData,szTmp);
	szTmp	= getRtlByKey("result",szUrlData,szCharRtl2);
	if(szTmp == NULL || atoi(szTmp))
		return false;

	char	szSignUrl[256];
	szTmp	= getParamByKey("sign",szParam,szCharRtl);
	strcpy(szSignUrl,szTmp);

	char depuk[1024] =  "MzBDMjU5Q0JFNDRGQjMwQjBCMkQzN0I1OUZGNTAzQTRCN0FENEFBMU1UVTRPVGMyTVRJM05EQXpOVE0yTnprMU1Ua3JNamcwTXpZeE5EUXlNVGd5TkRBeE1qUTVORGN6TVRrMk5qRXpNek13T1RVd016Y3lORGt4";
	
	if(_checkABOk(depuk,szUrlData,szSignUrl)==false)
		return false;

	//获取目标服务器编号
	szTmp	= getRtlByKey("cpprivate",szUrlData,szCharRtl2);
	if(szTmp == NULL)
		return false;
	char*	szSplit	= strstr(szTmp,"|+|");
	char	szSvr[32] = {0};
	memcpy(szSvr,szTmp,szSplit-szTmp);
	int	nSvrID	= atoi(szSvr);
	char*	szSp2	= strstr(szSplit+3,"|+|");
	BYTE	bTpMark	= 0;
	if(szSp2)
	{
		char	szTPMark[32] = {0};
		memcpy(szTPMark,szSplit+3,szSp2-szSplit-3);
		if(atoi(szTPMark))
			bTpMark	= 1;
		szSplit	= szSp2;
	}

	char	szAccUin[50];
	strcpy(szAccUin,szSplit+3);	//账号信息

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"abpay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getRtlByKey("money",szUrlData,szCharRtl2);

		if(szTmp == NULL)
			return false;
		char	szRMB[64];
		strcpy(szRMB,szTmp);
		int nPayVal	= atoi(szRMB);
		nPayVal	= nPayVal/100;
		if(nPayVal == 0)
			return false;

		char	szOrder[256];	//订单号
		szTmp	= getRtlByKey("transid",szParam,szCharRtl2);
		if(szTmp == NULL)
			return false;
		sprintf(szOrder,"ab%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAccUin,szOrder,nPayVal,TP_ABPAY+bTpMark))
			return true;
		else
			return false;
	}
	//else
	//{
	//	return false;
	//}

	return true;
}

bool HttpCenter::TBTPayDeal(char* szParam)
{
	char	szCharRtl[1024]	= {0};
	
	//AppId：140341
	//AppKey：ivF8He3r5Olyn#ViXu8RHerB4OaynVKh
	char*	szTmp	= getParamByKey("debug",szParam,szCharRtl);
	if(atoi(szTmp) == 1)
	{//测试版本过来的信息
#ifndef _DEBUG
		return false;
#endif
	}

	Cmd5Capi	md5Capi;

	char	szSign[2048]	= {0};
	char*	szMd5End	= strstr(szParam,"&sign");
	if(szMd5End == NULL)
		return false;
	memcpy(szSign,szParam,szMd5End-szParam);
	strcat(szSign,"&key=ivF8He3r5Olyn#ViXu8RHerB4OaynVKh");
	char* szMd5	= md5Capi.Digest(szSign);

	szTmp	= getParamByKey("sign",szParam,szCharRtl);
	if(strcmp(szMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	szTmp	= getParamByKey("paydes",szParam,szCharRtl);
	if(strlen(szTmp) == 0)
		return false;
	char*	szSplit	= strstr(szTmp,"|+|");
	char	szSvr[32] = {0};
	memcpy(szSvr,szTmp,szSplit-szTmp);
	int	nSvrID	= atoi(szSvr);
	char	szAccUin[50];
	strcpy(szAccUin,szSplit+3);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"tbtpay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else/* if(nSvrID == ServerCfg::GetInstance().nServerID)*/
	{//本服处理该订单
		szTmp	= getParamByKey("amount",szParam,szCharRtl);
		if(strlen(szTmp) == 0)
			return false;

		int nPayVal	= atoi(szTmp)/100;

		char	szOrder[256];	//订单号
		szTmp	= getParamByKey("trade_no",szParam,szCharRtl);

		sprintf(szOrder,"tbt%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAccUin,szOrder,nPayVal,TP_TBTPAY))
			return true;
		else
			return false;
	}
	//else
	//{
	//	return false;
	//}

	return true;
}

bool HttpCenter::TP91AdrDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("PayStatus",szParam,szCharRtl);
	if(strlen(szTmp) == 0 || atoi(szTmp) != 1)
		return false;

	//APPID：114610
	//APPKEY：d16858ac0349955b07e541e32e847af8df69c68fd5fd0a55
	Cmd5Capi	md5Capi;

	char	szSign[2048]	= {0};

	sprintf(szSign,"114610%s",getParamByKey("Uin",szParam,szCharRtl));
	sprintf(szSign+strlen(szSign),"%sd16858ac0349955b07e541e32e847af8df69c68fd5fd0a55",getParamByKey("OrderMoney",szParam,szCharRtl));

	char* szMd5	= md5Capi.Digest(szSign);
	char*	szTmpA	= getParamByKey("Sign",szParam,szCharRtl);
	if(strcmp(szMd5,szTmpA) != 0)
		return false;

	//获取目标服务器编号
	szTmp	= getParamByKey("Note",szParam,szCharRtl);
	if(strlen(szTmp) == 0)
		return false;
	int	nSvrID	= atoi(szTmp);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"91adr?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("OrderMoney",szParam,szCharRtl);
		if(strlen(szTmp) == 0)
			return false;

		char	szRMB[64];
		strcpy(szRMB,szTmp);
		char* szTmpC	= strstr(szRMB,".");
		szTmpC[0]	= 0;
		int nPayVal	= atoi(szRMB);

		char	szOrder[256];	//订单号
		szTmp	= getParamByKey("ConsumeStreamId",szParam,szCharRtl);

		sprintf(szOrder,"nd91%s",szTmp);
		szTmp	= getParamByKey("Uin",szParam,szCharRtl);
		char	szAcc[64];	//玩家账号
		sprintf(szAcc,"nd91%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADR91))
			return true;
		else
			return false;
	}

	return true;
}
bool HttpCenter::TP91PayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("PayStatus",szParam,szCharRtl);
	if(strlen(szTmp) == 0 || atoi(szTmp) != 1)
		return false;

	//APPID：113016
	//APPKEY：e1f201c3cdb47c35b48144d6b4642abdaebfada017a310c9
	Cmd5Capi	md5Capi;
	
	char	szSign[2048]	= {0};

	sprintf(szSign,"113016%s",getParamByKey("Uin",szParam,szCharRtl));
	sprintf(szSign+strlen(szSign),"%se1f201c3cdb47c35b48144d6b4642abdaebfada017a310c9",getParamByKey("OrderMoney",szParam,szCharRtl));

	char* szMd5	= md5Capi.Digest(szSign);
	char*	szTmpA	= getParamByKey("Sign",szParam,szCharRtl);
	if(strcmp(szMd5,szTmpA) != 0)
		return false;
	
	//获取目标服务器编号
	szTmp	= getParamByKey("Note",szParam,szCharRtl);
	if(strlen(szTmp) == 0)
		return false;
	int	nSvrID	= atoi(szTmp);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"91pay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("OrderMoney",szParam,szCharRtl);
		if(strlen(szTmp) == 0)
			return false;

		char	szRMB[64];
		strcpy(szRMB,szTmp);
		char* szTmpC	= strstr(szRMB,".");
		szTmpC[0]	= 0;
		int nPayVal	= atoi(szRMB);

		char	szOrder[256];	//订单号
		szTmp	= getParamByKey("ConsumeStreamId",szParam,szCharRtl);

		sprintf(szOrder,"tp91%s",szTmp);
		szTmp	= getParamByKey("Uin",szParam,szCharRtl);
		char	szAcc[64];	//玩家账号
		sprintf(szAcc,"tp91%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_91PAY))
			return true;
		else
			return false;
	}

	return true;
}

bool HttpCenter::KYPayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char	szCharRtl2[1024]	= {0};

	char depuk[1024] =  "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDHpAfjVw8xQ4GJtDJ7Pw+MebLPo"
		"5FmroJn5Iqa8ZlmmVL9I0Ds6Lh/H7ewJIMEd900OAxNdwQVkBVuoiMEj/2OOoMQYJ"
		"ObKsEP3sKKdgMGmcSKzppe843RE6FBYY/8n5MOLrhCFSEUW2rN8i7fqdUxmDOTC4g"
		"d7VfbfYJOaulJcQIDAQAB";

	unsigned int depuklen;
	unsigned char *puk = base64Decode(depuk,depuklen);

	char*	szTmp	= getParamByKey("notify_data",szParam,szCharRtl);//"Z7y6zRFzExubI64NE2OL7R90bXpGhvV4jMH7FuHh0Hf%2BuL6XeZ3cyawR6tYQnKgqOpvBya21jfudsJbKESyYlAnoQvxYirpyyr76VCLW%2FWY91%2FJd%2B1kQXI72wAIOZMdw31Tfam1EZZyzTNZZ%2BnS3cLz18Voe3Rsk3PDTvbIvBu681EOR7vu01RdrQ4lb3s%2Fj3D%2FFiTjsmAcAFGmshrGnq9eLx25NdfzAC8Y3M3LYhlyIVOH4aw6db98cp1jtof%2BK8oc2wPRUDSaFn71Hikmbz6W0zQXEtsHj%2BtUm0B2yw2xFCZSjnTe3AQrdfy00gbkfP%2F4zGNGI1qPf%2Bwpua%2F%2BPaA%3D%3D";
	char	szUrlD[2048]	= {0};
	UrlDecode(szTmp,szUrlD);
	unsigned int demlen;
	unsigned char *de = base64Decode(szUrlD,demlen);
	unsigned char testtext[1024] = {0};
	unsigned int txtlen;

	CX509 rsa;
	memset(testtext,0,1024);
	txtlen = rsa.DoEnde(puk,depuklen,de,demlen,testtext,1024,EX_PUBLICKEY_DE);

	szTmp	= getParamByKey("payresult",(char*)testtext,szCharRtl);
	if(strlen(szTmp) ==0 || atoi(szTmp) != 0)
		return false;
	
	//获取目标服务器编号
	szTmp	= getParamByKey("dealseq",(char*)testtext,szCharRtl);
	char*	szTmp2	= getParamByKey("dealseq",(char*)testtext,szCharRtl2);
	if(strlen(szTmp) == 0 || strcmp(szTmp2,szTmp) != 0)
		return false;

	char*	szSplit	= strstr(szTmp,"|+|");
	char	szSvr[32] = {0};
	memcpy(szSvr,szTmp,szSplit-szTmp);
	int	nSvrID	= atoi(szSvr);
	char	szOrder[50];
	strcpy(szOrder,szSplit+3);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"kypay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("fee",(char*)testtext,szCharRtl);

		if(strlen(szTmp)==0)
			return false;
		char	szRMB[64];
		strcpy(szRMB,szTmp);
		char* szTmpC	= strstr(szRMB,".");
		if(szTmpC)
			szTmpC[0]	= 0;
		int nPayVal	= atoi(szRMB);

		char	szOrder[256];	//订单号
		szTmp	= getParamByKey("orderid",szParam,szCharRtl);
		if(strlen(szTmp)==0)
			return false;
		sprintf(szOrder,"ky%s",szTmp);
		szTmp	= getParamByKey("uid",szParam,szCharRtl);
		if(strlen(szTmp)==0)
			return false;
		char	szAcc[64];	//玩家账号
		sprintf(szAcc,"ky%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_KYPAY))
			return true;
		else
			return false;
	}
	//else
	//{
	//	return false;
	//}

	return true;
}

bool HttpCenter::DJoyPayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};

	//AppId：140341
	//AppKey：ivF8He3r5Olyn#ViXu8RHerB4OaynVKh
	//PK:vHu4ccGkrJbA
	char*	szTmp	= getParamByKey("result",szParam,szCharRtl);
	if(atoi(szTmp) != 1)
		return false;

	char	szCheckMD5[1024]	= {0};
	sprintf(szCheckMD5+strlen(szCheckMD5),"order=%s",getParamByKey("order",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&money=%s",getParamByKey("money",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&mid=%s",getParamByKey("mid",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&time=%s",getParamByKey("time",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&result=%s",getParamByKey("result",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&ext=%s&key=vHu4ccGkrJbA",getParamByKey("ext",szParam,szCharRtl));

	Cmd5Capi	md5Capi;
	char* szMd5	= md5Capi.Digest(szCheckMD5);

	szTmp	= getParamByKey("signature",szParam,szCharRtl);
	if(strcmp(szMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	szTmp	= getParamByKey("ext",szParam,szCharRtl);
	if(strlen(szTmp) == 0)
		return false;
	char*	szSplit	= strstr(szTmp,"SPLIT");
	char	szSvr[32] = {0};
	memcpy(szSvr,szTmp,szSplit-szTmp);
	int	nSvrID	= atoi(szSvr);
	char	szOrder[256];	//订单号
	sprintf(szOrder,"djoy%s",szSplit+5);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"djoypay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("money",szParam,szCharRtl);
		if(strlen(szTmp)==0)
			return false;
		char	szRMB[64];
		strcpy(szRMB,szTmp);
		char* szTmpC	= strstr(szRMB,".");
		if(szTmpC)
			szTmpC[0]	= 0;
		int nPayVal	= atoi(szRMB);

		char	szAcc[64];
		szTmp	= getParamByKey("mid",szParam,szCharRtl);

		sprintf(szAcc,"djoy%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_DJOYPAY))
			return true;
		else
			return false;
	}
	//else
	//{
	//	return false;
	//}

	return true;
}

bool HttpCenter::TPAdrDJoyDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};

	//PK:vHu4ccGkrJbA
	char*	szTmp	= getParamByKey("result",szParam,szCharRtl);
	if(atoi(szTmp) != 1)
		return false;

	char	szCheckMD5[1024]	= {0};
	sprintf(szCheckMD5+strlen(szCheckMD5),"order=%s",getParamByKey("order",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&money=%s",getParamByKey("money",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&mid=%s",getParamByKey("mid",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&time=%s",getParamByKey("time",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&result=%s",getParamByKey("result",szParam,szCharRtl));
	sprintf(szCheckMD5+strlen(szCheckMD5),"&ext=%s&key=nhN824C8DVpn",getParamByKey("ext",szParam,szCharRtl));

	Cmd5Capi	md5Capi;
	char* szMd5	= md5Capi.Digest(szCheckMD5);

	szTmp	= getParamByKey("signature",szParam,szCharRtl);
	if(strcmp(szMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	szTmp	= getParamByKey("ext",szParam,szCharRtl);
	if(strlen(szTmp) == 0)
		return false;
	char*	szSplit	= strstr(szTmp,"#");
	char	szSvr[32] = {0};
	memcpy(szSvr,szTmp,szSplit-szTmp);
	int	nSvrID	= atoi(szSvr);
	char	szOrder[256];	//订单号
	sprintf(szOrder,"adrdjoy%s",szSplit+1);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"adrdjoy?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("money",szParam,szCharRtl);
		if(strlen(szTmp)==0)
			return false;
		char	szRMB[64];
		strcpy(szRMB,szTmp);
		char* szTmpC	= strstr(szRMB,".");
		if(szTmpC)
			szTmpC[0]	= 0;
		int nPayVal	= atoi(szRMB);
		if(nPayVal == 0)
			return false;
		char	szAcc[64];
		szTmp	= getParamByKey("mid",szParam,szCharRtl);

		sprintf(szAcc,"adrdjoy%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRDJOY))
			return true;
		else
			return false;
	}
	//else
	//{
	//	return false;
	//}

	return true;
}
bool HttpCenter::PPSPayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char	szCharRtl2[1024]	= {0};
	char*	szTmp	= getParamByKey("status",szParam,szCharRtl);
	if(atoi(szTmp) != 0)
	{//失败
		return false;
	}

	char depuk[1024] =  "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAv7yVxGMBqguQzuWPgzYB"
		"Wc3pedxHIukAXYUYuyxOFN+TUqgtvxr9HbnDyYiMbUHctXjp+RRrIs2j+hv7gIdX"
		"nafg/Uo3TSC4qRNxBhN3DSul+Abno4Yqktem3BDa79TTa56LdRMRZpRzZD7azmNO"
		"bcYFsNAG0Vdi/gSUBRoj6RTYe9X3XqROcpWBtQRcvdR27zmhGxZydS6M51yOLMuu"
		"noyOH7ZPVKngELsdvoAkfGJ2ricdZb4lhm7ELbhVpCKDerjgAMuvdpy6ZgkyHOE7"
		"URLu3Kzs/AQWaIfezapXolsIr56KX/xyByzKzbET01DJcFeQU0+gZr5Uh7gnPgYf"
		"TwIDAQAB";

	unsigned int depuklen;
	unsigned char *puk = base64Decode(depuk,depuklen);
	szTmp	= getParamByKey("sign",szParam,szCharRtl);//"Z7y6zRFzExubI64NE2OL7R90bXpGhvV4jMH7FuHh0Hf%2BuL6XeZ3cyawR6tYQnKgqOpvBya21jfudsJbKESyYlAnoQvxYirpyyr76VCLW%2FWY91%2FJd%2B1kQXI72wAIOZMdw31Tfam1EZZyzTNZZ%2BnS3cLz18Voe3Rsk3PDTvbIvBu681EOR7vu01RdrQ4lb3s%2Fj3D%2FFiTjsmAcAFGmshrGnq9eLx25NdfzAC8Y3M3LYhlyIVOH4aw6db98cp1jtof%2BK8oc2wPRUDSaFn71Hikmbz6W0zQXEtsHj%2BtUm0B2yw2xFCZSjnTe3AQrdfy00gbkfP%2F4zGNGI1qPf%2Bwpua%2F%2BPaA%3D%3D";
	char	szUrlD[2048]	= {0};
	UrlDecode(szTmp,szUrlD);
	unsigned int demlen;
	unsigned char *de = base64Decode(szUrlD,demlen);
	unsigned char testtext[1024] = {0};
	unsigned int txtlen;

	CX509 rsa;
	memset(testtext,0,1024);
	txtlen = rsa.DoEnde(puk,depuklen,de,demlen,testtext,1024,EX_PUBLICKEY_DE);

	char*	szTmpA	= getParamByKey("order_id",szParam,szCharRtl);
	char*	szTmpB	= getRtlByKey("order_id",(char*)testtext,szCharRtl2);
	if(szTmpB==NULL || strlen(szTmpA) == 0 || strcmp(szTmpA,szTmpB)!=0)
		return false;

	//获取目标服务器编号
	szTmp	= getParamByKey("zone",szParam,szCharRtl);
	if(strlen(szTmp) == 0)
		return false;
	int	nSvrID	= atoi(szTmp);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"ppspay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("amount",szParam,szCharRtl);
		
		if(strlen(szTmp)==0)
			return false;
		char	szRMB[64] = {0};
		strncpy(szRMB,szTmp,60);
		char* szTmpC	= strstr(szRMB,".");
		if(szTmpC)
			szTmpC[0]	= 0;
		int nPayVal	= atoi(szRMB);

		char	szOrder[512];	//订单号
		szTmp	= getParamByKey("order_id",szParam,szCharRtl);
		if(strlen(szTmp)==0)
			return false;
		sprintf(szOrder,"pps%s",szTmp);
		szTmp	= getParamByKey("roleid",szParam,szCharRtl);
		if(strlen(szTmp)==0)
			return false;
		char	szAcc[64] = {0};	//玩家账号
		strncpy(szAcc,szTmp,60);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_PPSPAY))
			return true;
		else
			return false;
	}
	//else
	//{
	//	return false;
	//}

	return true;
}

bool HttpCenter::SKPayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	//MD5加密验证
	Cmd5Capi	md5Capi;
	char	szMd5Src[512]	= {0};
	memset(szMd5Src,0,512);
	char*	szEnd	= strstr(szParam,"&signMsg");
	memcpy(szMd5Src,szParam,szEnd-szParam);
	sprintf(szMd5Src+strlen(szMd5Src),"&key=sri47654!@47386658679");
	char* szMd5	= md5Capi.Digest(szMd5Src);
	char*	szTmp	= getParamByKey("signMsg",szParam,szCharRtl);
	if(stricmp(szMd5,szTmp) != 0)
	{
		g_pLogFile->OutPut("HTTP ERROR: %s",szParam);
		return false;
	}

	//错误过滤
	szTmp	= getParamByKey("resultCode",szParam,szCharRtl);
	if(strlen(szTmp) == 0 || atoi(szTmp) != 0)
		return false;

	//获取目标服务器编号
	szTmp	= getParamByKey("ext2",szParam,szCharRtl);
	int	nSvrID	= atoi(szTmp);
	
	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"adrsky?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
		{//转发成功,记录日志
			return true;
		}
		else
		{//服务器未开启之类的原因转发失败了
			g_pLogFile->OutPut("HTTP Link Acc[%s]Failed: %s",pCAcc->szHttpIP,szParam);
			return false;
		}
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getParamByKey("realAmount",szParam,szCharRtl);
		if(szTmp == NULL)
			return false;
		int nPayVal	= atoi(szTmp)/100;
		char	szOrder[256];	//订单号
		szTmp	= getParamByKey("payNum",szParam,szCharRtl);
		if(szTmp == NULL)
			return false;
		sprintf(szOrder,"sk%s",szTmp);
		szTmp	= getParamByKey("ext1",szParam,szCharRtl);
		if(szTmp == NULL)
			return false;
		char	szAcc[64];	//玩家账号
		sprintf(szAcc,"%s",szTmp);

		szTmp	= getParamByKey("ext3",szParam,szCharRtl);
		int nTPID	=	0;
		if(szTmp)
			nTPID	= atoi(szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_SKPAY+nTPID))
			g_pLogFile->OutPut("HTTP PayOK Acc=%s Order=%s money=%d",szAcc,szOrder,nPayVal);
		else
			g_pLogFile->OutPut("HTTP PayFailed Acc=%s Order=%s money=%d",szAcc,szOrder,nPayVal);
		return true;
	}
	//else
	//{
	//	g_pLogFile->OutPut("HTTP PayFailed MSG=%s",szParam);
	//}
	return false;
}

void HttpCenter::SendToMainAcc( char* szMsg )
{//转发用户账号中心
	g_pLogFile->OutPut("HTTP SendToMainAcc ip%s port=%d  MSG=%s",ServerCfg::GetInstance().szMainIP,ServerCfg::GetInstance().nMainPort,szMsg);
	requestbyIp(ServerCfg::GetInstance().szMainIP,ServerCfg::GetInstance().nMainPort,szMsg);
}

bool HttpCenter::InviteDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	//获取目标服务器编号
	char* szTmp	= getParamByKey("svr",szParam,szCharRtl);
	if(szTmp == NULL)
		return false;
	int	nSvrID	= atoi(szTmp);
	szTmp	= getParamByKey("event",szParam,szCharRtl);
	if(szTmp == NULL)
		return false;
	int	nEvent	= atoi(szTmp);
	szTmp	= getParamByKey("invcode",szParam,szCharRtl);
	if(szTmp == NULL)
		return false;

	g_pMainAccount->RecvInviteCmd(nSvrID,szTmp,nEvent);
}

void HttpCenter::SendToChildAcc( char* szMsg,int nSvr )
{
	
	_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvr);
	if(pCAcc)
	{//转发给其他服务器
		g_pLogFile->OutPut("HTTP SendToChildAcc ip%s port=%d  MSG=%s",pCAcc->szHttpIP,pCAcc->nHttpPort,szMsg);
		requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szMsg);
	}
}

bool HttpCenter::TPLogin(char* szUin, char* szToken, BYTE nTPlat, OUT char* szAcc,OUT char* szPswd,OUT char* szBKey )
{
	char	szCharRtl[1024]	= {0};
	char	szCharRtl2[1024]	= {0};
	char	szRecv[1024]	= {0};
	char	szSign[1024]	= {0};
	char	szUrlDe[512]	= {0};
	switch(nTPlat)
	{
	case TP_PPSPAY:
		{
			requestTPlat(true,"passport_i.25pp.com",8080,"/index?tunnel-command=2852126756",szToken,szRecv,TP_PPSPAY);
			char*	szRtl	= getRtlByKey("status",szRecv,szCharRtl2);
			if(szRtl && atoi(szRtl) == 0)
			{
				szRtl	= getRtlByKey("userid",szRecv,szCharRtl2);
				if(szRtl != NULL)
				{
					int nUid	= atoi(szRtl);
					if(nUid)
					{
						sprintf(szAcc,"pps%d",nUid);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}
			}
			else
			{
				g_pLogFile->OutPut("PPSPAY login faild :%s",szToken);
			}
		}
		return false;
	case TP_ADRBDDK:
		{
			sprintf(szSign,"4140b1e282773555633b06d8f727889cb5c9%s%s1cfd65fe75cf7e1152a59a953a468166", szUin,szToken);

			Cmd5Capi	md5Capi;
			char* szMd5	= md5Capi.Digest(szSign);

			sprintf(szSign,"/openapi/sdk/checksession?appid=4140&appkey=b1e282773555633b06d8f727889cb5c9&uid=%s&sessionid=%s&clientsecret=%s",szUin,szToken,szMd5);
			requestTPlat(false,"sdk.m.duoku.com",80,szSign,"",szRecv,TP_ADRBDDK);
			char*	szRtl	= getRtlByKey("error_code",szRecv,szCharRtl2);
			if(szRtl && strlen(szRtl) && atoi(szRtl)==0)
			{
				sprintf(szAcc,"bddk%s",szUin);
				szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;
			}
			else
				g_pLogFile->OutPut("ADRBDDK login faild(%s) :%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRMMY:
		{
			//http://pay.mumayi.com/user/index/validation
			sprintf(szUrlDe, "/user/index/validation?token=%s&uid=%s", szToken,szUin);
			requestTPlat(false, "pay.mumayi.com", 80, szUrlDe, "", szRecv, TP_ADRMMY);
			if(strlen(szRecv) && strnicmp(szRecv,"success",7)==0)
			{
				
					if(szUin && strlen(szUin) > 3)
					{
						if(strlen(szUin) > 16)
							sprintf(szAcc,"mmy%s",szUin+strlen(szUin)-16);
						else
							sprintf(szAcc,"mmy%s",szUin);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				
			}

			g_pLogFile->OutPut("ADRMMY login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRYL:
		{
			//http://api.411game.com/validation.do
			Cmd5Capi	md5Capi;
			char szMd[256];
			sprintf(szMd, "%s103857e5c3c0a0f785653eea50184f8ed122bf", szToken);
			char* szMd2	= md5Capi.Digest(szMd);
			strupr(szMd2);
			sprintf(szUrlDe, "/validation.do?UserName=%s&PID=103857&flag=%s", szToken, szMd2);
			requestTPlat(true, "api.411game.com", 80, szUrlDe, szToken, szRecv, TP_ADRYL);
			if(strlen(szRecv))
			{
				char*	szTmp	= getRtlByKey("userState",szRecv,szCharRtl);
				if(szTmp && atoi(szTmp)==1)
				{
					char* szUin	= getRtlByKey("userName",szRecv,szCharRtl);
 					if(szUin && strlen(szUin) > 3)
 					{
 						if(strlen(szUin) > 16)
 							sprintf(szAcc,"yl%s",szUin+strlen(szUin)-16);
 						else
 							sprintf(szAcc,"yl%s",szUin);
 						Cmd5Capi	md5Capi1;
 						char* szMd5	= md5Capi1.Digest(szAcc);
 						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}
			}
			g_pLogFile->OutPut("ADRYL login faild(%s):%s",szRecv,szToken);
		}
		return false;
	case TP_ADRYYH:
		{
			sprintf(szUrlDe,"/appchina-usersdk/user/get.json?app_id=10240&app_key=sKn2Uhtl1j2QO0Hs&ticket=%s",szToken);
			requestTPlat(false,"api.appchina.com",80,szUrlDe,"",szRecv,TP_ADRYYH);
			if(strlen(szRecv))
			{
				char* szHRtl	= getRtlByKey("status",szRecv,szCharRtl);
				if(szHRtl && atoi(szHRtl) == 0)
				{
					char* szUin	= getRtlByKey("user_id",szRecv,szCharRtl);
					if(szUin && strlen(szUin) > 3)
					{
						if(strlen(szUin) > 16)
							sprintf(szAcc,"yyh%s",szUin+strlen(szUin)-16);
						else
							sprintf(szAcc,"yyh%s",szUin);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}
			}

			g_pLogFile->OutPut("ADRYYH login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRJF:
		{
			//http://api.gfan.com/uc1/common/verify_token?token=iouaw7c5ss5
			sprintf(szUrlDe,"/uc1/common/verify_token?token=%s",szToken);
			requestTPlat(false,"api.gfan.com",80,szUrlDe,"",szRecv,TP_ADRJF);
			if(strlen(szRecv))
			{
				char* szHRtl	= getRtlByKey("resultCode",szRecv,szCharRtl);
				if(szHRtl && atoi(szHRtl) == 1)
				{
					char* szUin	= getRtlByKey("uid",szRecv,szCharRtl);
					if(szUin && strlen(szUin) > 3)
					{
						if(strlen(szUin) > 16)
							sprintf(szAcc,"jf%s",szUin+strlen(szUin)-16);
						else
							sprintf(szAcc,"jf%s",szUin);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}
			}

			g_pLogFile->OutPut("ADRJF login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRMI:
		{
			//http://mis.migc.xiaomi.com/api/biz/service/verifySession.do
			char	szGUin[64]	= {0};
			char	szGToken[255]	= {0};
			memset(szGToken,0,255);

			char* szGTmp	= strstr(szToken,"#!#");
			if(szGTmp)
			{
				strcpy(szGUin,szGTmp+3);
				memcpy(szGToken,szToken,szGTmp-szToken);
			}
			else
			{
				strcpy(szGUin,szUin);
				strcpy(szGToken,szToken);
			}

			sprintf(szSign,"appId=26632&session=%s&uid=%s",szGToken,szGUin);
			//sprintf(szSign,"appId=2882303761517225896&session=%s&uid=%s",szToken,szUin);
			//signature

			//调用方法："b3e8e03c-92b8-1d13-9232-5396cbe5da64"
			char*  keySrc ="b3e8e03c-92b8-1d13-9232-5396cbe5da64";
			//char*  keySrc ="5451722573896";

			char  sha1Str[1024] = {0};
			char	szGN[256]	= {0};
			int		nSize	= 0;
			char	szTmp111[1024]	= {0};
			hmac_sha(keySrc,strlen(keySrc),szSign,strlen(szSign),szGN,20);
			byte2hex(szGN,strlen(szGN),szTmp111);
			
			sprintf(szUrlDe,"/api/biz/service/verifySession.do?%s&signature=%s",szSign,szTmp111);
			requestTPlat(false,"mis.migc.xiaomi.com",80,szUrlDe,"",szRecv,TP_ADRMI);
			if(strlen(szRecv))
			{
				char* szHRtl	= getRtlByKey("errcode",szRecv,szCharRtl);
				if(szHRtl && (atoi(szHRtl) == 200 || atoi(szHRtl) == 1525))
				{
					if(strlen(szGUin) > 16)
						sprintf(szAcc,"mi%s",szGUin+strlen(szGUin)-16);
					else
						sprintf(szAcc,"mi%s",szGUin);
					Cmd5Capi	md5Capi;
					char* szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}


			g_pLogFile->OutPut("ADRMI login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRAZ:
		{
			sprintf(szSign,"1402387483VYF0uzAJm909tgjI5Pbv%s95WjE52o22L5xmi8sHWHoMW0",szToken);
			char	szB64[1024]	= {0};
			Base64_Encode(szB64,szSign,strlen(szSign));

			char	szTime[64]	= {0};
			tm tmNow = CMTime::GetCurTime();
			sprintf(szTime,"%04d%02d%02d%02d%02d%02d000",tmNow.tm_year+1900,tmNow.tm_mon+1,tmNow.tm_mday,tmNow.tm_hour,tmNow.tm_min,tmNow.tm_sec);

			sprintf(szUrlDe,"/web/api/sdk/third/1/queryislogin?time=%s&appkey=1402387483VYF0uzAJm909tgjI5Pbv&sid=%s&sign=%s",szTime,szToken,szB64);
			requestTPlat(true,"user.anzhi.com",80,szUrlDe,"",szRecv,TP_ADRAZ);
			if(strlen(szRecv) > 10)
			{
				char* szTmp	= getRtlByKey("sc",szRecv,szCharRtl2);
				if(szTmp != NULL && strstr( szTmp,"1"))
				{
					szTmp	= getRtlByKey("msg",szRecv,szCharRtl2);
					//Base64_Decode(szBaseD,szTmp, strlen(szTmp));
					if(szTmp[0] == '\'')
						szTmp++;
					if(szTmp[strlen(szTmp)-1] == '\'')
						szTmp[strlen(szTmp)-1]	= 0;
					unsigned int nLen;
					unsigned char *szBaseD = base64Decode(szTmp,nLen);
					char*	szRtl	= getRtlByKey("uid",(char*)szBaseD,szCharRtl2);
					if(szRtl != NULL && strlen(szRtl) > 3)
					{
						if(szRtl[0] == '\'')
							szRtl++;
						if(szRtl[strlen(szRtl)-1] == '\'')
							szRtl[strlen(szRtl)-1]	= 0;
						if(strlen(szRtl) > 16)
							sprintf(szAcc,"az%s",szRtl+strlen(szRtl)-16);
						else
							sprintf(szAcc,"az%s",szRtl);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}
			}
			g_pLogFile->OutPut("ADRAZ login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRPPS:
		{
			//获取目标服务器编号
			char*	szSplit	= strstr(szToken,"#");
			char	szUID[32] = {0};
			if(szSplit == NULL)
				return false;
			memcpy(szUID,szToken,szSplit-szToken);
			
			char*	szMD5	= strstr(szSplit+1,"#");
			if(szMD5 == NULL)
				return false;
			char	szTime[64] = {0};
			memcpy(szTime,szSplit+1,szMD5-szSplit-1);
			szMD5++;

			sprintf(szSign,"%s&%s&74974bf301ff7e270d0e1e6860735f38",szUID,szTime);
			Cmd5Capi	md5Capi;
			char* szCMd5	= md5Capi.Digest(szSign);
			if(stricmp(szCMd5,szMD5) == 0)
			{
				if(strlen(szUID) > 12)
					sprintf(szAcc,"pps%s",szUID+strlen(szUID)-12);
				else
					sprintf(szAcc,"pps%s",szUID);

				char* szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;
				
			}
			g_pLogFile->OutPut("ADRPPS login faild() :%s:%s",szUin,szToken);
		}
		return false;
	case TP_ADRMZW:
		{
			//http://sdk.muzhiwan.com/oauth2/getuser.php
			sprintf(szSign,"/oauth2/getuser.php?token=%s&appkey=14e5fed2fa42054886f726739b83d418",szToken);
			requestTPlat(false,"sdk.muzhiwan.com",80,szSign,"",szRecv,TP_ADRMZW);
			char*	szRtl	= getRtlByKey("code",szRecv,szCharRtl2);
			if(szRtl && strlen(szRtl) && atoi(szRtl)==1)
			{
				char* szUID	= getRtlByKey("uid",szRecv,szCharRtl2);
				if(szUID && strlen(szUID))
				{
					if(strlen(szUID) > 12)
						sprintf(szAcc,"mzw%s",szUID+strlen(szUID)-12);
					else
						sprintf(szAcc,"mzw%s",szUID);
					Cmd5Capi	md5Capi;
					char* szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}
			g_pLogFile->OutPut("MZW login faild(%s) :%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRMZ:
		{
			//http://app.5gwan.com:9000/user/info.php?sign=e7dd6bb3f0845f5f416a76
			//7524dc83c0&token=b3066822e277f30638966f3e23719de2&app_id=a001 

			sprintf(szSign,"7fd374b52d09ddb508f46fbaab5ebcca_%s",szToken);

			Cmd5Capi	md5Capi;
			char* szMd5A	= strdup(md5Capi.Digest(szSign));
			char* szMd5	= md5Capi.Digest(szMd5A);
			free(szMd5A);

			sprintf(szSign,"/user/info.php?sign=%s&token=%s&app_id=A726",szMd5,szToken);
			requestTPlat(false,"app.5gwan.com",9000,szSign,"",szRecv,TP_ADRMZ);
			char*	szRtl	= getRtlByKey("state",szRecv,szCharRtl2);
			if(szRtl && strlen(szRtl) && atoi(szRtl)==1)
			{
				char* szUID	= getRtlByKey("userid",szRecv,szCharRtl2);
				if(szUID && strlen(szUID))
				{
					if(strlen(szUID) > 12)
						sprintf(szAcc,"adrmz%s",szUID+strlen(szUID)-12);
					else
						sprintf(szAcc,"adrmz%s",szUID);
					szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}
			
			g_pLogFile->OutPut("ADRMZ login faild(%s) :%s:%s",szRecv,szUin,szToken);

		}
		return false;
	case TP_ADRHUAWEI:
		{
			char	szUrlE[256];
			UrlEncode(szToken,szUrlE,256);
			DWORD	dwTickSec	= CMTime::GetCurTickCount();
			sprintf(szUrlDe,"/login/huaweilogin.php?nsp_svc=OpenUP.User.getInfo&nsp_ts=%d&access_token=%s",dwTickSec,szUrlE);
			requestTPlat(false,"www.mejoygame.com",80,szUrlDe,"",szRecv,TP_ADRHUAWEI);
			if(szRecv && strlen(szRecv))
			{
				char* szUID	= getRtlByKey("userID",szRecv,szCharRtl);
				if(szUID && strlen(szUID) > 3)
				{
					if(strlen(szUID) > 12)
						sprintf(szAcc,"adrhw%s",szUID+strlen(szUID)-12);
					else
						sprintf(szAcc,"adrhw%s",szUID);
					Cmd5Capi	md5Capi;
					char* szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}
			g_pLogFile->OutPut("ADRHUAWEI login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRLX:
		{
			//http://passport.lenovo.com/interserver/authen/1.2/getaccountid?lpsust=
			//char	szUrlEn[512];
			//UrlEncode(szToken,szUrlEn,512);
			sprintf(szUrlDe,"/interserver/authen/1.2/getaccountid?lpsust=%s&realm=1406260014602.app.ln",szToken);

			requestTPlat(false,"passport.lenovo.com",80,szUrlDe,"",szRecv,TP_ADRLX);
			if(strlen(szRecv))
			{
				char*	szTmpL1	= strstr(szRecv,"AccountID>");
				char*	szTmpL2	= strstr(szTmpL1,"</AccountID>");
				char	szRID[256]	= {0};
				if(szTmpL1 &&  szTmpL2)
				{
					memcpy(szRID,szTmpL1+10,szTmpL2-szTmpL1-10);
					if(strlen(szRID) > 3)
					{
						sprintf(szAcc,"adrlx%s",szRID);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}

			}
			g_pLogFile->OutPut("ADRLX login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADRWDJ:
		{
			//String uidCheckUrl = "https://pay.wandoujia.com/api/uid/check";
			//String url=uidCheckUrl+"?uid="+user.getUid()+"&token="+ URLEncoder.encode(user.getToken())+"&appkey_id="+appkey_id;
			char	szUrlE[256];
			UrlEncode(szToken,szUrlE,256);
			sprintf(szUrlDe,"/login/wdjlogin.php?uid=%s&token=%s&appkey_id=100007395",szUin,szUrlE);
			requestTPlat(false,"www.mejoygame.com",80,szUrlDe,"",szRecv,TP_ADRWDJ);
			if(strstr(szRecv,"true"))
			{				
				sprintf(szAcc,"wdj%s",szUin);
				Cmd5Capi	md5Capi;
				char* szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;
			}
			g_pLogFile->OutPut("ADRWDJ login faild(%s):%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADR360:
		{
			//https://openapi.360.cn/oauth2/access_token?grant_type=authorization_code&code=120653f48687
			//763d6ddc486fdce6b51c383c7ee544e6e5eab&client_id=0fb2676d5007f123756d1c1b4b5968bc&cli
			//	ent_secret=1234567890ab18384f562d7d3f.....&redirect_uri=oob 
			sprintf(szUrlDe,"/login/360login1.php?grant_type=authorization_code&code=%s&client_id=66a6683dfa017ced023a25059348cc67&client_secret=42690aef7647a783bce3498ce37d80f8&redirect_uri=oob",szToken);
			requestTPlat(false,"www.mejoygame.com",80,szUrlDe,"",szRecv,TP_ADR360);

			if(strlen(szRecv))
			{
				char* szRecvToken	= getRtlByKey("access_token",szRecv,szCharRtl2);
				strcpy(szBKey, szRecvToken);
				if(szRecvToken && strlen(szRecvToken))
				{
					//https://openapi.360.cn/user/me.json?access_token=12345678983b38aabcdef387453ac8133ac3263987654321&fields=id,name,avatar,sex,area 
					sprintf(szUrlDe,"/login/360login2.php?access_token=%s&fields=id,name",szRecvToken);
					requestTPlat(false,"www.mejoygame.com",80,szUrlDe,"",szRecv,TP_ADR360);

					if(strlen(szRecv) > 1)
					{
						char* szRID	= getRtlByKey("id",szRecv,szCharRtl2);
						if(szRID && strlen(szRID) > 3)
						{
							sprintf(szBKey+strlen(szBKey),"#%s",szRID);
							sprintf(szAcc,"adr360%s",szRID);
							Cmd5Capi	md5Capi;
							char* szMd5	= md5Capi.Digest(szAcc);
							strncpy(szPswd,szMd5+1,12);
							return true;
						}
					}
				}
			}

			g_pLogFile->OutPut("ADR360 login faild(%s):%s:%s",szRecv,szUin,szToken);



		}
		return false;
	case TP_ADROPPO:
		{
			char	szPT[100]	= {0};
			char	szPS[100]	= {0};
			strcpy(szPT,getParamByKey("oauth_token",szToken,szCharRtl));
			strcpy(szPS,getParamByKey("oauth_token_secret",szToken,szCharRtl));

			sprintf(szSign,"/login/oppologin.php?params=%s&paramt=%s",szPS,szPT);
			requestTPlat(false,"www.mejoygame.com",80,szSign,"",szRecv,TP_ADROPPO);
			char*	szRtl	= getRtlByKey("id",szRecv,szCharRtl2);
			if(szRtl && strlen(szRtl))
			{
				sprintf(szAcc,"oppo%s",szRtl);
				Cmd5Capi	md5Capi;
				char* szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;

			}
			else
				g_pLogFile->OutPut("ADROPPO login faild(%s) :%s:%s",szRecv,szUin,szToken);
		}
		return false;
	case TP_ADR91:
		{
			//APPID：114610
			//APPKEY：d16858ac0349955b07e541e32e847af8df69c68fd5fd0a55
			sprintf(szSign,"1146104%s%sd16858ac0349955b07e541e32e847af8df69c68fd5fd0a55", szUin,szToken);

			Cmd5Capi	md5Capi;
			char* szMd5	= md5Capi.Digest(szSign);
			sprintf(szSign,"/usercenter/AP.aspx?AppId=114610&Act=4&Uin=%s&Sign=%s&SessionID=%s",szUin,szMd5,szToken);
			requestTPlat(false,"service.sj.91.com",80,szSign,"",szRecv,TP_ADR91);
			char*	szRtl	= getRtlByKey("ErrorCode",szRecv,szCharRtl2);
			if(szRtl && atoi(szRtl) == 1)
			{
				sprintf(szAcc,"nd91%s",szUin);
				szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;

			}
			else
			{
				g_pLogFile->OutPut("ADR91 login faild(%s) :%s:%s",szRecv,szUin,szToken);
			}
		}
		return false;
	case TP_91PAY:
		{//APPID+ACT=4
			sprintf(szSign,"1130164%s%se1f201c3cdb47c35b48144d6b4642abdaebfada017a310c9", szUin,szToken);

			Cmd5Capi	md5Capi;
			char* szMd5	= md5Capi.Digest(szSign);
			sprintf(szSign,"/usercenter/AP.aspx?AppId=113016&Act=4&Uin=%s&Sign=%s&SessionID=%s",szUin,szMd5,szToken);
			requestTPlat(false,"service.sj.91.com",80,szSign,"",szRecv,TP_91PAY);
			char*	szRtl	= getRtlByKey("ErrorCode",szRecv,szCharRtl2);
			if(szRtl && atoi(szRtl) == 1)
			{
				sprintf(szAcc,"tp91%s",szUin);
				szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;
				
			}
			else
			{
				g_pLogFile->OutPut("91PAY login faild(%s) :%s:%s",szRecv,szUin,szToken);
			}
		}
		return false;
	case TP_TBTPAY:
		{
			UrlDecode(szToken,szUrlDe);
			sprintf(szSign,"/check.aspx?k=%s",szUrlDe);
			requestTPlat(false,"tgi.tongbu.com",80,szSign,"",szRecv,TP_TBTPAY);
			if(atoi(szRecv) >= 1)
			{
				sprintf(szAcc,"tbt%s",szUin);
				Cmd5Capi	md5Capi;
				char* szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;
			}
			else
			{
				g_pLogFile->OutPut("TPPay login faild(%s) :%s:%s",szRecv,szUin,szUrlDe);
			}
		}
		return false;
	case TP_ADRDJOY:
		{
			Cmd5Capi	md5Capi;
			sprintf(szUrlDe,"%s|b1G5LiWt",szToken);
			char* szMd5	= md5Capi.Digest(szUrlDe);

			sprintf(szSign,"/open/member/info/?app_id=1455&mid=%s&token=%s&sig=%s",szUin,szToken,szMd5);
			requestTPlat(false,"connect.d.cn",80,szSign,"",szRecv,TP_ADRDJOY);
			if(strlen(szRecv) > 1)
			{
				char*	szTmp	= getRtlByKey("error_code",szRecv,szCharRtl);
				if(szTmp && atoi(szTmp)==0)
				{
					sprintf(szAcc,"adrdjoy%s",szUin);
					Cmd5Capi	md5Capi;
					char* szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}
			g_pLogFile->OutPut("ADrDJoy login faild :%s:%s",szUin,szToken);
		}
		return false;
	case TP_DJOYPAY:
		{
			Cmd5Capi	md5Capi;
			sprintf(szUrlDe,"%s|O4bT7WAL",szToken);
			char* szMd5	= md5Capi.Digest(szUrlDe);

			sprintf(szSign,"/open/member/info/?app_id=1456&mid=%s&token=%s&sig=%s",szUin,szToken,szMd5);
			requestTPlat(false,"connect.d.cn",80,szSign,"",szRecv,TP_DJOYPAY);
			if(strlen(szRecv) > 1)
			{
				char*	szTmp	= getRtlByKey("error_code",szRecv,szCharRtl);
				if(szTmp && atoi(szTmp)==0)
				{
					sprintf(szAcc,"djoy%s",szUin);
					Cmd5Capi	md5Capi;
					char* szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}
			g_pLogFile->OutPut("DJOYPay login faild :%s:%s",szUin,szToken);
		}
		return false;
	case TP_ADRUC:
		{
			//http://sdk.g.uc.cn/ss/bp/ucid;   //bp/ucid
			Cmd5Capi	md5Capi;

			sprintf(szUrlDe,"33271sid=%s040fdef4f48825ecc4d835d32d460b0d",szToken);

			char* szMd5	= md5Capi.Digest(szUrlDe);
			DWORD	dwCurTick	= CMTime::GetCurTickCount();
			sprintf(szSign,"{\"id\":%d,\"service\":\"ucid.user.sidInfo\",\"data\":{\"sid\":\"%s\"},\"game\":{\"cpId\":33271,\"gameId\":538942,\"channelId\":\"2\",\"serverId\":3125},\"sign\":\"%s\"}",dwCurTick,szToken,szMd5);

			requestTPlat(true,"sdk.g.uc.cn",80,"/ss",szSign,szRecv,TP_ADRUC);
			if(strlen(szRecv) > 1)
			{
				char*	szTmp	= getRtlByKey("code",szRecv,szCharRtl);

				if(szTmp && atoi(szTmp)==1)
				{
					szTmp	= getRtlByKey("ucid",szRecv,szCharRtl);
					if(szTmp && strlen(szTmp) > 3)
					{
						sprintf(szAcc,"uc%s",szTmp);
						Cmd5Capi	md5Capi;
						char* szMd5	= md5Capi.Digest(szAcc);
						strncpy(szPswd,szMd5+1,12);
						return true;
					}
				}
			}

			g_pLogFile->OutPut("UCPAY login faild :%s:%s",szUin,szToken);
		}
		return false;
	case TP_ITOOLPAY:
		{

			//https://pay.itools.cn/?r=auth/verify&appid=1&sessionid=122_3087c1291639190cb205a6dcd63d902c7e710c0b&sign=ed7f3ccda78c487485a7a552a65543a5
			Cmd5Capi	md5Capi;
			sprintf(szUrlDe,"appid=206&sessionid=%s",szToken);
			char* szMd5	= md5Capi.Digest(szUrlDe);

			sprintf(szSign,"/itoollogin.php?appid=206&sessionid=%s&sign=%s",szToken,szMd5);
			requestTPlat(false,"www.mejoygame.com",80,szSign,"",szRecv,TP_ITOOLPAY);
			if(strlen(szRecv) > 1)
			{
				if(strstr(szRecv,"success"))
				{
					sprintf(szAcc,"itool%s",szUin);
					Cmd5Capi	md5Capi;
					char* szMd5	= md5Capi.Digest(szAcc);
					strncpy(szPswd,szMd5+1,12);
					return true;
				}
			}
			g_pLogFile->OutPut("ITOOLPay login faild :%s:%s",szUin,szToken);
		}
		return false;
	case TP_KYPAY:
		{
			//http://f_signin.bppstore.com/loginCheck.php?tokenKey=a43cd7a510fc3b06792a0cb509b58415&sign=aee0f84886d038b931a7bedab9d8040f
			Cmd5Capi	md5Capi;
			sprintf(szSign,"402d4a1371b4534017672fa4039233c0%s",szToken);
			char* szMd5	= md5Capi.Digest(szSign);
			sprintf(szUrlDe,"/loginCheck.php?tokenKey=%s&sign=%s",szToken,szMd5);
			requestTPlat(false,"f_signin.bppstore.com",80,szUrlDe,"",szRecv,TP_KYPAY);

			char*	szRtl	= getRtlByKey("code",szRecv,szCharRtl2);
			if(szRtl && atoi(szRtl) == 0)
			{//登录成功
				szRtl	= getRtlByKey("guid",szRecv,szCharRtl2);
				if(szRtl==NULL)
					return false;
				sprintf(szAcc,"ky%s",szRtl);
				szMd5	= md5Capi.Digest(szAcc);
				strncpy(szPswd,szMd5+1,12);
				return true;
			}
		}
		return false;
	}
}

bool HttpCenter::_checkABOk(char *sKey,char *pTransData,char *pSign)
{
	char szCompKey[256];
	char szPrivateKey[256];
	char szModKey[256];

	memset(szCompKey,0,sizeof(szCompKey));
	memset(szPrivateKey,0,sizeof(szPrivateKey));
	memset(szModKey,0,sizeof(szModKey));

	gIAppBase64Decode(sKey,szCompKey);
	char *pSplit;
	pSplit = strchr(szCompKey,'+');
	strcpy(szModKey,pSplit+1);   //modkey

	strncpy(szPrivateKey,szCompKey,pSplit-szCompKey); //privatekey
	//开始验证
	char *pSignMD5;  //签名值的md5
	int nlen;
	pSignMD5 = gIAppDecRSA(&nlen,pSign,strlen(pSign),szPrivateKey,szModKey);

	char szOrgMD5[256];
	szOrgMD5[0]='\0';
	gIAppMD5((char*)pTransData,strlen(pTransData),(char*)szOrgMD5);

	return (strcmp(szOrgMD5,pSignMD5)==0);
}

BYTE HttpCenter::TPCheckIAPPay( char* szAcc,char* szToken )
{
	//https://sandbox.itunes.apple.com/verifyReceipt
	//https://buy.itunes.apple.com/verfyReceipt
	char	szRecv[1024]	= {0};
	char	szB64[4096]	= {0};
	Base64_Encode(szB64,szToken,strlen(szToken));
	requestTPlat(true,"www.mejoygame.com",80,"/iap.php",szB64,szRecv,TP_IAP);

	char	szCharTmp[1024]	= {0};
	char	szOrderID[256]	= {0};
	//char*	szRtl	= getRtlByKey("unique_identifier",szRecv,szCharTmp);
	//if(szRtl == NULL)
	//	return false;
	//sprintf(szOrderID,"IAP%s",szRtl);
	char*	szRtl	= getRtlByKey("transaction_id",szRecv,szCharTmp);
	if(szRtl == NULL)
		return 0;
	sprintf(szOrderID,"IAP%s",szRtl);
	//strcat(szOrderID,szRtl);
	szRtl	= getRtlByKey("product_id",szRecv,szCharTmp);
	if(szRtl == NULL)
		return 0;
	int	nRealRmb	= 0;
	if(strcmp(szRtl,"com.miyigame.yhzj.60gems") == 0)
	{
		nRealRmb	= 6;
	}
	else if(strcmp(szRtl,"com.miyigame.yhzj.300gems") == 0)
	{
		nRealRmb	= 30;
	}
	else if(strcmp(szRtl,"com.miyigame.yhzj.780gems") == 0)
	{
		nRealRmb	= 78;
	}
	else if(strcmp(szRtl,"com.miyigame.yhzj.2180gems") == 0)
	{
		nRealRmb	= 198;
	}
	else if(strcmp(szRtl,"com.miyigame.yhzj.3780gems") == 0)
	{
		nRealRmb	= 328;
	}
	else if(strcmp(szRtl,"com.miyigame.yhzj.7780gems") == 0)
	{
		nRealRmb	= 648;
	}
	else
		return 9;
	if(g_pMainAccount->HttpPayRequest(szAcc,szOrderID,nRealRmb,TP_IAP))
		return 1;
	else
		return 9;

	return true;
}

bool HttpCenter::DianRuCheck( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("drkey",szParam,szCharRtl);

	if(szTmp == NULL || strlen(szTmp) <= 32)
		return false;

	char	szMac[32]	= {0};
	char	szIDFA[50]	= {0};
	char	szEx[50]	= {0};
	memset(szEx,0,50);
	memcpy(szEx,szTmp,32);
	if(strlen(szTmp) > 60)
	{//IDFA
		strcpy(szIDFA,szTmp+32);
	}
	else
	{//MAC
		strcpy(szMac,szTmp+32);
	}

	g_pMainAccount->RecvIosAD(szMac,szIDFA,"dianru",szEx);
	return true;
}

bool HttpCenter::DuoMengCheck( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("appId",szParam,szCharRtl);

	if(szTmp == NULL || atoi(szTmp) != 843125355)
		return false;

	char	szMac[32]	= {0};
	char	szIDFA[50]	= {0};
	char	szEx[50]	= {0};
	szTmp	= getParamByKey("mac",szParam,szCharRtl);
	strcpy(szMac,szTmp);
	szTmp	= getParamByKey("ifa",szParam,szCharRtl);
	strcpy(szIDFA,szTmp);

	szTmp	= getParamByKey("source",szParam,szCharRtl);
	if(strstr(szTmp,"domob_jifen"))
		g_pMainAccount->RecvIosAD(szMac,szIDFA,"duomeng",szEx);
	else
		g_pMainAccount->RecvIosAD(szMac,szIDFA,"duomengA",szEx);
	return true;
}

bool HttpCenter::WanPuCheck( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("app",szParam,szCharRtl);

	if(szTmp == NULL || atoi(szTmp) != 843125355)
		return false;

	char	szMac[32]	= {0};
	char	szIDFA[50]	= {0};
	char	szEx[50]	= {0};
	szTmp	= getParamByKey("udid",szParam,szCharRtl);
	strcpy(szMac,szTmp);
	szTmp	= getParamByKey("idfa",szParam,szCharRtl);
	strcpy(szIDFA,szTmp);
	g_pMainAccount->RecvIosAD(szMac,szIDFA,"wanpu",szEx);
	return true;
}

bool HttpCenter::LiMeiCheck( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("app",szParam,szCharRtl);

	if(szTmp == NULL || atoi(szTmp) != 843125355)
		return false;

	char	szMac[32]	= {0};
	char	szIDFA[50]	= {0};
	char	szEx[255]	= {0};
	szTmp	= getParamByKey("udid",szParam,szCharRtl);

	if(strlen(szTmp) > 30)
	{//IDFA
		strcpy(szIDFA,szTmp);
	}
	else
	{
		strcpy(szMac,szTmp);
	}

	szTmp	= getParamByKey("pburl",szParam,szCharRtl);
	UrlDecode(szTmp,szEx);
	g_pMainAccount->RecvIosAD(szMac,szIDFA,"limei",szEx);
	return true;
}

bool HttpCenter::YouMiCheck( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	//char*	szTmp	= getParamByKey("appid",szParam,szCharRtl);

	//if(szTmp == NULL || atoi(szTmp) != 843125355)
	//	return false;

	char	szMac[32]	= {0};
	char	szIDFA[50]	= {0};
	char	szEx[255]	= {0};

	strcpy(szIDFA,getParamByKey("ifa",szParam,szCharRtl));
	strcpy(szMac,getParamByKey("mac",szParam,szCharRtl));

	char* szTmp	= getParamByKey("callback_url",szParam,szCharRtl);
	UrlDecode(szTmp,szEx);
	g_pMainAccount->RecvIosAD(szMac,szIDFA,"youmi",szEx);
	return true;
}

bool HttpCenter::AdsageCheck( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	//char*	szTmp	= getParamByKey("appid",szParam,szCharRtl);

	//if(szTmp == NULL || atoi(szTmp) != 843125355)
	//	return false;

	char	szMac[32]	= {0};
	char	szIDFA[50]	= {0};
	char	szEx[255]	= {0};

	strcpy(szIDFA,getParamByKey("idfa",szParam,szCharRtl));
	strcpy(szMac,getParamByKey("mac",szParam,szCharRtl));

	char* szTmp	= getParamByKey("callback",szParam,szCharRtl);
	UrlDecode(szTmp,szEx);
	g_pMainAccount->RecvIosAD(szMac,szIDFA,"adsage",szEx);
	return true;
}

bool HttpCenter::IToolPayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};
	char	szCharRtl2[1024]	= {0};
	
	char depuk[1024] =  "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC2kcrRvxURhFijDoPpqZ/IgPlA"
		"gppkKrek6wSrua1zBiGTwHI2f+YCa5vC1JEiIi9uw4srS0OSCB6kY3bP2DGJagBo"
		"Egj/rYAGjtYJxJrEiTxVs5/GfPuQBYmU0XAtPXFzciZy446VPJLHMPnmTALmIOR5"
		"Dddd1Zklod9IQBMjjwIDAQAB";

	unsigned int depuklen;
	unsigned char *puk = base64Decode(depuk,depuklen);

	char*	szTmp	= getParamByKey("notify_data",szParam,szCharRtl);//"Z7y6zRFzExubI64NE2OL7R90bXpGhvV4jMH7FuHh0Hf%2BuL6XeZ3cyawR6tYQnKgqOpvBya21jfudsJbKESyYlAnoQvxYirpyyr76VCLW%2FWY91%2FJd%2B1kQXI72wAIOZMdw31Tfam1EZZyzTNZZ%2BnS3cLz18Voe3Rsk3PDTvbIvBu681EOR7vu01RdrQ4lb3s%2Fj3D%2FFiTjsmAcAFGmshrGnq9eLx25NdfzAC8Y3M3LYhlyIVOH4aw6db98cp1jtof%2BK8oc2wPRUDSaFn71Hikmbz6W0zQXEtsHj%2BtUm0B2yw2xFCZSjnTe3AQrdfy00gbkfP%2F4zGNGI1qPf%2Bwpua%2F%2BPaA%3D%3D";
	char	szUrlD[2048]	= {0};
	UrlDecode(szTmp,szUrlD);
	unsigned int demlen;
	unsigned char *de = base64Decode(szUrlD,demlen);
	unsigned char testtext[1024] = {0};
	unsigned int txtlen;

	CX509 rsa;
	memset(testtext,0,1024);
	txtlen = rsa.DoEnde(puk,depuklen,de,demlen,testtext,1024,EX_PUBLICKEY_DE);

	szTmp	= getRtlByKey("order_id_com",(char*)testtext,szCharRtl);
	if(szTmp == NULL)
		return false;

	int	nSvrID	= atoi(szTmp);
	
	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"itool?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		szTmp	= getRtlByKey("user_id",(char*)testtext,szCharRtl);
		if(szTmp == NULL)
			return false;
		char	szAccUin[50];
		sprintf(szAccUin,"itool%d",atoi(szTmp));
		szTmp	= getRtlByKey("amount",(char*)testtext,szCharRtl);
		if(szTmp == NULL)
			return false;
		char	szRMB[64];
		strcpy(szRMB,szTmp);
		int nPayVal	= atoi(szRMB);
		if(nPayVal == 0)
			return false;

		char	szOrder[256];	//订单号
		szTmp	= getRtlByKey("order_id\"",(char*)testtext,szCharRtl);
		if(szTmp == NULL)
			return false;
		sprintf(szOrder,"itool%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAccUin,szOrder,nPayVal,TP_ITOOLPAY))
			return true;
		else
			return false;
	}

	return true;
}

bool HttpCenter::UCPayDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};

	char*	szTmp	= getRtlByKey("orderStatus",szParam,szCharRtl);
	if(!szTmp || stricmp(szTmp,"S") != 0)
		return false;
	char	szOS[32];
	strcpy(szOS,szTmp);
	char	szOrder[256];
	szTmp	= getRtlByKey("orderId",szParam,szCharRtl);
	if(!szTmp)
		return false;
	strcpy(szOrder,szTmp);
	char	szUCId[32];
	szTmp	= getRtlByKey("ucid",szParam,szCharRtl);
	if(!szTmp)
		return false;
	strcpy(szUCId,szTmp);
	char	szPayWay[32];
	szTmp	= getRtlByKey("payWay",szParam,szCharRtl);
	if(!szTmp)
		return false;
	strcpy(szPayWay,szTmp);
	char	szMoney[32];
	szTmp	= getRtlByKey("amount",szParam,szCharRtl);
	if(!szTmp)
		return false;
	strcpy(szMoney,szTmp);
	char	szClient[256];
	szTmp	= getRtlByKey("callbackInfo",szParam,szCharRtl);
	if(!szTmp)
		return false;
	strcpy(szClient,szTmp);
	char	szSn[256];
	szTmp	= getRtlByKey("sign",szParam,szCharRtl);
	if(!szTmp)
		return false;
	strcpy(szSn,szTmp);

	char	szSrcMD5[1024]	= {0};
	sprintf(szSrcMD5,"33271amount=%scallbackInfo=%sfailedDesc=gameId=538942orderId=%sorderStatus=%spayWay=%sserverId=3125ucid=%s040fdef4f48825ecc4d835d32d460b0d",
		szMoney,szClient,szOrder,szOS,szPayWay,szUCId);

	//sprintf(szSrcMD5,"109amount=100.00callbackInfo=custominfo=xxxxx#user=xxxxfailedDesc=gameId=123orderId=abcf1330orderStatus=SpayWay=1serverId=654ucid=123456202cb962234w4ers2aaa");
	Cmd5Capi	md5Capi;
	char* szMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szMd5,szSn) != 0)
		return false;

	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"ucpay?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单

		char* szTmpC	= strstr(szMoney,".");
		szTmpC[0]	= 0;
		int nPayVal	= atoi(szMoney);
		char	szRtlOrder[256];	//订单号
		sprintf(szRtlOrder,"uc%s(%s)",szOrder,szPayWay);
		char	szAcc[64];	//玩家账号
		sprintf(szAcc,"uc%s",szUCId);
		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRUC))
			return true;
		else
			return false;
	}

	return true;
}

bool HttpCenter::TPAdrLXDeal( char* szParam )
{
	//transdata={"exorderno":"1","transid":"2","appid":"3","waresid":31,"feetype":4,"money":5,
	//"count":6,"result":0,"transtype":0,"transtime":"2012-12-12 
	//	12:11:10","cpprivate":"7","paytype":1}&sign=d91cbc584316b9d99919921a9 
	char	szCharRtl[1024]	= {0};
	char	szRecvData[1024]	= {0};
	getParamByKey("transdata",szParam,szRecvData);		//分

	if(strlen(szRecvData) < 10)
		return false;

	char	szIsOk[32]	= {0};
	char*	szTmp	= getRtlByKey("result",szRecvData,szCharRtl);
	if(szTmp==NULL || atoi(szTmp) != 0)
		return false;

	char	szCOrder[255] = {0};
	szTmp	= getRtlByKey("exorderno",szRecvData,szCharRtl);
	if(szTmp)
		strcpy(szCOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getRtlByKey("money",szRecvData,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);

	char	szClient[255] = {0};
	szTmp	= getRtlByKey("cpprivate",szRecvData,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);


	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	int nPayVal	= atoi(szPrice)/100;
	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szCOrder,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrlx?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"adrlx%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRLX))
			return true;
		else
			return false;
	}


	return false;
}

bool HttpCenter::TPAdr360Deal( char* szDeParam )
{
	//order_id=1211090012345678901&app_key=1234567890abcdefghijklmnopqrstuv&product_id=p1&amount=10
	//	1&app_uid=123456789&app_ext1=XXX201211091985&user_id=987654321&sign_type=md5&gateway_flag=s
	//	uccess&sign=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx&sign_return=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};
	char	szPrice[32];
	strcpy(szPrice,getParamByKey("amount",szParam,szCharRtl));		//分
	char	szClient[255];
	strcpy(szClient,getParamByKey("app_ext1",szParam,szCharRtl));
	char	szCOrder[255];
	strcpy(szCOrder,getParamByKey("app_ext2",szParam,szCharRtl));
	char	szAppKey[64];
	strcpy(szAppKey,getParamByKey("app_key",szParam,szCharRtl));
	char	szAppID[32];
	strcpy(szAppID,getParamByKey("app_uid",szParam,szCharRtl));

	char	szGF[32];
	strcpy(szGF,getParamByKey("gateway_flag",szParam,szCharRtl));
	char	szSOrder[64];
	strcpy(szSOrder,getParamByKey("order_id",szParam,szCharRtl));
	char	szPID[32];
	strcpy(szPID,getParamByKey("product_id",szParam,szCharRtl));
	char	szST[64];
	strcpy(szST,getParamByKey("sign_type",szParam,szCharRtl));
	char	szSUser[64];
	strcpy(szSUser,getParamByKey("user_id",szParam,szCharRtl));

	//char	szCalcSign[1024]	= {0};
	//sprintf(szCalcSign,"%s#%s#%s#%s",szPrice,szClient,szAppKey,szAppID);
	//if(strlen(szGF))
	//	sprintf(szCalcSign+strlen(szCalcSign),"#%s",szGF);
	//sprintf(szCalcSign+strlen(szCalcSign),"#%s",szSOrder);
	//if(strlen(szPID))
	//	sprintf(szCalcSign+strlen(szCalcSign),"#%s",szPID);
	//if(strlen(szST))
	//	sprintf(szCalcSign+strlen(szCalcSign),"#%s",szST);
	//if(strlen(szSUser))
	//	sprintf(szCalcSign+strlen(szCalcSign),"#%s",szSUser);
	//sprintf(szCalcSign+strlen(szCalcSign),"#42690aef7647a783bce3498ce37d80f8");

	//if(0)	//验证签名
	//	return false;

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	int nPayVal	= atoi(szPrice)/100;
	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szCOrder,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adr360?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"adr360%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADR360))
			return true;
		else
			return false;
	}


	return false;
}

bool HttpCenter::TPOppoAdrDeal( char* szDeParam )
{
	char	szCharRtl[1024]	= {0};

	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char depuk[1024] =  "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmreYIkPwVovKR8rLHWlFVw7YDfm9uQOJKL89Smt6ypXGVdrAKKl0wNYc3/jecAoPi2ylChfa2iRu5gunJyNmpWZzlCNRIau55fxGW0XEu553IiprOZcaw5OuYGlf60ga8QT6qToP0/dpiL/ZbmNUO9kUhosIjEu22uFgR+5cYyQIDAQAB";

	char	szNotID[64];
	strcpy(szNotID,getParamByKey("notifyId",szParam,szCharRtl));
	char	szClient[64];
	strcpy(szClient,getParamByKey("partnerOrder",szParam,szCharRtl));
	char	szPName[64];
	strcpy(szPName,getParamByKey("productName",szParam,szCharRtl));
	char	szDesc[64];
	strcpy(szDesc,getParamByKey("productDesc",szParam,szCharRtl));
	char	szPrice[32];
	strcpy(szPrice,getParamByKey("price",szParam,szCharRtl));
	char	szCount[32];
	strcpy(szCount,getParamByKey("count",szParam,szCharRtl));
	char	szExP[256];
	strcpy(szExP,getParamByKey("attach",szParam,szCharRtl));

	char	szSAcc[64];
	strcpy(szSAcc,getParamByKey("userId",szParam,szCharRtl));

	//char	szNSign[1024]	= {0};
	//sprintf(szNSign,"notifyId=%s&partnerOrder=%s&productName=%s&productDesc=%s&price=%s&count=%s&attach=%s",
	//	szNotID,szClient,szPName,szDesc,szPrice,szCount,szExP);

	//获取目标服务器编号
	char*	szSplit	= strstr(szExP,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szExP,szSplit-szExP);
	int	nSvrID	= atoi(szSvr);

	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	int nPayVal	= atoi(szPrice)/100;
	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szClient,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"oppoadr?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"oppo%s",szClient);//szNotID);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADROPPO))
			return true;
		else
			return false;
	}

	return true;
}

bool HttpCenter::TPBDDKAdrDeal( char* szDeParam )
{
	char	szCharRtl[1024]	= {0};
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szIsOk[32]	= {0};
	char*	szTmp	= getParamByKey("result",szParam,szCharRtl);
	if(szTmp==NULL || atoi(szTmp) != 1)
		return false;

	char	szCOrder[255] = {0};
	szTmp	= getParamByKey("orderid",szParam,szCharRtl);
	if(szTmp)
		strcpy(szCOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("amount",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("aid",szParam,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);


	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	int nPayVal	= atoi(szPrice);
	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szCOrder,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"bddkadr?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"bddk%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRBDDK))
			return true;
		else
			return false;
	}


	return false;
}

bool HttpCenter::TPAdrAZDeal( char* szParam )
{
	char	szCharRtl[1024]	= {0};

	char*	szTmp	= getParamByKey("code",szParam,szCharRtl);
	if(atoi(szTmp) != 1)
		return false;

	char	szCOrder[255] = {0};
	szTmp	= getParamByKey("orderId",szParam,szCharRtl);
	strcpy(szCOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("payAmount",szParam,szCharRtl);
	strcpy(szPrice,szTmp);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("cpInfo",szParam,szCharRtl);
	strcpy(szClient,szTmp);


	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	
	char	szAcc[64] = {0};
	strcpy(szAcc,szSplit+1);
	int nPayVal	= atoi(szPrice)/100;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adraz?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"adraz%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRAZ))
			return true;
		else
			return false;
	}


	return false;
}

bool HttpCenter::TPAdrMiDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("orderStatus",szParam,szCharRtl);		//分

	if(strcmp(szTmp,"TRADE_SUCCESS") != 0)
		return false;


	char	szCOrder[255] = {0};
	szTmp	= getParamByKey("cpOrderId",szParam,szCharRtl);
	strcpy(szCOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("payFee",szParam,szCharRtl);
	strcpy(szPrice,szTmp);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("cpUserInfo",szParam,szCharRtl);
	strcpy(szClient,szTmp);

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	int nPayVal	= atoi(szPrice)/100;
	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szCOrder,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrmi?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"adrmi%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRMI))
			return true;
		else
			return false;
	}


	return false;
}

bool HttpCenter::TPAdrWDJDeal( char* szDeParam )
{
	char	szCharRtl[1024]	= {0};
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);

	char depuk[1024] =  "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCd95FnJFhPinpNiE/h4VA6bU1rzRa5+a2"
		"5BxsnFX8TzquWxqDCoe4xG6QKXMXuKvV57tTRpzRo2jeto40eHKClzEgjx9lTYVb2RFHHFW"
		"io/YGTfnqIPTVpi7d7uHY+0FZ0lYL5LlW4E2+CQMxFOPRwfqGzMjs1SDlH7lVrLEVy6QIDA"
		"QAB";

	std::string public_key_data;
	Base64ToBinary(depuk, strlen(depuk), &public_key_data);

	std::string sign_data;
	const char* sign = getParamByKey("sign",szParam,szCharRtl);
	Base64ToBinary(sign, strlen(sign), &sign_data);

	crypto::SignatureVerifier signer;
	signer.VerifyInit((const uint8*)sign_data.data(), sign_data.size(),
		(const uint8*)public_key_data.data(), public_key_data.size());

	char szContent[512]	= {0};
	strcpy(szContent,getParamByKey("content",szParam,szCharRtl));
	signer.VerifyUpdate((const uint8*)szContent, strlen(szContent));
	if(signer.VerifyFinal() == false)
		return false;

	
	char*	szClient	= getRtlByKey("out_trade_no",szContent,szCharRtl);
	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);

	char	szAcc[64] = {0};
	strcpy(szAcc,szSplit+1);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"adrwdj?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char* szTmp	= getRtlByKey("money",(char*)szContent,szCharRtl);

		if(szTmp == NULL || strlen(szTmp)==0)
			return false;

		int nPayVal	= atoi(szTmp)/100;

		char	szOrder[256];	//订单号
		szTmp	= getRtlByKey("orderId",szParam,szCharRtl);
		if(szTmp == NULL || strlen(szTmp)==0)
			return false;
		sprintf(szOrder,"adrwdj%s",szTmp);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_KYPAY))
			return true;
		else
			return false;
	}

	return false;
}

bool HttpCenter::TPAdrHWDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};
	char*	szTmp	= getParamByKey("result",szParam,szCharRtl);		//分
	if(strcmp(szTmp,"0") != 0)
		return false;

	char	szCOrder[255] = {0};
	szTmp	= getParamByKey("requestId",szParam,szCharRtl);
	strcpy(szCOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("amount",szParam,szCharRtl);
	strcpy(szPrice,szTmp);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("extReserved",szParam,szCharRtl);
	strcpy(szClient,szTmp);

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	int nPayVal	= atoi(szPrice);
	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szCOrder,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[1024];
		sprintf(szTPParam,"adrhw?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		szTmp	= getParamByKey("orderId",szParam,szCharRtl);
		if(szTmp == NULL || strlen(szTmp)==0)
			return false;
		sprintf(szOrder,"adrhw%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRHUAWEI))
			return true;
		else
			return false;
	}

	return false;
}

bool HttpCenter::TPAdrMZDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};

	char	szCOrder[255] = {0};
	char* szTmp	= getParamByKey("change_id",szParam,szCharRtl);
	if(szTmp)
		strcpy(szCOrder,szTmp);

	char	szUN[255] = {0};
	szTmp	= getParamByKey("username",szParam,szCharRtl);
	if(szTmp)
		strcpy(szUN,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("money",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);
	int nPayVal	= atoi(szPrice);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("object",szParam,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);

	char	szSMD5[256] = {0};

	Cmd5Capi	md5Capi;
	
	sprintf(szSMD5,"%s|%s|%s|7fd374b52d09ddb508f46fbaab5ebcca",szUN,szCOrder,szPrice);
	char* szCMd5	= md5Capi.Digest(szSMD5);
	szTmp	= getParamByKey("hash",szParam,szCharRtl);
	if(stricmp(szCMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrmz?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"adrmz%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRMZ))
			return true;
		else
			return false;
	}

	return false;
}

bool HttpCenter::TPAdrMZWDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};

	char	szOrder[255] = {0};
	char* szTmp	= getParamByKey("orderID",szParam,szCharRtl);
	if(szTmp)
		strcpy(szOrder,szTmp);
	char	szPname[64] = {0};
	szTmp	= getParamByKey("productName",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPname,szTmp);
	char	szPdesc[64] = {0};
	szTmp	= getParamByKey("productDesc",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPdesc,szTmp);
	char	szPID[32] = {0};
	szTmp	= getParamByKey("productID",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPID,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("money",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);
	int nPayVal	= atoi(szPrice);

	char	szUID[64]	= {0};
	szTmp	= getParamByKey("uid",szParam,szCharRtl);
	if(szTmp)
		strcpy(szUID,szTmp);
	
	char	szClient[255] = {0};
	szTmp	= getParamByKey("extern",szParam,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);

	char	szSMD5[512] = {0};
	Cmd5Capi	md5Capi;
	sprintf(szSMD5,"14e5fed2fa42054886f726739b83d418%s%s%s%s%s%s%s53ba04b5b1812",szOrder,szPname,szPdesc,szPID,szPrice,szUID,szClient);
	char* szCMd5	= md5Capi.Digest(szSMD5);
	szTmp	= getParamByKey("sign",szParam,szCharRtl);
	if(stricmp(szCMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrmzw?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szEOrder[256];	//订单号
		sprintf(szEOrder,"adrmzw%s",szOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szEOrder,nPayVal,TP_ADRMZW))
			return true;
		else
			return false;
	}
	return false;
}

bool HttpCenter::TPAdrPPSDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};

	char	szUID[32] = {0};
	char* szTmp	= getParamByKey("user_id",szParam,szCharRtl);
	if(szTmp)
		strcpy(szUID,szTmp);
	char	szPname[64] = {0};
	szTmp	= getParamByKey("role_id",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPname,szTmp);
	char	szOrder[64] = {0};
	szTmp	= getParamByKey("order_id",szParam,szCharRtl);
	if(szTmp)
		strcpy(szOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("money",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);
	int nPayVal	= atoi(szPrice);

	char	szTime[32] = {0};
	szTmp	= getParamByKey("time",szParam,szCharRtl);
	if(szTmp)
		strcpy(szTime,szTmp);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("userData",szParam,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);

	char	szSMD5[512] = {0};
	Cmd5Capi	md5Capi;
	sprintf(szSMD5,"%s%s%s%s%sYHZJ5645Gfg6757gfhjtu56hfr78Y573",szUID,szPname,szOrder,szPrice,szTime);
	char* szCMd5	= md5Capi.Digest(szSMD5);
	szTmp	= getParamByKey("sign",szParam,szCharRtl);
	if(stricmp(szCMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"_");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"_");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrpps?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szEOrder[256];	//订单号
		sprintf(szEOrder,"adrpps%s",szOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szEOrder,nPayVal,TP_ADRPPS))
			return true;
		else
			return false;
	}
	return false;
}

bool HttpCenter::TPAdrJFDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};

	char	szTime[64] = {0};
	char* szTmp	= getParamByKey("time",szParam,szCharRtl);
	if(szTmp)
		strcpy(szTime,szTmp);

	char	szCOrder[255]	= {0};
	char* szPS	= strstr(szParam,"<order_id>");
	if(szPS == NULL)
		return false;
	szPS	+= 10;
	char* szPE	= strstr(szPS,"</order_id>");
	if(szPE == NULL)
		return false;
	memcpy(szCOrder,szPS,szPE-szPS);

	char	szPrice[32] = {0};
	szPS	= strstr(szParam,"<cost>");
	if(szPS == NULL)
		return false;
	szPS	+= 6;
	szPE	= strstr(szPS,"</cost>");
	if(szPE == NULL)
		return false;
	memcpy(szPrice,szPS,szPE-szPS);
	int		nPayVal	= atoi(szPrice)/10;
	char	szKey[64]	= {0};
	szPS	= strstr(szParam,"<appkey>");
	if(szPS == NULL)
		return false;
	szPS	+= 8;
	szPE	= strstr(szPS,"</appkey>");
	if(szPE == NULL)
		return false;
	memcpy(szKey,szPS,szPE-szPS);

	if(stricmp(szKey,"1452216202") != 0)
		return false;

	//获取目标服务器编号
	char*	szSplit	= strstr(szCOrder,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szCOrder,szSplit-szCOrder);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	szMD5	= szMD5+1;

	char*	szCOrd	= strstr(szMD5,"#");
	char	szEMD5[64]	= {0};
	if(szCOrd == NULL)
		return false;
	memcpy(szEMD5,szMD5,szCOrd-szMD5);

	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl",szAcc);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szEMD5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrjf?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szEOrder[256];	//订单号
		DWORD dwTick	= CMTime::GetCurTickCount();
		sprintf(szEOrder,"adrjf%d",dwTick);

		if(g_pMainAccount->HttpPayRequest(szAcc,szEOrder,nPayVal,TP_ADRJF))
			return true;
		else
			return false;
	}
	return false;


	return false;
}

bool HttpCenter::TPAdrMMYDeal( char* szParam )
{
	//'orderID='.$_POST['orderID'].'&productPrice='.$_POST['productPrice'].'&productDesc='.$_POST['productDesc'].'&tradeState='.$_POST['tradeState']);
	char	szCharRtl[1024]	= {0};

	char*	szTmp	= getParamByKey("tradeState",szParam,szCharRtl);
	if(stricmp(szTmp,"success") != 0)
		return false;

	char	szCOrder[255] = {0};
	szTmp	= getParamByKey("orderID",szParam,szCharRtl);
	strcpy(szCOrder,szTmp);

	//char	szPrice[64]	= {0};
	szTmp	= getParamByKey("productPrice",szParam,szCharRtl);
	int nPayVal	= atoi(szTmp);

	char	szClient[255] = {0};
	szTmp	= getParamByKey("productDesc",szParam,szCharRtl);
	strcpy(szClient,szTmp);

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);
	char	szSMd5[64]	= {0};
	char*	szOrd	= strstr(szMD5+1,"#");
	if(szOrd == NULL)
		return false;
	memcpy(szSMd5,szMD5+1,szOrd-szMD5-1);
	szOrd	= szOrd+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szOrd,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szSMd5) != 0)
		return false;

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adrmmy?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szOrder[256];	//订单号
		sprintf(szOrder,"adrmmy%s",szOrd);

		if(g_pMainAccount->HttpPayRequest(szAcc,szOrder,nPayVal,TP_ADRMMY))
			return true;
		else
			return false;
	}


	return false;
}

bool HttpCenter::TPAdrYYHDeal( char* szDeParam )
{
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};


	char	szCOrder[64] = {0};
	char* szTmp	= getRtlByKey("exorderno",szParam,szCharRtl);
	if(szTmp)
		strcpy(szCOrder,szTmp);

	char	szPrice[64]	= {0};
	szTmp	= getRtlByKey("money",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);
	int nPayVal	= atoi(szPrice)/100;


	char	szClient[255] = {0};
	szTmp	= getRtlByKey("cpprivate",szParam,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"#");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"#");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);

	szMD5	= szMD5+1;
	char	szSrcMD5[64]	= {0};
	sprintf(szSrcMD5,"%szyl%s%d",szAcc,szCOrder,nPayVal);
	Cmd5Capi	md5Capi;
	char* szCMd5	= md5Capi.Digest(szSrcMD5);
	if(stricmp(szCMd5,szMD5) != 0)
		return false;
	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adryyh?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szEOrder[256];	//订单号
		sprintf(szEOrder,"adryyh%s",szCOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szEOrder,nPayVal,TP_ADRYYH))
			return true;
		else
			return false;
	}
	return false;

}

bool HttpCenter::TPAdrYLDeal(char* szDeParam){
	char szParam[1024]	= {0};
	UrlDecode(szDeParam,szParam);
	char	szCharRtl[1024]	= {0};

	char	szOrder[64] = {0};
	char* szTmp	= getParamByKey("orderId",szParam,szCharRtl);
	if(szTmp)
		strcpy(szOrder,szTmp);
	char	szUID[32]	= {0};
	szTmp	= getParamByKey("userName",szParam,szCharRtl);
	if(szTmp)
		strcpy(szUID,szTmp);
	char	szPrice[64]	= {0};
	szTmp	= getParamByKey("amount",szParam,szCharRtl);
	if(szTmp)
		strcpy(szPrice,szTmp);
	int nPayVal	= atoi(szPrice);
	char	szClient[255] = {0};
	szTmp	= getParamByKey("extra",szParam,szCharRtl);
	if(szTmp)
		strcpy(szClient,szTmp);

	char	szSMD5[512] = {0};
	Cmd5Capi	md5Capi;
	sprintf(szSMD5,"%s%s%s%se5c3c0a0f785653eea50184f8ed122bf",szOrder,szUID,szPrice,szClient);
	char* szCMd5	= md5Capi.Digest(szSMD5);
	strupr(szCMd5);
	szTmp	= getParamByKey("flag",szParam,szCharRtl);
	if(stricmp(szCMd5,szTmp) != 0)
		return false;

	//获取目标服务器编号
	char*	szSplit	= strstr(szClient,"_");
	char	szSvr[32] = {0};
	if(szSplit == NULL)
		return false;
	memcpy(szSvr,szClient,szSplit-szClient);
	int	nSvrID	= atoi(szSvr);
	char*	szMD5	= strstr(szSplit+1,"_");
	if(szMD5 == NULL)
		return false;
	char	szAcc[64] = {0};
	memcpy(szAcc,szSplit+1,szMD5-szSplit-1);

	if(g_pMainAccount->m_bIsMainAcc)
	{//转发给其他服务器
		_childAccSvrIP* pCAcc	= ServerCfg::GetInstance().GetAccSvr(nSvrID);
		if(!pCAcc)
			return false;
		char	szTPParam[2048];
		sprintf(szTPParam,"adryl?%s",szParam);
		if(requestbyIp(pCAcc->szHttpIP,pCAcc->nHttpPort,szTPParam) == 0)
			return true;
		else
			return false;
	}
	else// if(nSvrID == ServerCfg::GetInstance().nServerID)
	{//本服处理该订单
		char	szEOrder[256];	//订单号
		sprintf(szEOrder,"adryl%s",szOrder);

		if(g_pMainAccount->HttpPayRequest(szAcc,szEOrder,nPayVal,TP_ADRYL))
			return true;
		else
			return false;
	}
	return false;
}
