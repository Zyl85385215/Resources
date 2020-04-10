#ifndef _GAME_LOG_MAP_FILE_
#define _GAME_LOG_MAP_FILE_

#define BUF_SIZE 5242880	//可写的缓存大小5M
#define ONE_FILE_SIZE 5242880	//一个日志文件大小5M
#define BUF_LIST_COUNT 50000	//5w
#define PROCESSID_SIZE 4	//writer进程ID
#define UPDATE_INTERVAL 500	//update间隔

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

	int		m_nEnd_Index;		//记录内存块最后的index，用来控制pop
	DWORD	m_dwExtendSize;		//内存块最后空闲内存大小
	DWORD	m_dwLastItemSize;
	
	DWORD	m_dwBeginPos;	//相对地址
	DWORD	m_dwEndPos;	
	DWORD	m_dwWritePos;
	DWORD	m_dwItems[BUF_LIST_COUNT];
public:
	void	Init();
	int		GetDataCount(){return m_nDataCount;}
    char*	Pop(int maxpopcount, DWORD& popedSize);	//弹出最多maxpopcount个，如果到达内存末尾则弹到末尾
    bool    Push(void* pItem, DWORD size);
};





#endif