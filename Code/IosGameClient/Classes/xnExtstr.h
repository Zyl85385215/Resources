#pragma once

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <string>
#include <vector>
#include <sstream>

struct xnExtstr {
	char * sectionName;
	int	sectionIndex;
	int keyCount;
	int	sectionCount;
	static xnExtstr * Import(const char * fileName);
	static xnExtstr * ImportFromMemory(void * buffer, int bufSize);
	void	Free(void);
	bool	SetSection(char * section);
	bool	SetSection(int sectionIndex);
	char *	ReadString(int index, char * defaultResult = 0);
	char *	ReadString(char * key, char * defaultResult = 0);
	char *	GetKey(int index, char * defaultResult);
};

void StringSplit(std::string s,char* szC,std::vector<std::string>& vec);		//·Ö½â×Ö·û´®
void StringReplace(std::string& s,char C1,char C2);		//Ìæ»»×Ö·û
void FixDescString(char* pMsg);