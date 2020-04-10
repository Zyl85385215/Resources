#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

class SearchGroupTypeConfig : public CSingleton<SearchGroupTypeConfig>
{
public:
	SearchGroupTypeConfig(void);
	~SearchGroupTypeConfig(void);

	bool		Load(const char * filename);

	char *		GetTxt(int nPos);

	xnList *	m_pList;
};