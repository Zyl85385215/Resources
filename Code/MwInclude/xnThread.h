#pragma once

#include <windows.h>

// /ʹ��:�ٴ��������߳�[�����߳�����,�����߳����ȼ�]�ڿ�ʼ�̢߳�{������Ϣ}(ѭ��)��ֹͣ�̢߳������߳�
class xnThread 
{	
public:
	// /��ʼ��
	xnThread(BOOLEAN (*process)(void * data, MSG *pMsg), void * data, char* szText = NULL, DWORD cpumask = 0); //���������߳�,�ص����� process
	virtual ~xnThread();																	//ֹͣ�߳�,�ɵ��߳�����,�ر��߳̾��
	void SetText(char* szText);                                                             //�����߳�����
	void SetPriority(int priority);                                                         //�������ȼ�
	// /����
	void Start(void);                                                                       //����̹߳������������
	void Stop(DWORD waitTime = 600*1000);												    //��WM_QUIT���߳�,�ȴ�waitTimeʱ��,�̻߳��������о�ǿ�ƹر�
	void SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam,BOOLEAN isWait = FALSE);        //������̷߳�����Ϣ,Ĭ�ϲ��ȴ��߳����оͷ�
	// /�ж�
	bool IsActive();																		//�߳��Ƿ����
	bool IsRunning();                                                                       //�߳��Ƿ���������
	void PrintCpuTime();

private:	
	static DWORD WINAPI xnThread::_ThreadProc(void* pvData); //��������״̬(true),֪ͨ�߳̿���,{�ȴ��߳���Ϣ,������Ϣ}(ѭ��),֪ͨ�߳��˳�,��������״̬(false)

	HANDLE	m_hThread;								//�߳̾��
	DWORD	m_dwThreadID;							//�߳�ID
	char*	m_szText;								//�߳�����
	DWORD	m_dwUseTime;							//������Ϣ���ۼ�ʱ��
	void *	m_pParam;								//������Ϣ���������
	BOOLEAN m_bRunning;								//����״̬,true����,false����

	LONGLONG	m_llTotalMessageCnt;		//��Ϣ����
	LONGLONG	m_llProcessedMessageCnt;	//�Ѵ�����Ϣ����
	DWORD	m_dwLastTick;							//

	BOOLEAN	(*msgProcess)(void * data, MSG *pMsg);
};