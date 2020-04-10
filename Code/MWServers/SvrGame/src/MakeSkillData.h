#ifndef MakeSkillData_h__
#define MakeSkillData_h__
#include "ItemConfig.h"
#include "ios_user_packet.h"
class UserData;
class MakeSkillTag;
class MakeSkillData{
public:
	MakeSkillData(UserData* pData);
	
	enum{
		LEARN_SKILL_EXIST,
		LEARN_SKILL_FULL,
		LEARN_SKILL_SUCCESS,
		LEARN_SKILL_LV,
		LEARN_SKILL_NOT_ENOUGH_MONEY,
	};
	int			LearnMakeSkill(MAKE_SKILL_TYPE byType);
	enum{
		GIVE_UP_SKILL_UNEXIST,
		GIVE_UP_SKILL_SUCCESS,
	};
	int			GiveUpMakeSkill(MAKE_SKILL_TYPE byType);
	bool		CheckMakeSkill(MAKE_SKILL_TYPE byType);
	enum{
		USE_MAKE_SKILL_UNEXIST,
		USE_MAKE_SKILL_NOT_ENOUGH_ITEM,
		USE_MAKE_SKILL_NOT_ENOUGH_BAG,
		USE_MAKE_SKILL_CFG_ERROR,
		USE_MAKE_SKILL_LV_ERROR,
		USE_MAKE_SKILL_Gold_ERROR,
		USE_MAKE_SKILL_SUCCESS,
		USE_MAKE_SKILL_NOT_ENOUGH_POWER,
	};
	int			UseMakeSkill(int nRule);

	MakeSkill	m_skillInfo;
	void		LoadMakeSkillInfo(BYTE* pbD, int nSize);
	void		SendMakeSkillInfo();

	void		MakeSkillResult(BYTE byResult);

	int			GetPowerMax();
	void		AddPower(int nPower);
private:
	UserData*	m_pParent;
	inline	void		_setMakeSkill(int nPos,BYTE byType);
	void		_updateDBSkill();
	MakeSkillTag* m_pTempTag;
};

#endif // MakeSkillData_h__