#pragma once


class BaseOPCenter
{
public:
	//bType 特殊标号,如幸运石或可选择类的升法
	bool	RequestUpgrade(UserData* pUse, int	nObjID,BYTE bType = 0) {return false;}

private:
	//检查物品条件
	bool	_checkCostItems(UserData* pUse,BYTE bType)	{return true;}
	
	//成功条件判断
	bool	_getSuccess(AmzDataBase* pObjData, BYTE bType)	{return true;}
	//成功失败的结果保存
	bool	_runSuccessEffect(AmzDataBase* pObjData, BYTE bType)	{return true;}
	bool	_runFailedEffect(AmzDataBase* pObjData, BYTE bType)		{return true;}
	
	//删除物品
	bool	_removeCostItems(UserData* pUse) {return true;}
};