#pragma once
#include <string.h>
#include "JArray.h"
#include "mhSingleton.hpp"


// ------------------------------------------------------------------------------------------
// ����ִ��ϵͳ -------------------------------------
// =====================================������ IPrototype <T>�� �ָ���=============
template <class T>
class IPrototype
{
public:
	virtual T*		clone()   = 0;
	virtual void	release() = 0;
};

// =====================================������ IQuest <T>�� �ָ���=============
// �� T ��Ϊ����
class IQuest: public IPrototype<IQuest>
{
public:
	virtual bool	run()		= 0;
	virtual IQuest*	clone()		= 0;
	virtual void	release()	= 0;
};

// =====================================������ CQuestRunner <T> �ָ���============= 
// �����ζ���
// ����:
// 1. ���߳���������			push
// 2. �����̵߳ȴ�����			wait
// 3. �����̴߳�������			IQuest::run
// 4. �����̺߳�������			next
// 5. �����̵߳����Ѵ��������  pop
class CQuestRunner
{
public:
	CQuestRunner( int nTotalSize=1024 )
	{	
		m_nTotalSize    = nTotalSize;
		m_nTop			= 0;
		m_nBase			= 0;
		m_nLoadedTop	= 0;
		m_ppItems		= new IQuest*[nTotalSize];
		memset(m_ppItems,0,nTotalSize*sizeof(IQuest*));
		m_hProcessThread= CreateThread(NULL, 0, _processProc,this,0,NULL);
	};
	~CQuestRunner()
	{	
		TerminateThread(m_hProcessThread,0);// ǿ���߳�
		CloseHandle(m_hProcessThread);
		_reset();
		delete [] m_ppItems;
	};

	// ����������ܶ��̲߳��������߳�����
	bool					push(IQuest* pQuest)
	{
		// �����Ѽ���Ԫ�أ����߳��ͷ�
		while( m_nBase != m_nLoadedTop )
			_pop();

		// �ҵ������ײ�ǰһ��λ�� ---
		int nTopNext = m_nTop+1;
		nTopNext = nTopNext >= m_nTotalSize ? 0:nTopNext;

		// �󱸻����������䵽�����У�ֱ���޷����λ��
		while(nTopNext != m_nBase && m_xBakArray.m_nCount)
		{
			m_ppItems[m_nTop]	= m_xBakArray.m_ppItems[0];
			m_nTop				= nTopNext;

			m_xBakArray.Extract(0);	

			nTopNext			= m_nTop+1;
			nTopNext			= nTopNext >= m_nTotalSize ? 0:nTopNext;
		}

		// �Լ��ƽ�����
		if(nTopNext != m_nBase)
		{
			m_ppItems[m_nTop] = pQuest->clone();
			m_nTop			  = nTopNext;
			return true;
		}
		// �Լ������˶���ֻ�ܽ��󱸻���
		// ????����û��������������һ�ѵĻ����ͻ����������ں󱸻���ȴ������
		m_xBakArray.AddAtTail(pQuest->clone());
		return false;
	}
	void					update()
	{
		while(true)
		{
			IQuest*	pQuest = NULL;
			while((pQuest = _wait()) == NULL)
				Sleep(1);

			pQuest->run();
			_next();
		}
	}

private:
	int						m_nTotalSize;	// ���д�С
	int						m_nTop;			// �����ײ�
	int						m_nBase;        // ����β��
	int						m_nLoadedTop;	// ������λ��
	IQuest**				m_ppItems;		// �����ڴ�
	JArray< IQuest >		m_xBakArray;	// �󱸻���
	void					_reset()
	{
		while(m_nBase != m_nTop)// �Ƴ�Ԫ��
		{
			int nBaseNext	= m_nBase+1;
			nBaseNext		= nBaseNext >= m_nTotalSize ? 0:nBaseNext;
			delete			m_ppItems[m_nBase];
			m_nBase			= nBaseNext;
		}
	};

	IQuest*					_wait()		// �������̵߳���
	{
		if( m_nLoadedTop == m_nTop)	// ���ص�����
			return NULL;

		return m_ppItems[m_nLoadedTop];
	}

