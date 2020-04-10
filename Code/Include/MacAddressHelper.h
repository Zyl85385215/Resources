//#include <WinSock2.h>
#include <IPHlpApi.h>
#include <iostream>

#pragma comment(lib,"Iphlpapi.lib")

using namespace std;

void output(PIP_ADAPTER_INFO pIpAdapterInfo,char * szAddress);

void GetMacAddress(char * szAddress)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO;
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	}
	if (ERROR_SUCCESS==nRel)
	{
		output(pIpAdapterInfo,szAddress);
	}
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
}

void output(PIP_ADAPTER_INFO pIpAdapterInfo,char * szAddress)
{
	strcpy(szAddress,"",1);
	if (pIpAdapterInfo)
	{
		for (int i=0;i<pIpAdapterInfo->AddressLength;i++)
		{
			if (i==pIpAdapterInfo->AddressLength-1)
			{
				sprintf(szAddress+strlen(szAddress),"%02x\n",pIpAdapterInfo->Address[i]);
			}
			else
			{
				sprintf(szAddress+strlen(szAddress),"%02x-",pIpAdapterInfo->Address[i]);
			}
		}
	}
}