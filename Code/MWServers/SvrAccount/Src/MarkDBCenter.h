#pragma once


#include "DBCreate.h"


class MarkDBCenter
{
public:
	MarkDBCenter(CreateDB* pDB);
	~MarkDBCenter();

	void	CheckDayReset();	//�������߲����
	void	CreateRecord(int nTPID);
	void	AddNewRegAcc(BYTE byTP);		//��ע���˺�
	void	AddNewAtivAcc(BYTE byTP);		//�¼���
	void	AddNewCreateAcc(BYTE byTP);		//�´�����ɫ
	void	AddNewEffAcc(BYTE byTP);		//����Ч�˺�
	void	AddLoginAcc(BYTE byTP,bool bFirst);		//��¼�˺���
	void	AddPayUserAcc(BYTE byTP);		//���������û�
	void	AddPayTotalAcc(BYTE byTP,int nCount);		//��������

	void	AddDayPayUserAcc(BYTE byTP);		//�����û�
	void	AddDayPayUserTime(BYTE byTP);		//���Ѵ���
	void	AddDayPayTotalAcc(BYTE byTP,int nCount);		//����
	void	AddRmbPay(BYTE byTP, int nVal);	//��ֵ

	int		m_nDayMark;		//���ڱ��
	CreateDB*	m_pDB;
};