	void					_next()		// �ƶ������α�
	{
		int nLoadNext	= m_nLoadedTop+1;
		m_nLoadedTop	= nLoadNext >= m_nTotalSize ? 0:nLoadNext;
	}

	bool					_pop()			// ���̵߳���,����β��Ԫ��
	{
		if( m_nBase == m_nLoadedTop)// ����
			return false;

		int nBaseNext	= m_nBase+1;
		nBaseNext		= nBaseNext >= m_nTotalSize ? 0:nBaseNext;

		m_ppItems[m_nBase]->release();
		m_nBase			= nBaseNext;
		return true;
	}

	HANDLE					m_hProcessThread;
	static DWORD WINAPI		_processProc(LPVOID pParam)
	{
		CQuestRunner* pRunner = (CQuestRunner*) pParam;
		while(true)
		{
			pRunner->update();
		}
		return 0;
	}
};
// ����ִ��ϵͳ -------------------------------------
// ------------------------------------------------------------------------------------------

// =====================================������ ���� �ָ���=============
template <class T> class COnlyReadCacheNodeHandle;
template <class T> class COnlyReadCacheNode;
template <class T> class COnlyReadCache;
template <class T> class COnlyReadCaches;

class I2StepLoader
{
public:
	I2StepLoader()
	{	
		m_bLoadFinish = false;
	}
	bool			isLoadFinish()
	{
		if(this == NULL)
			return false;
		return m_bLoadFinish;
	}

	bool			loadStep1( char* szKey )					
	{ 
		return _onLoadStep1(szKey);
	}

	bool			loadStep2( char* szKey, bool isMutiThread )	
	{ 
		bool bRlt		= _onLoadStep2(szKey, isMutiThread); 
		m_bLoadFinish	= true;
		return bRlt;
	}
protected:
	virtual bool	_onLoadStep1 ( char* szKey) = 0;					// ��������һ������,�����Ƿ�ɹ�
	virtual bool	_onLoadStep2 ( char* szKey, bool isMutiThread) = 0;	// �������ڶ�������,�����Ƿ�ɹ�
private:
	bool			m_bLoadFinish;
};

// ------------------------------------------------------------------------------------------
// �ڵ���ϵͳ ------ ����������ܣ�clone()һ�������Ϊ�����Ӹ�����ִ����
// =====================================������ COnlyReadCacheNodeHandle <T> ʵ�ַָ���============= 
template <class T>
class COnlyReadCacheNodeHandle: public IQuest
{
	friend COnlyReadCache<T>;
	friend CQuestRunner;
	friend I2StepLoader;
public:
	COnlyReadCacheNodeHandle(char* szKey, bool isMutiThread=true);
	COnlyReadCacheNodeHandle(const COnlyReadCacheNodeHandle<T>& pHandle);
	COnlyReadCacheNodeHandle();
	virtual ~COnlyReadCacheNodeHandle();

	COnlyReadCacheNodeHandle<T>&					operator= (const COnlyReadCacheNodeHandle<T>& pHandle);
	bool							Set(char* szKey = NULL, bool isMutiThread=true);
	bool							HasData()  { return m_pNode != NULL;}	// �����ϵģ��Ժ���ÿ������岻��
	bool							isNULL() { return m_pNode == NULL;}		// 
	T& operator*()  const			{ return *(m_pNode->m_pObj);}
	T* operator->() const			{ return  m_pNode?m_pNode->m_pObj:NULL; }

	unsigned int					Hash(const char * a_key);

	// IPrototype ------------
	virtual IQuest*					clone();
	virtual void                    release();
	// IQuest --------------------
	virtual bool					run();
protected:	
	// COnlyReadCacheNodeHandle------------
	bool							_load(char* szKey, T* pObj,bool isMutiThread);								// spr���̼߳�������
	void							_attach(COnlyReadCacheNode<T>* pNode);
	void							_detach();

	COnlyReadCacheNode<T>*			m_pNode;
};

// =====================================������ CCacheNode�� �ָ���=============
template <class T> 
class COnlyReadCacheNode
{
	friend CQuestRunner;
	friend COnlyReadCache<T>;
	friend COnlyReadCacheNodeHandle<T>;
public:
	~COnlyReadCacheNode();					

private:
	void							_addRef();
	void							_decRef();

