#pragma once


//��ͼ������߲�ѯ�ж�ָ��

class MapAIObj;
class MapManager;
class MRActionCenter;
class MapAICenter
{
public:
	MapAICenter(MapManager* pParent);
	~MapAICenter();

	bool	GetEnemyAICommand(MapAIObj* pObj);
	bool	GetRobotAICommand(MapAIObj* pObj);
private:
	void	_tryAtk(MapAIObj* pObj);

	MapManager* m_pMap;
	MRActionCenter*	m_pActCenter;
};