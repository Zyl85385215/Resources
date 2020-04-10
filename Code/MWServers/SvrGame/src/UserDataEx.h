#pragma once
class UserData;
class UserLuckData{
public:
	UserLuckData(UserData* pData);
	DWORD	m_dwVipLv;
	int		GetFreePower();		//��ѻָ���������
	int		GetMoneyEquipLvUp();//װ������ǿ������
	bool	CanCreateLeague();	//��������
	int		GetPetCritic();		//����ǿ������  װ��ǿ������
	int		GetMagicExpPer();	//��������
	int		GetFreePVPTimes();	//�۽�
	int		GetFreeJJCTimes();	//������
	int		GetFreeLeaderSkill();//������öӳ����ܴ���
	int		GetLeagueContributeTimes();	//���Ź��״���
	int		GetMazeTimes();		//�Թ�̽������
	int		GetConvoyTimes();	//���ڴ���
	int		GetMonsterTimes();	//��ս����
	int		GetEndLessTimes();	//���޴���

	BYTE	CalcLuckBetRtl(BYTE byCoin, WORD szVal[]);
	BYTE	CalcLuckBetRtlEx(BYTE bySide);
	void	GetLuckBetAward();
	int		m_nAwardCnt;
private:
	UserData*	m_pData;

	
	BYTE	m_byCoinType;
};
