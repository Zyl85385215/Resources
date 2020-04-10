#include "SkillBase.h"
#include "xnExtStr.h"
#include <stdio.h>
#include "xnfile.H"
PLOADFUNC loadfromversion[E_VERSION_MAX-ORIVERSION]=
{
	LOAD_ORIVERSION,
	LOAD_VER_1,
	LOAD_VER_2,
	LOAD_VER_3,
	LOAD_VER_4,
	LOAD_VER_5,
	LOAD_VER_6,
	LOAD_VER_7,
	LOAD_VER_8,
	LOAD_VER_9,
	LOAD_VER_10,
	LOAD_VER_11,
	LOAD_VER_12,
	LOAD_VER_13,
	LOAD_VER_14,
	LOAD_VER_15,
};
DWORD SkillInfo::count=0;
DWORD SkillInfo::version=ORIVERSION;


SkillInfo::SkillInfo()
{
	szName = NULL;
	wID = 0;
	bLevel = 0;
	bType = S_PHYSICAL;
	bType2	= 0;
	wIcon	= 0;
	nMaxTalentPoint = 0;
	nEffectPerTalentPoint = 0;
	nCastTimePerTalent = 0;
	nCoolTimePerTalent = 0;
	nDistancePerTalent = 0;
	fHpPerTalent = 0;
	fMpPerTalent = 0;
	fParamPerTalent = 0;
	nActNeedSkill1 = 0;
	nActNeedSkill2 = 0;
	nActNeedTalent1 = 0;
	nActNeedTalent2 = 0;

	wReadyEffect = 0;
	wAtkEffect = 0;
	wFlyEffect = 0;
	wHurtEffect = 0;
	szReadySound = NULL;
	szAtkSound = NULL;
	szFlySound = NULL;
	szHurtSound = NULL;

	szDesc	 = 0;

	dwReadyTime = 0;
	szAction = NULL;
	dwDelay = 0;
	bDelayType = 0;

	dwKeep_Interval = 0;
	dwKeep_Num = 0;
	wDistance_Max,wDistance_Min = 0;
	bArea_KeyPoint = (E_AREAKEYPOINT)0;
	bArea_Type = (E_AREATYPE)0;
	bArea_param1,bArea_param2,bArea_param1PerTalent = 0;
	bAtkCnt = 0;

	SkillPower_Const = 0;
	SkillPower_AttriType = (E_SKILLPOWERPARAM)0;
	SkillPower_MulParam = 0;
	bFormulaIndex = 0;

	dwConjureItem = 0;
	dwConjureItemCount = 0;
	dwSummonCreatureID,dwSummonCount = 0;

	dwJumpIndex = 0;
	dwJumpX = 0;
	dwJumpY = 0;
	bScreenEffect = 0;

	dwLearnCost = 0;   //下级技能
	bNeedCharLvl = 0;
	wNeedSkill = 0;   //上级技能
	bCanLearnFromPlayer = 0;
	bNeedClass = 0;			//job
	dwUseInStateFlag = 0;
	wNeedStr = 0;
	wNeedCon = 0;
	wNeedIng = 0;
	wNeedWis = 0;
	wNeedDex = 0;;
	wNeedLuk = 0;   //初始技能
	bNeedWeapon = 0;
	bNeedMoveState = 0;
	wNeedMP = 0;
	wNeedHP = 0;; // if <1,is a percent value
	wNeedItemID,wNeedItemCount = 0;;
	wNeedLearnItemID = 0;;
	bNeedTargetLevel_Max = 0;
	bNeedTargetLevel_Min = 0;
	bTargetType,bTargetRelation = 0;
	wNeedExp = 0;           //exp for upgrading skill
	wNeedPoint = 0;			//point for upgrading skill
	bUseSkillMsg = 0;
	szSkillMsg = NULL;
	m_szTalentTxt = NULL;
	m_szSkillAttackEventLuaScript = NULL;

	memset(SelfGainBuff, 0, sizeof(SelfGainBuff));
	memset(SelfRemoveBuff,0,sizeof(SelfRemoveBuff));
	memset(SelfBuffRatio,0,sizeof(SelfBuffRatio));
	memset(SelfBuffRatioPerTalent,0,sizeof(SelfBuffRatioPerTalent));
	memset(SelfStatePerTalent,0,sizeof(SelfStatePerTalent));
	SelfRemoveFlag = 0;
	SelfChgACT = 0;
	memset(TargetGainBuff, 0, sizeof(TargetGainBuff));
	memset(TargetRemoveBuff, 0, sizeof(TargetRemoveBuff));
	memset(TargetBuffRatio,0,sizeof(TargetBuffRatio));
	memset(TargetBuffRatioPerTalent,0,sizeof(TargetBuffRatioPerTalent));
	memset(TargetStatePerTalent,0,sizeof(TargetStatePerTalent));
	TargetRemoveFlag = 0;
	TargetChgACT = 0;
	TargetHpChg = 0;
	TargetMpChg = 0;
	bSitFlag = 0;

	SCRIPT = NULL;
	m_pbScript = NULL;

	CheckScript = NULL;
	m_pbCheckScript = NULL;

	bIgnoreBlock = 0;
	HateValue = 0;
	bStrengthenType = 0;
	fStrengthenVal = 0;
	bLevelUpType = 0;

	GainBuffOnLearn = 0;
	GainBuffToSlaveOnLearn = 0;
	GainBuffToPlayerOnLearn = 0;

	UpExp = 0;
	UpExpLvl = 0;
	UpExpPower = 0;
	UpMoney = 0;
	UpMoneyLvl = 0;
	UpMoneyPower = 0;
	UpContribution = 0;
	UpContributionLvl = 0;
	UpContributionPower = 0;

	bSelfGainBuffRatio = 0;
	bSelfGainBuffRatioLvl = 0;
	bTarGainBuffRatio = 0;
	bTarGainBuffRatioLvl = 0;

	m_bLock = false;


	szName=strdup("NewSkill");
	szReadySound = strdup("");
	szAtkSound = strdup("");
	szHurtSound=strdup("");
	szDesc=strdup("");
	szFlySound=strdup("");
	SelfChgACT=strdup("");
	TargetChgACT=strdup("");
	SCRIPT=strdup("");
	CheckScript=strdup("");
	m_bLock = false;

	bInFury = false;
	nFuryStar = 0;
	nHateVal	= 0;
	m_byRideStatus = SKILL_RIDE_STATUS_WALK;
}

