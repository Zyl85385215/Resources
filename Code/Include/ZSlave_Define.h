#pragma once

class SlaveAttrib		// ����
{
public:
	WORD	str;		// ��
	WORD	con;		// ��constitution
	WORD	ing;		// ��intelligence
	WORD	dex;		// ��dexterity
	WORD	wis;		// ��wisdom	
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
	int				m_nSkill[9];		//��3��Ϊ�츳
};