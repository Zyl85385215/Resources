#pragma once


class BaseOPCenter
{
public:
	//bType ������,������ʯ���ѡ���������
	bool	RequestUpgrade(UserData* pUse, int	nObjID,BYTE bType = 0) {return false;}

private:
	//�����Ʒ����
	bool	_checkCostItems(UserData* pUse,BYTE bType)	{return true;}
	
	//�ɹ������ж�
	bool	_getSuccess(AmzDataBase* pObjData, BYTE bType)	{return true;}
	//�ɹ�ʧ�ܵĽ������
	bool	_runSuccessEffect(AmzDataBase* pObjData, BYTE bType)	{return true;}
	bool	_runFailedEffect(AmzDataBase* pObjData, BYTE bType)		{return true;}
	
	//ɾ����Ʒ
	bool	_removeCostItems(UserData* pUse) {return true;}
};