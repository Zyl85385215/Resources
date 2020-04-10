#pragma once
#include "TitleConfig.h"


class UserData;
class TitleCenter
{
public:
	TitleCenter(UserData* pUser);
	~TitleCenter();
	void AddHonour(int nVal);
	void ResetObj();
	void KillEnemy(TitleObj* pObj);
	void BeKilled();

	TitleObj* m_pObj;
	TitleObj* m_pNextObj;
	UserData* m_pParent;
};