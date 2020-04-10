#pragma  once

#include "singleton.h"

#define		EQUIT_COUNT		10
#define		ENCHASE_TYPE_COUNT	10

#define		ENCHASE_LVL_COUNT	9

class	EnchaseExCig:public CSingleton<EnchaseExCig>
{
public:
	EnchaseExCig();
	~EnchaseExCig();

	void	LoadFile(char * szFile);
	int		GetPara(int	Type,int Lvl);
	int		GetType(int	Slot,bool	special);


	int		EnchaseType[EQUIT_COUNT][ENCHASE_TYPE_COUNT];
	int		EnchaseLvlPara[ENCHASE_TYPE_COUNT][ENCHASE_LVL_COUNT];
};