#pragma once

class IosThirdPlat{
public:
    IosThirdPlat();
    ~IosThirdPlat();
   
	const char*		TP_GetMacAddress();
	const char*			TP_GetDevID();
	void			TP_Update();
	int				TP_GetPlat();
	bool			TP_OPenCenter();
	//��¼���ֵ���Ϣ
	bool			TP_Login();
	int		nLoginStep;		//0û����,1ƽ̨׼����½ 2ƽ̨��¼�ɹ�, 3���ڽ���, 
	int		nLoginMID;		
	char*	szToken;		//

	//֧�����ֵ���Ϣ
	virtual	void	TP_RqPay(int nVal,int nSvrID, char* szAcc, char* szOrder);
	int		nPayStep;		//1֧�����
	char*	szOrder_No;		//������,���ڷ�������ʱ��֤��ѯ��
	int		nMoney;
};