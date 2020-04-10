// md5Capi.h: interface for the Cmd5Capi class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <wincrypt.h>	// Cryptographic API Prototypes and Definitions

class Cmd5Capi  
{
public:
	char* Digest(char* csBuffer);

	Cmd5Capi();
	virtual ~Cmd5Capi();
	char	csDigest[256];
};