	char			 *				m_szKey;	// ��
	T				 *				m_pObj;		// ����	
	long							m_nRef;		// ���ô���
	COnlyReadCache<T>*				m_pParent;

	// �������ⲿ����node���ⲿ��ֵ�Ϳ�������
	COnlyReadCacheNode (T* pObj, char*   szKey, COnlyReadCache<T>* pCache = NULL);

	template <class T>
		COnlyReadCacheNode (const COnlyReadCacheNode<T>& pNode)				{}

		template <class T>
			COnlyReadCacheNode<T>& operator= (const COnlyReadCacheNode<T>& pNode)	{}
};


// =====================================������ COnlyReadCache�� �ָ���=============
template <class T>
class COnlyReadCache
{
	friend COnlyReadCacheNode<T>;
	friend COnlyReadCacheNodeHandle<T>;
public:
	COnlyReadCache(CQuestRunner* pLoadQuestRunner,int nCacheCnt = 200);
	virtual ~COnlyReadCache();
	int							GetUsedCount();
	int							GetUnuseCount();
	void						SetCacheSize(int nCacheSize);

	void						clearCache();
protected:
	COnlyReadCacheNode<T>*		_add(T* pObj, char*  szKey);					// ��ӽ����϶�Ҫ�õ�,�ŵ������б�.
	COnlyReadCacheNode<T>*		_get(char* szKey);								// ������������û���������.
	void						_2LoadWithMultiThread(COnlyReadCacheNodeHandle<T>*);

	void						_markUnuse( COnlyReadCacheNode<T>* pObj );		// û��������,�ӻ�ʹ���б�
	void						_markUsed ( COnlyReadCacheNode<T>* pObj );		// ��������,�û���ʹ��

	JArray< COnlyReadCacheNode<T> >		m_xUsedArray;
	JArray< COnlyReadCacheNode<T> >		m_xUnUseArray;
	int									m_nCacheSize;
	CQuestRunner*						m_pLoadQuestRunner;
};

// =====================================������ COnlyReadCaches�� �ָ���=============
#define  CACHESIZE 8
#define  CACHEMASK(hashVal)  hashVal&0x7
template <class T>
class COnlyReadCaches
{
public:
	EAGER_SINGLETON(COnlyReadCaches)
		COnlyReadCaches();
	~COnlyReadCaches();

	COnlyReadCache<T>*  getCache(int nCacheType = 0);
	void				setCacheSize(int nCacheSize);
	void				clearCaches();

private:
	COnlyReadCache<T>*	m_pCaches[CACHESIZE];
	int					m_nCacheSize;
	CQuestRunner		m_xLoadQuestRunner;
};


template <class T>
COnlyReadCaches<T>::COnlyReadCaches()
{
	m_nCacheSize = 200;
	for (int i=0;i<CACHESIZE;i++)
	{
		m_pCaches[i]		= NULL;
	}
}
template <class T>
COnlyReadCaches<T>::~COnlyReadCaches()
{
	for(int i=0; i<CACHESIZE; i++ )
	{
		if(m_pCaches[i])
		{
			delete m_pCaches[i];
			m_pCaches[i] = NULL;
		}
	}
}
template <class T>
COnlyReadCache<T>* COnlyReadCaches<T>::getCache( int nCacheType /*= 0*/ )
{
	if(m_pCaches[nCacheType] == NULL && nCacheType < CACHESIZE)
	{
		m_pCaches[nCacheType]				= new COnlyReadCache<T>(&m_xLoadQuestRunner, m_nCacheSize);
	}
	return m_pCaches[nCacheType];
}

template <class T>
void COnlyReadCaches<T>::setCacheSize( int nCacheSize )
{
	m_nCacheSize = nCacheSize;
	for(int i=0; i<CACHESIZE; i++ )
	{
		if(m_pCaches[i])
		{
			m_pCaches[i]->SetCacheSize(m_nCacheSize);
		}
	}
}

template <class T>
void COnlyReadCaches<T>::clearCaches()
{	
	for(int i=0; i<CACHESIZE; i++ )
	{
		if(m_pCaches[i])
		{
			m_pCaches[i]->clearCache();
		}
	}
}
// �ڵ���ϵͳ ------ 
// ------------------------------------------------------------------------------------------