SkillInfo::~SkillInfo()
{
	if (szName)
		free(szName);
	if (szAction)
		free(szAction);
	if(szReadySound != NULL)
		free(szReadySound);
	if(szAtkSound != NULL)
		free(szAtkSound);
	if(szHurtSound != NULL)
		free(szHurtSound);
	if(szDesc != NULL)
		free(szDesc);
	if (szFlySound)
		free(szFlySound);
	if (szSkillMsg)
		free(szSkillMsg);
	if (m_szTalentTxt)
		free(m_szTalentTxt);
	if (SelfChgACT)
		free(SelfChgACT);
	if (TargetChgACT)
		free(TargetChgACT);
	if (SCRIPT)
		free(SCRIPT);
	if (CheckScript)
		free(CheckScript);
	if(m_szSkillAttackEventLuaScript) free(m_szSkillAttackEventLuaScript);
}
static ConvertLineBrk(char* szStr,bool blIn)
{
	char* szFlag = NULL;
	char* szReplace = NULL;
	if(blIn)
	{
		szFlag = "\\n";
		szReplace = "\r\n";
	}
	else
	{
		szFlag = "\r\n";
		szReplace = "\\n";
	}

	char* szBrk = strstr(szStr,szFlag);
	while(szBrk != NULL)
	{
		szBrk[0] = szReplace[0];
		szBrk[1] = szReplace[1];
		szStr = szBrk + 2;
		szBrk = strstr(szStr,szFlag);
	}
}
void SkillInfo::SetDesc(const char* szStr,bool blIn)
{
	if(szDesc != NULL)
	{
		free(szDesc);
		szDesc = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		szDesc = strdup(szStr);
	else
		szDesc = strdup("");
	ConvertLineBrk(szDesc,blIn);
}

void SkillInfo::SetName(const char* szStr)
{	
	if(szName != NULL)
	{
		free(szName);
		szName = NULL;
	}
	if(szStr &&strlen(szStr) > 0)
		szName = strdup(szStr);
	else
		szName = strdup("未命名");
}	

void SkillInfo::SetReadySound(const char* szStr)
{
	if(szReadySound != NULL)
	{
		free(szReadySound);
		szReadySound = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		szReadySound = strdup(szStr);
	else
		szReadySound = strdup("");
}

void SkillInfo::SetAtkSound(const char* szStr)
{
	if(szAtkSound != NULL)
	{
		free(szAtkSound);
		szAtkSound = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		szAtkSound = strdup(szStr);
	else
		szAtkSound = strdup("");
}

void SkillInfo::SetHurtSound(const char* szStr)
{
	if(szHurtSound != NULL)
	{
		free(szHurtSound);
		szHurtSound = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		szHurtSound = strdup(szStr);
	else
		szHurtSound = strdup("");
}

void SkillInfo::SetAction(const char* szStr)
{
	if(szAction != NULL)
	{
		free(szAction);
		szAction = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		szAction = strdup(szStr);
	else
		szAction = strdup("");
}

void SkillInfo::SetFlySound( const char *szStr ) 
{
	if (szFlySound)
	{
		free(szFlySound);
		szFlySound=NULL;
	}
	if(szStr && strlen(szStr))
	{
		szFlySound=strdup(szStr);
	}
	else
		szFlySound=strdup("");
}

int SkillInfo::Load( BYTE * Data ) 
{
	int readpos=0;
	//save basic info
	Loadstring(szName);
	LoadData(wID);
	LoadData(bLevel);
	LoadData(bType);
	LoadData(wIcon);
	
	LoadData(wReadyEffect);LoadData(wAtkEffect);LoadData(wFlyEffect);LoadData(wHurtEffect);
	Loadstring(szReadySound);Loadstring(szAtkSound);Loadstring(szFlySound);Loadstring(szHurtSound);
	
	Loadstring(szDesc);
	//save attribute info
	LoadData(dwReadyTime);
	Loadstring(szAction);
	LoadData(dwDelay);
	LoadData(bDelayType);
	
	LoadData(dwKeep_Interval);
	LoadData(dwKeep_Num);
	LoadData(wDistance_Max);
	LoadData(wDistance_Min);
	LoadData(bArea_KeyPoint);
	LoadData(bArea_Type);
	LoadDataByType(BYTE,bArea_param1);LoadDataByType(BYTE,bArea_param2);
	LoadData(bAtkCnt);
	
	LoadData(SkillPower_Const);
	LoadData(SkillPower_AttriType);
	LoadData(SkillPower_MulParam);
	LoadDataByType(BYTE,bFormulaIndex);
	
	LoadData(dwConjureItem);
	LoadData(dwConjureItemCount);
	LoadData(dwSummonCreatureID);LoadData(dwSummonCount);
	
	LoadData(dwJumpIndex);LoadData(dwJumpX);LoadData(dwJumpY);
	LoadData(bScreenEffect);
	//save learn and use
	LoadData(dwLearnCost);
	LoadData(bNeedCharLvl);
	LoadData(wNeedSkill);
	LoadData(bCanLearnFromPlayer);
	
	LoadData(bNeedClass);
	LoadData(dwUseInStateFlag);
	
	LoadData(wNeedStr);LoadData(wNeedCon);LoadData(wNeedIng);LoadData(wNeedWis);LoadData(wNeedDex);LoadData(wNeedLuk);
	LoadData(bNeedWeapon);LoadData(bNeedMoveState);
	LoadData(wNeedMP);LoadData(wNeedHP);
	LoadData(wNeedItemID);LoadData(wNeedItemCount);
	LoadData(wNeedLearnItemID);
	LoadData(bNeedTargetLevel_Max);LoadData(bNeedTargetLevel_Min);
	
	LoadData(bTargetType);
	LoadData(bTargetRelation);

	LoadData(wNeedExp); //add by dxx
	LoadData(wNeedPoint); //add by Luopei
	
	LoadData(bUseSkillMsg);		//add by Luopei
	Loadstring(szSkillMsg);
	//save skill effect
	LoadData(SelfGainBuff[0]);LoadData(SelfRemoveBuff[0]);LoadData(SelfRemoveFlag);Loadstring(SelfChgACT);
	LoadData(TargetGainBuff[0]);LoadData(TargetRemoveBuff[0]);LoadData(TargetRemoveFlag);
	LoadData(TargetHpChg);LoadData(TargetMpChg);
	Loadstring(SCRIPT);
	LoadData(bSitFlag);
	//the extra data doesn't load here,will load by version switch loader
	return readpos;
}

int SkillInfo::Load1( BYTE * Data ) 
{
	int readpos=0;
	//save basic info
	Loadstring(szName);
	LoadData(wID);
	LoadData(bLevel);
	LoadData(bType);
	LoadData(wIcon);
	
	LoadData(wReadyEffect);LoadData(wAtkEffect);LoadData(wFlyEffect);LoadData(wHurtEffect);
	Loadstring(szReadySound);Loadstring(szAtkSound);Loadstring(szFlySound);Loadstring(szHurtSound);
	
	Loadstring(szDesc);
	//save attribute info
	LoadData(dwReadyTime);
	Loadstring(szAction);
	LoadData(dwDelay);
	LoadData(bDelayType);
	
	LoadData(dwKeep_Interval);
	LoadData(dwKeep_Num);
	LoadData(wDistance_Max);
	LoadData(wDistance_Min);
	LoadData(bArea_KeyPoint);
	LoadData(bArea_Type);
	LoadDataByType(DWORD,bArea_param1);LoadDataByType(DWORD,bArea_param2);
	LoadData(bAtkCnt);
	
	LoadData(SkillPower_Const);
	LoadData(SkillPower_AttriType);
	LoadData(SkillPower_MulParam);
	LoadDataByType(DWORD,bFormulaIndex);
	
	LoadData(dwConjureItem);
	LoadData(dwConjureItemCount);
	LoadData(dwSummonCreatureID);LoadData(dwSummonCount);
	
	LoadData(dwJumpIndex);LoadData(dwJumpX);LoadData(dwJumpY);
	LoadData(bScreenEffect);
	//save learn and use
	LoadData(dwLearnCost);
	LoadData(bNeedCharLvl);
	LoadData(wNeedSkill);
	LoadData(bCanLearnFromPlayer);
	
	LoadData(bNeedClass);
	LoadData(dwUseInStateFlag);
	
	LoadData(wNeedStr);LoadData(wNeedCon);LoadData(wNeedIng);LoadData(wNeedWis);LoadData(wNeedDex);LoadData(wNeedLuk);
	LoadData(bNeedWeapon);LoadData(bNeedMoveState);
	LoadData(wNeedMP);LoadData(wNeedHP);
	LoadData(wNeedItemID);LoadData(wNeedItemCount);
	LoadData(wNeedLearnItemID);
	LoadData(bNeedTargetLevel_Max);LoadData(bNeedTargetLevel_Min);
	
	LoadData(bTargetType);
	LoadData(bTargetRelation);
	
	LoadData(wNeedExp);
	LoadData(wNeedPoint);

	LoadData(bUseSkillMsg);		//add by Luopei
	Loadstring(szSkillMsg);
	//save skill effect
	LoadData(SelfGainBuff[0]);
	LoadData(SelfGainBuff[1]);
	LoadData(SelfGainBuff[2]);

	LoadData(SelfRemoveBuff[0]);
	LoadData(SelfRemoveBuff[1]);
	LoadData(SelfRemoveBuff[2]);

	LoadData(SelfRemoveFlag);
	Loadstring(SelfChgACT);

	LoadData(TargetGainBuff[0]);
	LoadData(TargetGainBuff[1]);
	LoadData(TargetGainBuff[2]);

	LoadData(TargetRemoveBuff[0]);
	LoadData(TargetRemoveBuff[1]);
	LoadData(TargetRemoveBuff[2]);

	LoadData(TargetRemoveFlag);
	LoadData(TargetHpChg);LoadData(TargetMpChg);
	Loadstring(SCRIPT);
	LoadData(bSitFlag);
	//the extra data doesn't load here,will load by version switch loader
	return readpos;
}

void SkillInfo::SetSelfChgACT( const char* szStr ) 
{
	if(SelfChgACT != NULL)
	{
		free(SelfChgACT);
		SelfChgACT = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		SelfChgACT = strdup(szStr);
	else
		SelfChgACT = strdup("");
}

void SkillInfo::SetTargetChgACT( const char* szStr ) 
{
	if(TargetChgACT != NULL)
	{
		free(TargetChgACT);
		TargetChgACT = NULL;
	}
	if(szStr && strlen(szStr) > 0)
		TargetChgACT = strdup(szStr);
	else
		TargetChgACT = strdup("");
}

void SkillInfo::SetScript( const char * pScript) 
{
	if (this->SCRIPT)
	{
		free(SCRIPT);
		SCRIPT=NULL;
	}
	if (pScript && strlen(pScript)>0)
	{
		SCRIPT=strdup(pScript);
	}
	else
		SCRIPT=strdup("");
}

void SkillInfo::SetCheckScript( const char * pScript)
{
	if (this->CheckScript)
	{
		free(CheckScript);
		CheckScript=NULL;
	}
	if (pScript && strlen(pScript)>0)
	{
		CheckScript=strdup(pScript);
	}
	else
		CheckScript=strdup("");
}

SkillInfo & SkillInfo::operator=( SkillInfo & pinfo ) 
{
	//delete all string pointer first
		if (szAction)
			free(szAction);
		if(szReadySound != NULL)
			free(szReadySound);
		if(szAtkSound != NULL)
			free(szAtkSound);
		if(szHurtSound != NULL)
			free(szHurtSound);
		if(szName != NULL)
			free(szName);
		if(szDesc != NULL)
			free(szDesc);
		if (szFlySound)
			free(szFlySound);
		if (SelfChgACT)
			free(SelfChgACT);
		if (TargetChgACT)
			free(TargetChgACT);
		if (SCRIPT)
			free(SCRIPT);
		if (CheckScript)
			free(CheckScript);
		//copy all data from pinfo,include the string pointer,but,the string pointer must be reset
		memcpy(this,&pinfo,sizeof pinfo);
		//reset the string pointer,and copy value from pinfo
		this->szName=strdup(pinfo.szName);
		this->szReadySound=strdup(pinfo.szReadySound);
		this->szAtkSound=strdup(pinfo.szAtkSound);
		this->szHurtSound=strdup(pinfo.szHurtSound);
		this->szFlySound=strdup(pinfo.szFlySound);
		this->szAction=strdup(pinfo.szAction);
		this->SelfChgACT=strdup(pinfo.SelfChgACT);
		this->TargetChgACT=strdup(pinfo.TargetChgACT);
		this->SCRIPT=strdup(pinfo.SCRIPT);
		this->CheckScript=strdup(pinfo.CheckScript);
		this->szDesc=strdup(pinfo.szDesc);
		return * this;
}

int SkillInfo::LoadAllSkill( char * szname,LPSKILL*& pResult ) 
{
	int size=0;
	BYTE* pData=(BYTE *)xnFileRead(szname,&size);
	if (!pData) return 0;
	DWORD fileversion=0;
	memcpy(&fileversion,pData,4);
	int rt=0;
	if (fileversion>ORIVERSION && fileversion<E_VERSION_MAX)
		rt=loadfromversion[fileversion-ORIVERSION](pData,size,pResult);
	else
		rt=loadfromversion[0](pData,size,pResult);
	free(pData);
	return rt;
}

DWORD SkillInfo::GetReadyTime(int nTal)
{
	return (dwReadyTime + (nTal-1) * nCastTimePerTalent);
}

DWORD SkillInfo::GetCoolTime(int nTal)
{
	return (dwDelay + (nTal-1) * nCoolTimePerTalent);
}

int SkillInfo::GetMaxDistance(int nTal)
{
	return wDistance_Max + (nTal-1)*nDistancePerTalent;
}

float SkillInfo::GetNeedHp(int nTal)
{
	if (abs(fHpPerTalent)<1 && abs(wNeedHP)>1)
		return wNeedHP + abs(wNeedHP)*(nTal-1)*fHpPerTalent;
	else
		return wNeedHP + (nTal-1)*fHpPerTalent;
}

float SkillInfo::GetNeedMp(int nTal)
{
	if (abs(fMpPerTalent)<1 && abs(wNeedMP) > 1)
		return wNeedMP +abs(wNeedMP)*(nTal-1)*fMpPerTalent;
	else
		return wNeedMP + (nTal-1)*fMpPerTalent;
}