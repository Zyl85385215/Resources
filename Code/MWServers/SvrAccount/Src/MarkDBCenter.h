#pragma once


#include "DBCreate.h"


class MarkDBCenter
{
public:
	MarkDBCenter(CreateDB* pDB);
	~MarkDBCenter();

	void	CheckDayReset();	//启动或者并检测
	void	CreateRecord(int nTPID);
	void	AddNewRegAcc(BYTE byTP);		//新注册账号
	void	AddNewAtivAcc(BYTE byTP);		//新激活
	void	AddNewCreateAcc(BYTE byTP);		//新创建角色
	void	AddNewEffAcc(BYTE byTP);		//新有效账号
	void	AddLoginAcc(BYTE byTP,bool bFirst);		//登录账号数
	void	AddPayUserAcc(BYTE byTP);		//新增付费用户
	void	AddPayTotalAcc(BYTE byTP,int nCount);		//新增付费

	void	AddDayPayUserAcc(BYTE byTP);		//付费用户
	void	AddDayPayUserTime(BYTE byTP);		//付费次数
	void	AddDayPayTotalAcc(BYTE byTP,int nCount);		//付费
	void	AddRmbPay(BYTE byTP, int nVal);	//充值

	int		m_nDayMark;		//日期编号
	CreateDB*	m_pDB;
};