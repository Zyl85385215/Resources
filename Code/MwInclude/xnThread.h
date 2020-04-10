#pragma once

#include <windows.h>

// /使用:①创建挂起线程[设置线程名字,设置线程优先级]②开始线程③{发送消息}(循环)④停止线程⑤销毁线程
class xnThread 
{	
public:
	// /初始化
	xnThread(BOOLEAN (*process)(void * data, MSG *pMsg), void * data, char* szText = NULL, DWORD cpumask = 0); //创建挂起线程,回调函数 process
	virtual ~xnThread();																	//停止线程,干掉线程名字,关闭线程句柄
	void SetText(char* szText);                                                             //设置线程名字
	void SetPriority(int priority);                                                         //设置优先级
	// /操作
	void Start(void);                                                                       //如果线程挂起就让它运行
	void Stop(DWORD waitTime = 600*1000);												    //丢WM_QUIT到线程,等待waitTime时间,线程还在运行中就强制关闭
	void SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam,BOOLEAN isWait = FALSE);        //给活动的线程发送消息,默认不等待线程运行就发
	// /判断
	bool IsActive();																		//线程是否活着
	bool IsRunning();                                                                       //线程是否正在运行
	void PrintCpuTime();

private:	
	static DWORD WINAPI xnThread::_ThreadProc(void* pvData); //设置运行状态(true),通知线程开启,{等待线程消息,处理消息}(循环),通知线程退出,设置运行状态(false)

	HANDLE	m_hThread;								//线程句柄
	DWORD	m_dwThreadID;							//线程ID
	char*	m_szText;								//线程名字
	DWORD	m_dwUseTime;							//处理消息的累计时间
	void *	m_pParam;								//处理消息的引入参数
	BOOLEAN m_bRunning;								//运行状态,true运行,false挂起

	LONGLONG	m_llTotalMessageCnt;		//消息总数
	LONGLONG	m_llProcessedMessageCnt;	//已处理消息总数
	DWORD	m_dwLastTick;							//

	BOOLEAN	(*msgProcess)(void * data, MSG *pMsg);
};