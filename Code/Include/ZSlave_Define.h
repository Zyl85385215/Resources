#pragma once

class SlaveAttrib		// 属性
{
public:
	WORD	str;		// 力
	WORD	con;		// 体constitution
	WORD	ing;		// 智intelligence
	WORD	dex;		// 敏dexterity
	WORD	wis;		// 心wisdom	
};

class CZylSlaveData
{
public:
	CZylSlaveData(void)
	{
		memset(this,0,sizeof(CZylSlaveData));
	}

	CZylSlaveData(WORD wMId, LONGLONG llSlaveKey)
	{
		memset(this,0,sizeof(CZylSlaveData));
		m_llSlaveKey	= llSlaveKey;
		wMonID			= wMId;
	}
	
	LONGLONG		m_llSlaveKey;
	BYTE			bAtkType;
	WORD			wSkin;
	WORD			wMonID;
	WORD			wPoint;
	SlaveAttrib		sAttribCur;
	SlaveAttrib		sAttribBase;

	int				m_nHp;
	int				m_nMp;
	BYTE			bLevel;
	BYTE			bQuality;
	BYTE			bLife;
	BYTE			bPowLevel;
	DWORD			dwPowExp;
	DWORD			dwExp;
	char			szName[20];
	int				m_nSkill[9];		//后3个为天赋
};