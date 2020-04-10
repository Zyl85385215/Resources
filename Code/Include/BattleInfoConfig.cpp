#include "BattleInfoConfig.h"
#include "xnExtstr.h"

CreateSingleton(CBattleInfoConfig);

CBattleInfoConfig::CBattleInfoConfig( void )
{
	for (int i=0;i<B_I_G_MAX;i++)
	{
		for (int j=0;j<B_I_O_MAX;j++)
		{
			szSprName[i][j] = NULL;
		}
	}
}

CBattleInfoConfig::~CBattleInfoConfig( void )
{
	for (int i=0;i<B_I_G_MAX;i++)
	{
		for (int j=0;j<B_I_O_MAX;j++)
		{
			if(szSprName[i][j])
				free(szSprName[i][j]);
		}
	}
}

bool CBattleInfoConfig::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	char szTempTarget[25];
	char szTempSelf[25];

	pIni->SetSection("Normal");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_NORMAL][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_NORMAL][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_NORMAL][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_NORMAL][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_NORMAL][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_NORMAL][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_NORMAL][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_NORMAL][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("Critical");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_CRITICAL][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_CRITICAL][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_CRITICAL][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_CRITICAL][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_CRITICAL][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_CRITICAL][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_CRITICAL][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_CRITICAL][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("Whack");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_WHACK][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_WHACK][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_WHACK][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_WHACK][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_WHACK][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_WHACK][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_WHACK][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_WHACK][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("Dodge");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_DODGE][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_DODGE][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_DODGE][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_DODGE][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_DODGE][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_DODGE][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_DODGE][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_DODGE][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("Cure");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_CURE][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_CURE][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_CURE][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_CURE][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_CURE][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_CURE][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_CURE][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_CURE][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("Parry");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_PARRY][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_PARRY][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_PARRY][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_PARRY][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_PARRY][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_PARRY][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_PARRY][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_PARRY][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("RegMp");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_REGMP][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_REGMP][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_REGMP][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_REGMP][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_REGMP][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_REGMP][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_REGMP][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_REGMP][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));

	pIni->SetSection("Money");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_MONEY][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_MONEY][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_MONEY][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_MONEY][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_MONEY][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_MONEY][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_MONEY][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_MONEY][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));
	pIni->SetSection("anqi");
	for (int i=0;i<10;i++)
	{
		sprintf(szTempSelf,"selfnum%d",i);
		sprintf(szTempTarget,"targetnum%d",i);
		szSprName[B_I_G_BOMB][B_I_O_SELFNUM0+i] = strdup(pIni->ReadString(szTempSelf));
		szSprName[B_I_G_BOMB][B_I_O_TARGETNUM0+i] = strdup(pIni->ReadString(szTempTarget));
	}
	szSprName[B_I_G_BOMB][B_I_O_SELFINFO] = strdup(pIni->ReadString("selfinfo"));
	szSprName[B_I_G_BOMB][B_I_O_TARGETINFO] = strdup(pIni->ReadString("targetinfo"));
	szSprName[B_I_G_BOMB][B_I_O_SELFPLUSSYMBOL] = strdup(pIni->ReadString("selfplussymbol"));
	szSprName[B_I_G_BOMB][B_I_O_SELFMINUSSYMBOL] = strdup(pIni->ReadString("selfminussymbol"));
	szSprName[B_I_G_BOMB][B_I_O_TARGETPLUSSYMBOL] = strdup(pIni->ReadString("targetplussymbol"));
	szSprName[B_I_G_BOMB][B_I_O_TARGETMINUSSYMBOL] = strdup(pIni->ReadString("targetminussymbol"));
	pIni->Free();
	return false;
}

char * CBattleInfoConfig::GetSprName( int b_i_g,int b_i_o )
{
	return szSprName[b_i_g][b_i_o];
}