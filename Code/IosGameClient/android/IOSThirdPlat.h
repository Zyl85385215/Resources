#pragma once

class IosThirdPlat{
public:
    IosThirdPlat();
    ~IosThirdPlat();
   
	const char*		TP_GetMacAddress();	//��ȡMAC��ַ,7.0����Ϊ�̶���ֵ
	int				TP_GetSystemVersion();	//IOS�汾��, float*10000
	const char*		TP_GetIDFA();		//��ȡIDFA,6.0����Ϊ��
	const char*			TP_GetDevID();
	void			TP_Update();
	int				TP_GetPlat();
	bool			TP_OPenCenter();
	int				TP_GetCenterType();
	//��¼���ֵ���Ϣ
	bool			TP_Login();
	int		nLoginStep;		//0û����,1ƽ̨׼����½ 2ƽ̨��¼�ɹ�, 3���ڽ���, 
	int		nLoginMID;		
	char*	szToken;		//

	//֧�����ֵ���Ϣ
	virtual	void	TP_RqPay(int nVal,int nSvrID, char* szAcc, char* szOrder);
	int		nPayStep;		//1֧�����  2IAP�ɹ�, 9ʧ��
	const char*	szOrder_No;		//������,���ڷ�������ʱ��֤��ѯ��
	int		nMoney;

	void		TP_OtherEvent(int nType,char* szUrl); 
	int			nEventMark;

	int		nLockInUpdate;
};

extern char*	g_szAPNToken;
void	gfun_TPResetVersionFile();		//������Դ�ļ�
char*	gfun_TPOpenVersionUrl(char*	szUrl,void* pData, int& nSize);
bool	gfun_TPDownZipFile(char*	szUrl,char*	szSaveName);
bool	gfun_TPUnZipFile(char*	szName);

//void	gfun_TPResetZipFileCnt();		//������Ϊ0,�ٿ�ʼ��ѹ,�ٻ�ȡ��ѹ�ļ�����ȷ������
//int		gfun_TPGetZipFileCnt();

