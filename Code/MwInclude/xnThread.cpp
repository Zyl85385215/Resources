#include "stdafx.h"
#include "xnThread.h"
#include "debug_def.h"

// xnThread ==================================================================
DWORD WINAPI xnThread::_ThreadProc(void* pvData)
{
	xnThread *	pThread = (xnThread*)pvData;
	MSG			tMsg;

	PeekMessage(&tMsg, NULL, NULL, NULL, PM_NOREMOVE);
	pThread->m_bRunning = TRUE;

	while(GetMessage(&tMsg, NULL, NULL, NULL))
	{
		if (pThread->msgProcess != NULL)
		{
			BOOLEAN ret = pThread->msgProcess(pThread->m_pParam, &tMsg);
/*			if (ret)*/
			{
				pThread->m_llProcessedMessageCnt++;
			}
		}
	}

	pThread->m_bRunning = FALSE;	
	return 0;
}

xnThread::xnThread(BOOLEAN (*process)(void * data, MSG *pMsg), void * data, char* szText /*= NULL*/, DWORD cpumask)
{
	m_hThread		= CreateThread(NULL , 0, _ThreadProc, this, CREATE_SUSPENDED, &m_dwThreadID);
	m_szText		= strdup(szText);
// 	if (cpumask)
// 	{
// 		DWORD ret = SetThreadAffinityMask(m_hThread, cpumask);
// 		if (ret == 0)
// 		{
// 			ret = GetLastError();
// 			printf("SetThreadAffinityMask err:%d\n", ret);
// 		}
// 	}

	this->m_pParam	= data;
	msgProcess		= process;

	m_bRunning		= FALSE;
	m_dwUseTime		= 0;

	m_llTotalMessageCnt	= 0;
	m_llProcessedMessageCnt = 0;
	m_dwLastTick	= 0;
}

xnThread::~xnThread()
{
	Stop();
	CloseHandle(m_hThread);
	free(m_szText);
}

void xnThread::Start(void)
{
	if (!m_bRunning)
		ResumeThread(m_hThread);
	m_bRunning	= TRUE;
}

void xnThread::SetText(char* szText)
{
	free(m_szText);
	m_szText = strdup(szText);
}

void xnThread::Stop(DWORD waitTime)
{
	if (m_bRunning)
	{
		DWORD delay = GetTickCount() + waitTime;
		while (m_bRunning && !PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0))
		{
			Sleep(1000);
		}
		while ((m_bRunning) && (GetTickCount() < delay))
			Sleep(1000);
		if (m_bRunning)
			TerminateThread(m_hThread, 0);
		m_bRunning = FALSE;
	}
}

void xnThread::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam,BOOLEAN isWait)
{
	DWORD dwExitCode = 0;
	GetExitCodeThread(m_hThread,&dwExitCode);
	if(dwExitCode == STILL_ACTIVE)
	{
		if (m_bRunning)
		{
			if (isWait)
			{
				int trycnt = 0;
				while(!PostThreadMessage(m_dwThreadID, Msg, wParam, lParam) && m_bRunning)
				{
/*					Sleep(2);*/
					GetExitCodeThread(m_hThread,&dwExitCode);
					if(dwExitCode != STILL_ACTIVE)
					{
						break;
					}
					if (trycnt ++ > 50)
					{
						Sleep(1);
						trycnt = 0;
					}
				}
				m_llTotalMessageCnt++;
			}
			else
			{
				if (PostThreadMessage(m_dwThreadID, Msg, wParam, lParam) )
					m_llTotalMessageCnt++;
			}
		}

	}

}

void xnThread::SetPriority(int priority)
{
	if (this != NULL)
		SetThreadPriority(m_hThread, priority);
}

bool xnThread::IsActive()
{
	DWORD dwExitCode = 0;
	GetExitCodeThread(m_hThread,&dwExitCode);
	if(dwExitCode == STILL_ACTIVE)
	{
		return true;
	}
	return false;
}

bool xnThread::IsRunning()
{
	return m_bRunning;
}

DWORD g_dwTimeElapsed = 1;
void xnThread::PrintCpuTime()
{
	FILETIME createTime, exitTime, kernelTime, userTime;
	GetThreadTimes(m_hThread, &createTime,&exitTime,&kernelTime,&userTime);
	ULONGLONG temp ,temp2= 0;
	memcpy(&temp, &kernelTime, sizeof(ULONGLONG));
	memcpy(&temp2, &userTime, sizeof(ULONGLONG));
	double kernelPer = temp/10000/(g_dwTimeElapsed*1000.0);
	double userPer = temp2/10000/(g_dwTimeElapsed*1000.0);
	printf("%s kernelTime:%I64d(%.3lf) UserTime:%I64d(%.3lf)\n",m_szText, temp,kernelPer, temp2,userPer);
}