// =====================================������ COnlyReadCacheNodeHandle <T> ʵ�ַָ���============= 
template <class T>
COnlyReadCacheNodeHandle<T>::COnlyReadCacheNodeHandle()
{
	m_pNode  = NULL;
}


template <class T>
COnlyReadCacheNodeHandle<T>::COnlyReadCacheNodeHandle( const COnlyReadCacheNodeHandle<T>& pHandle )
{
	m_pNode			= pHandle.m_pNode;
	if(m_pNode)		_attach(m_pNode);
}

template <class T>
COnlyReadCacheNodeHandle<T>::COnlyReadCacheNodeHandle( char* szKey, bool isMutiThread/*=true*/ )
{
	m_pNode  = NULL;
	Set(szKey, isMutiThread);
}

template <class T>
COnlyReadCacheNodeHandle<T>& COnlyReadCacheNodeHandle<T>::operator=( const COnlyReadCacheNodeHandle<T>& pHandle )
{
	_detach();
	m_pNode			= pHandle.m_pNode;
	if(m_pNode)		_attach(m_pNode);
	return (*this);
}

template <class T>
COnlyReadCacheNodeHandle<T>::~COnlyReadCacheNodeHandle()
{
	_detach();
}

template <class T>
bool COnlyReadCacheNodeHandle<T>::Set( char* szKey, bool isMutiThread)
{
	if(szKey == NULL)
	{
		_detach();
		return false;
	}

	if(m_pNode)
	{
		if(strcmp(m_pNode->m_szKey, szKey) == 0)
			return true;	// ͬ��
		_detach();			// ��ͬ����������
	}

	COnlyReadCacheNode<T>* pNode = COnlyReadCaches<T>::instance()->getCache(CACHEMASK(Hash(szKey)))->_get(szKey);
	if(pNode == NULL)
	{
		T*		pObj			= new T;
		if(_load(szKey, pObj, isMutiThread) == false)
		{
			delete pObj;
			return false;
		}
		// ��ӵ�������
		pNode = COnlyReadCaches<T>::instance()->getCache(CACHEMASK(Hash(szKey)))->_add(pObj, szKey);
		// �ҹ�Node---- PS. ��������ȹҹ�NodeȻ����ܶ��̼߳��أ����˳��������ܻ���� �������(�����߳�) �ҹ�Nodeǰ(���߳�) pNode���ͷ��ˡ�
		_attach(pNode);
		// ��������һ����
		if(isMutiThread)
		{
			COnlyReadCaches<T>::instance()->getCache(CACHEMASK(Hash(szKey)))->_2LoadWithMultiThread(this);
		}
	}
	else
	{
		_attach(pNode);
	}
	return true;
}


template <class T>
unsigned int COnlyReadCacheNodeHandle<T>::Hash( const char * a_key )
{
	unsigned int key = 0;
	const char * cp = (const char *) a_key;

	while(*cp != '\0') 
	{
		key = (key + ((key << 5) + *cp));
		++cp;
	}
	return key;
}

template <class T>
void COnlyReadCacheNodeHandle<T>::_attach( COnlyReadCacheNode<T>* pNode )
{
	pNode->_addRef();
	m_pNode = pNode;
}

template <class T>
void COnlyReadCacheNodeHandle<T>::_detach()
{
	if(m_pNode)
	{
		m_pNode->_decRef();
		m_pNode = NULL;
	}
}


template <class T>
bool COnlyReadCacheNodeHandle<T>::_load( char* szKey, T* pObj,bool isMutiThread )
{
	if(isMutiThread)
	{
		return pObj->loadStep1(szKey);
	}
	else
	{
		if(pObj->loadStep1(szKey))
			return pObj->loadStep2(szKey, false);
		return false;
	}
}


template <class T>
IQuest* COnlyReadCacheNodeHandle<T>::clone()
{
	COnlyReadCacheNodeHandle<T> *pHandle = new COnlyReadCacheNodeHandle<T>;
	(*pHandle) = (*this);
	return pHandle;
}

template <class T>
void COnlyReadCacheNodeHandle<T>::release()
{
	delete this;
}

template <class T>
bool COnlyReadCacheNodeHandle<T>::run()
{
	return m_pNode->m_pObj->loadStep2(m_pNode->m_szKey,true);
}

