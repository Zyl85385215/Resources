#ifndef _JCH_Encrypt_Dll_H
#define _JCH_Encrypt_Dll_H
#include <wtypes.h>
extern   "C" 
{ 
#ifdef DLL_FILE
	_declspec(dllexport) void EncryptDword  (DWORD& dwVal);
	_declspec(dllexport) void DecryptDword  (DWORD& dwVal);
	_declspec(dllexport) void EncryptString  (char* szString);
	_declspec(dllexport) void DecryptString  (char* szString);
#else
	_declspec(dllimport) void EncryptDword  (DWORD& dwVal);
	_declspec(dllimport) void DecryptDword  (DWORD& dwVal);
	_declspec(dllimport) void EncryptString  (char* szString);
	_declspec(dllimport) void DecryptString  (char* szString);
#endif
}

typedef void (*EncryptDwordPtr)(DWORD& dwVal);
typedef void (*DecryptDwordPtr)(DWORD& dwVal);
typedef void (*EncryptStringPtr)(char* szString);
typedef void (*DecryptStringPtr)(char* szString);
#endif