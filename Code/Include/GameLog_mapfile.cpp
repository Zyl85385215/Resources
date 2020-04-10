#include "GameLog_mapfile.h"
#include <stdio.h>

void nnList::Init()
{
	m_nDataCount = 0;
	m_dwDataSize = 0;
	m_nRead_Index = 0;
	m_nWrite_Index = 0;
	m_nEnd_Index = -1;
	m_dwLastItemSize = 0;

	m_dwBeginPos = sizeof(m_dwItems);
	m_dwEndPos = m_dwBeginPos + BUF_SIZE;	
	m_dwWritePos = m_dwBeginPos;
}

char* nnList::Pop(int maxpopcount, DWORD& popedSize)
{
	if (maxpopcount < 1)
	{
		return NULL;
	}
	if (m_nDataCount == 0)
	{
		return NULL;
	}

	int popcount = 0;
	popedSize = 0;
	DWORD dwReadStart = m_dwItems[m_nRead_Index];	
	if (maxpopcount > m_nDataCount)
	{
		maxpopcount = m_nDataCount;		
	}
	bool bReadAll = (maxpopcount == m_nDataCount);	//读到最后一个?
	int nReadEnd_Index = m_nRead_Index + maxpopcount;
	if (bReadAll)
	{		
		nReadEnd_Index --;				//往前面一个才能知道边界
	}
		
	if (nReadEnd_Index >= BUF_LIST_COUNT)
	{
		nReadEnd_Index -= BUF_LIST_COUNT;
	}
	DWORD dwReadEnd = m_dwItems[nReadEnd_Index];
	if (dwReadEnd < dwReadStart)			//发现内存换行的地方了
	{
		dwReadEnd = m_dwItems[m_nEnd_Index];           
		popcount = m_nEnd_Index - m_nRead_Index;
		if (popcount < 0)
		{
			popcount += BUF_LIST_COUNT;
		}			 
		popedSize += m_dwEndPos - dwReadStart - m_dwExtendSize;
		m_dwDataSize -= m_dwExtendSize;		
	}
	else
	{
		if (bReadAll)
		{		
			popedSize += m_dwLastItemSize;	//加上最后一个的大小
		}		
		popcount = maxpopcount;		
		popedSize += dwReadEnd - dwReadStart;
	}

	m_nRead_Index += popcount;
	if (m_nRead_Index >= BUF_LIST_COUNT)
	{
		m_nRead_Index -= BUF_LIST_COUNT;
	}		
	
	printf("poped count : %d\n", popcount);
	printf("poped size  : %d\n", popedSize);

	m_nDataCount -= popcount;
	m_dwDataSize -= popedSize;
	return (char*)(this)+dwReadStart;
}

bool nnList::Push(void* pItem, DWORD size)
{
	if (m_dwDataSize + size > BUF_SIZE)
	{		
		printf("nnList::Push failed m_dwDataSize: %d\n", m_dwDataSize);
        return false;
	}
	if (m_nDataCount >= BUF_LIST_COUNT)
	{
		printf("nnList::Push failed m_nDataCount: %d\n", m_nDataCount);
		return false;
	}
	if (m_nWrite_Index >= BUF_LIST_COUNT)		//检查index换行
	{		
		m_nWrite_Index = 0;
	}
	int extendsize = 0;
	if (m_dwWritePos + size > m_dwEndPos)			//检查内存换行
	{
		extendsize = m_dwEndPos - m_dwWritePos;
		m_dwExtendSize = extendsize;
		if(m_dwDataSize + size + extendsize > BUF_SIZE)
		{
			printf("nnList::Push failed 2 m_dwDataSize: %d\n", m_dwDataSize);
			return false;
		}		
		m_dwWritePos = m_dwBeginPos;
		m_nEnd_Index = m_nWrite_Index;
	}
	m_dwItems[m_nWrite_Index] = m_dwWritePos;
	memcpy((char*)(this)+m_dwWritePos, pItem, size);
	m_dwWritePos += size;
	m_nDataCount++;
	m_nWrite_Index++;
	m_dwDataSize += size + extendsize;
	m_dwLastItemSize = size;
	return true;
}