// =====================================������ CCacheNodeʵ�� �ָ���=============
template <class T>
COnlyReadCacheNode<T>::COnlyReadCacheNode( T* pObj, char* szKey, COnlyReadCache<T>* pCache /*= NULL*/ )
{
	m_pObj	= pObj;
	m_szKey = strdup(szKey);
	m_nRef	= 0;
	m_pParent= pCache;
}

template <class T>
COnlyReadCacheNode<T>::~COnlyReadCacheNode()
{
	if(m_pObj) 
		delete m_pObj;

	if(m_szKey)
		free(m_szKey);
}

template <class T>
void COnlyReadCacheNode<T>::_addRef()
{
	if(m_nRef == 0)
		m_pParent->_markUsed(this);

	m_nRef ++;
}

template <class T>
void COnlyReadCacheNode<T>::_decRef()
{
	m_nRef--;
	if(m_nRef == 0)
		m_pParent->_markUnuse(this);
}


// =====================================������ COnlyReadCache ʵ�� �ָ���=============
template <class T>
COnlyReadCache<T>::COnlyReadCache( CQuestRunner* pLoadQuestRunner, int nCacheCnt /*= 200*/ )
:m_xUnUseArray(nCacheCnt?nCacheCnt:8)
{
	m_pLoadQuestRunner = pLoadQuestRunner;
	SetCacheSize(nCacheCnt?nCacheCnt:8);
}

template <class T>
COnlyReadCache<T>::~COnlyReadCache()
{
}


template <class T>
COnlyReadCacheNode<T>* COnlyReadCache<T>::_add( T* pObj, char* szKey)
{
	COnlyReadCacheNode<T>* pNode = new COnlyReadCacheNode<T>(pObj, szKey, this);
	m_xUsedArray.AddAtTail(pNode);

	return pNode;
}


template <class T>
void COnlyReadCache<T>::_2LoadWithMultiThread( COnlyReadCacheNodeHandle<T>* pNodeHandle)
{
	// ������̼߳���
	m_pLoadQuestRunner->push( pNodeHandle );
}

template <class T>
COnlyReadCacheNode<T>* COnlyReadCache<T>::_get( char* szKey )
{
	// �����б��в�ѯ
	for(int i=0; i<m_xUnUseArray.m_nCount; i++)
	{
		if(strcmp(szKey, m_xUnUseArray.m_ppItems[i]->m_szKey) == 0)
			return m_xUnUseArray.m_ppItems[i];
	}
	// �����б��в�ѯ
	for(int i=0; i<m_xUsedArray.m_nCount; i++)
	{
		if(strcmp(szKey, m_xUsedArray.m_ppItems[i]->m_szKey) == 0)
			return m_xUsedArray.m_ppItems[i];
	}
	return NULL;
}

template <class T>
void COnlyReadCache<T>::_markUsed(COnlyReadCacheNode<T>* pObj)
{
	int nIndex = m_xUnUseArray.IndexOf(pObj);
	if(nIndex != -1 &&  m_xUnUseArray.Extract(nIndex))
		m_xUsedArray.AddAtTail(pObj);
}

template <class T>
void COnlyReadCache<T>::_markUnuse(COnlyReadCacheNode<T>* pObj)
{
	int nIndex = m_xUsedArray.IndexOf(pObj);
	if(nIndex != -1 &&  m_xUsedArray.Extract(nIndex))
	{
		if(m_xUnUseArray.m_nCount == m_nCacheSize)
		{
			// ��������,�ѽ�����β��
			m_xUnUseArray.DelAt(0);
		}
		m_xUnUseArray.AddAtTail(pObj);
	}
}

template <class T>
int COnlyReadCache<T>::GetUnuseCount()
{
	return m_xUnUseArray.m_nCount;
}

template <class T>
int COnlyReadCache<T>::GetUsedCount()
{
	return m_xUsedArray.m_nCount;
}

template <class T>
void COnlyReadCache<T>::SetCacheSize( int nCacheSize)
{
	m_nCacheSize = nCacheSize?nCacheSize:8;
}

template <class T>
void COnlyReadCache<T>::clearCache()
{
	m_xUnUseArray.ResetContent();
}
