#pragma once

#include "singleton.h"
#include <map>
using namespace std;

typedef struct
{
	int nDecLevelFailed;//升星失败后降星
	int	nNeedGold;		//需要钱
	int	nAtrribUpPercen;//属性加成百分比
	map<int,int> BasePercenttable;	//升星基数成功概率
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
