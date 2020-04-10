#include "EnchaseExCig.h"
#include "xnExtstr.h"
#include <stdio.h>

CreateSingleton(EnchaseExCig);

void EnchaseExCig::LoadFile( char * szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return ; 
	int index = 0;
	char szTmp[32];
	for (int i=0;i<EQUIT_COUNT;i++)
	{
		sprintf(szTmp,"Equit%d",i+1);
		if(pStr->SetSection(szTmp) != NULL)
		{
			for (int j=0;j<ENCHASE_TYPE_COUNT;j++)
			{
				sprintf(szTmp,"Enchase%d",j+1);
				EnchaseType[i][j] = atoi(pStr->ReadString(szTmp, "0"));
			}
		}
	}
	for (int i=0;i<ENCHASE_TYPE_COUNT;i++)
	{
		sprintf(szTmp,"Enchase%d",i+1);
		if(pStr->SetSection(szTmp) != NULL)
		{
			for (int j=0;j<ENCHASE_LVL_COUNT;j++)
			{
				sprintf(szTmp,"Para%d",j+1);
				EnchaseLvlPara[i][j] = atoi(pStr->ReadString(szTmp, "0"));
			}
		}
	}
	pStr->Free();
	return ;
}

EnchaseExCig::EnchaseExCig()
{
	memset(EnchaseType, 0, EQUIT_COUNT*ENCHASE_TYPE_COUNT*sizeof(int));
	memset(EnchaseLvlPara, 0, ENCHASE_TYPE_COUNT*ENCHASE_LVL_COUNT*sizeof(int));
}

EnchaseExCig::~EnchaseExCig()
{

}

int EnchaseExCig::GetPara( int Type,int Lvl )
{
	if (Type >=0 && Lvl >=0 && Type<ENCHASE_TYPE_COUNT && Lvl<ENCHASE_LVL_COUNT)
	{
		return EnchaseLvlPara[Type][Lvl];
	}
	else return 0;
}

int EnchaseExCig::GetType( int Slot ,bool	special)
{
	int i=1;int nType=0;
	if (Slot<EQUIT_COUNT && Slot>=0)
	{
		while(i!=0)
		{
			int nRand = rand() % 100;
			
			if (nRand<2)
			{
				nType=1;
			}
			else if (nRand>=2 && nRand<4)
			{
				nType=3;
			}
			else if (nRand>=4 && nRand<6)
			{
				nType=4;
			}
			else if (nRand>=6 && nRand<11)
			{
				nType=7;
			}
			else if (nRand>=11 && nRand<16)
			{
				nType=8;
			}
			else if (nRand>=16 && nRand<24)
			{
				nType=5;
			}
			else if (nRand>=24 && nRand<32)
			{
				nType=6;
			}
			else if (nRand>=32 && nRand<42)
			{
				nType=9;
			}
			else if (nRand>=42 && nRand<52)
			{
				nType=10;
			}
			else nType=2;

			if (special)
			{
				i=0;
			}
			if (EnchaseType[Slot][nType-1]>0)
			{
				i=0;
			}
		}
	}
	return nType;
}