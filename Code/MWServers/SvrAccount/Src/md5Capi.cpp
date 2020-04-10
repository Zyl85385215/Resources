
//////////////////////////////////////////////////////////////////////
// md5Capi.cpp: implementation of the Cmd5Capi class.
// Calcule MD5 Digest using the WIN Crypto API.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Windows.h>
#include "md5Capi.h"
#include <Wincrypt.h>
#include "stdio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cmd5Capi::Cmd5Capi()
{
	memset(csDigest,0,256);
}


Cmd5Capi::~Cmd5Capi()
{

}

char* Cmd5Capi::Digest(char* csBuffer)
{
	HCRYPTPROV hCryptProv; 
	HCRYPTHASH hHash; 
	BYTE bHash[0x7f]; 
	DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 
	DWORD cbContent= strlen(csBuffer); 
	BYTE* pbContent= (BYTE*)csBuffer; 


	if(CryptAcquireContext(&hCryptProv, 
		NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) 
	{

		if(CryptCreateHash(hCryptProv, 
			CALG_MD5,	// algorithm identifier definitions see: wincrypt.h
			0, 0, &hHash)) 
		{
			if(CryptHashData(hHash, pbContent, cbContent, 0))
			{

				if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) 
				{
					// Make a string version of the numeric digest value
					memset(csDigest,0,256);
					for (int i = 0; i<16; i++)
					{
						sprintf(csDigest,"%s%02x", csDigest,bHash[i]);
					}
				}
			}
		}
	}

	CryptDestroyHash(hHash); 
	CryptReleaseContext(hCryptProv, 0); 
	return csDigest; 
}
