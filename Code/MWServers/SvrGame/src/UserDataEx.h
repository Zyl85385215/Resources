#pragma once
class UserData;
class UserLuckData{
public:
	UserLuckData(UserData* pData);
	DWORD	m_dwVipLv;
	int		GetFreePower();		//免费恢复体力次数
	int		GetMoneyEquipLvUp();//装备银两强化次数
	bool	CanCreateLeague();	//创建军团
	int		GetPetCritic();		//灵兽强化暴击  装备强化暴击
	int		GetMagicExpPer();	//法宝淬炼
	int		GetFreePVPTimes();	//论剑
	int		GetFreeJJCTimes();	//竞技场
	int		GetFreeLeaderSkill();//免费重置队长技能次数
	int		GetLeagueContributeTimes();	//军团贡献次数
	int		GetMazeTimes();		//迷宫探索次数
	int		GetConvoyTimes();	//运镖次数
	int		GetMonsterTimes();	//挑战凶兽
	int		GetEndLessTimes();	//无限闯关

	BYTE	CalcLuckBetRtl(BYTE byCoin, WORD szVal[]);
	BYTE	CalcLuckBetRtlEx(BYTE bySide);
	void	GetLuckBetAward();
	int		m_nAwardCnt;
private:
	UserData*	m_pData;

	
	BYTE	m_byCoinType;
};
