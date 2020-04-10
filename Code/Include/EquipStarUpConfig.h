#pragma once

#include "singleton.h"
#include <map>
using namespace std;

typedef struct
{
	int nDecLevelFailed;//����ʧ�ܺ���
	int	nNeedGold;		//��ҪǮ
	int	nAtrribUpPercen;//���Լӳɰٷֱ�
	map<int,int> BasePercenttable;	//���ǻ����ɹ�����
	int nExHpVal;
	int	dwSpeedItem;
	int	dwSpeedItemEx;
}STAR_UP_CONFIG;

class CEquipStarUpConfig : public CSingleton<CEquipStarUpConfig>
{
public:
	CEquipStarUpConfig(void);
	~CEquipStarUpConfig(void);

	bool Load( const char * szFile );
	STAR_UP_CONFIG*	GetStarUpCfg(int nLevel);
	int		GetStarUpSpclItem(int dwItemID);
	int		GetStarUpPercent(STAR_UP_CONFIG* pStarUp,int nNum);
	int		nMustSucCount;
	int		nMustFailCount;
	map<int,STAR_UP_CONFIG> starUpCfg_;
};
