#ifndef _GAME_LOG_MAP_FILE_
#define _GAME_LOG_MAP_FILE_

#define BUF_SIZE 5242880	//��д�Ļ����С5M
#define ONE_FILE_SIZE 5242880	//һ����־�ļ���С5M
#define BUF_LIST_COUNT 50000	//5w
#define PROCESSID_SIZE 4	//writer����ID
#define UPDATE_INTERVAL 500	//update���

#include <windows.h>

enum eGameLogType
{
	GAME_LOG_DEBUG,
	GAME_LOG_PLAYER,
    GAME_LOG_CHAT,
	GAME_LOG_MAX,
};

static char* WPIDpObject = "WPIDpObject";
static char* GameLogMapObject[GAME_LOG_MAX] = 
{		
	"debug",		
	"player",
	"chat",
};

class nnList
{
	int		m_nDataCount;
	DWORD	m_dwDataSize;

	int		m_nRead_Index;
	int		m_nWrite_Index;

	int		m_nEnd_Index;		//��¼�ڴ������index����������pop
	DWORD	m_dwExtendSize;		//�ڴ���������ڴ��С
	DWORD	m_dwLastItemSize;
	
	DWORD	m_dwBeginPos;	//��Ե�ַ
	DWORD	m_dwEndPos;	
	DWORD	m_dwWritePos;
	DWORD	m_dwItems[BUF_LIST_COUNT];
public:
	void	Init();
	int		GetDataCount(){return m_nDataCount;}
    char*	Pop(int maxpopcount, DWORD& popedSize);	//�������maxpopcount������������ڴ�ĩβ�򵯵�ĩβ
    bool    Push(void* pItem, DWORD size);
};





#endif