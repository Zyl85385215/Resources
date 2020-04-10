#ifndef JCH_ARRAY
#define JCH_ARRAY
#include <windows.h>
#include <search.h>

template <class T>
class JArray
{
public:
	int m_nBufferSize;
	int m_nBlockSize;
	int m_nCount;
	T**	m_ppItems;

	JArray(int nBlockSize=8)
	{
		if(nBlockSize <= 0) nBlockSize = 1;
		m_nBufferSize=nBlockSize;
		m_nBlockSize=nBlockSize;
		m_nCount=0;
		m_ppItems=new T*[nBlockSize];
		ZeroMemory(m_ppItems,m_nBufferSize*sizeof(T*));
	};
	~JArray()
	{
		ResetContent();
		delete [] m_ppItems;
	};
	void Clear()
	{
		for(int i=0;i<m_nCount;i++)
		{
			if(m_ppItems[i])
			{
				m_ppItems[i]=NULL;
			}
		}
		m_nCount=0;		
	};
	void ResetContent()
	{
		for(int i=0;i<m_nCount;i++)
		{
			if(m_ppItems[i])
			{
				delete m_ppItems[i];
				m_ppItems[i]=NULL;
			}
		}
		m_nCount=0;
	};
	void AddAtTail(T* pItem)
	{
		if(m_nCount+1>m_nBufferSize)
		{
			m_nBufferSize+=m_nBlockSize;
			T** ppTemp=m_ppItems;
			m_ppItems=new T*[m_nBufferSize];
			ZeroMemory(m_ppItems,m_nBufferSize*sizeof(T*));
			memcpy(m_ppItems,ppTemp,sizeof(T*)*m_nCount);
			delete[] ppTemp;
		}
		m_ppItems[m_nCount]=pItem;
		m_nCount++;
	};
	void AddAtTop(T* pItem)
	{
		if(m_nCount+1>m_nBufferSize)
		{
			m_nBufferSize+=m_nBlockSize;
			T** ppTemp=m_ppItems;
			m_ppItems=new T*[m_nBufferSize];
			ZeroMemory(m_ppItems,m_nBufferSize*sizeof(T*));
			memcpy(&m_ppItems[1],ppTemp,sizeof(T*)*m_nCount);
			delete[] ppTemp;
		}
		else
			memmove(&m_ppItems[1],m_ppItems,sizeof(T*)*m_nCount);
		m_ppItems[0]=pItem;
		m_nCount++;
	};
	int  InsertAt(int nIndex,T* pItem)
	{
		if(nIndex<0||nIndex>m_nCount)return -1;
		AddAtTail(pItem);
		if(nIndex==m_nCount-1)
		{			
			return nIndex;
		}
		int movecnt=(m_nCount-1)-nIndex;
		memmove(&m_ppItems[nIndex+1],&m_ppItems[nIndex],movecnt*sizeof(T*));
		m_ppItems[nIndex]=pItem;
		return nIndex;
	};
	void DelAt(int nIndex)
	{
		if(nIndex<0||nIndex>=m_nCount)return;
		delete m_ppItems[nIndex];
		m_ppItems[nIndex]=NULL;
		if(nIndex!=m_nCount-1)
		{
			int movecnt=m_nCount-(nIndex+1);
			memmove(&m_ppItems[nIndex],&m_ppItems[nIndex+1],movecnt*sizeof(T*));
			m_ppItems[m_nCount-1]=NULL;
		}
		m_nCount--;
	};
	BOOL DelItem(T* pItem)
	{
		for(int i=0;i<m_nCount;i++)
		{
			if(!memcmp(m_ppItems[i],pItem,sizeof(T)))
			{
				DelAt(i);
				return TRUE;
			}
		}
		return FALSE;
	};
	int IndexOf(T* pItem) const
	{
		for(int i=0;i<m_nCount;i++)
		{
			if(pItem == m_ppItems[i])
			{
				return i;
			}
		}
		return -1;
	};
	T* Extract(int nIndex)
	{
		if(nIndex<0||nIndex>=m_nCount)return NULL;
		T* pItem=m_ppItems[nIndex];
		m_ppItems[nIndex]=NULL;
		if(nIndex!=m_nCount-1)
		{
			int movecnt=m_nCount-(nIndex+1);
			memmove(&m_ppItems[nIndex],&m_ppItems[nIndex+1],movecnt*sizeof(T*));
			m_ppItems[m_nCount-1]=NULL;
		}
		m_nCount--;
		return pItem;
	};
	void MoveToTop(int nIndex)
	{
		if(nIndex<=0||nIndex>=m_nCount)return;
		T* pItem=m_ppItems[nIndex];
		memmove(&m_ppItems[1],m_ppItems,nIndex*sizeof(T*));
		m_ppItems[0]=pItem;
	};
	void MoveToTail(int nIndex)
	{
		if(nIndex<0||nIndex>=m_nCount-1)return;
		T* pItem=m_ppItems[nIndex];
		memmove(&m_ppItems[nIndex],&m_ppItems[nIndex+1],(m_nCount-nIndex-1)*sizeof(T*));
		m_ppItems[m_nCount-1]=pItem;
	};
	void Sort(int (*Compare)(const void * Item1, const void * Item2))
	{
		qsort((void*)m_ppItems, m_nCount, sizeof(void*), Compare);
	};

	T*	BSearch(void* key, int (*Compare)(const void * Item1, const void * Item2),int* pIndex = NULL) const
	{
		if (this == NULL)
			return NULL;
		T** returnPtr;
		returnPtr = (T**)bsearch(key, (void*)m_ppItems, (unsigned int)m_nCount, sizeof(T*), Compare);
		if(returnPtr == NULL)
			return NULL;
		if(pIndex != NULL)
		{
			*pIndex = returnPtr - m_ppItems;
		}
		return *returnPtr;
	}
};

#endif