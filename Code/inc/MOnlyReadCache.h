#pragma once
#include <string.h>
#include "JArray.h"
#include "mhSingleton.hpp"


// ------------------------------------------------------------------------------------------
// 任务执行系统 -------------------------------------
// =====================================华丽的 IPrototype <T>类 分割线=============
template <class T>
class IPrototype
{
public:
	virtual T*		clone()   = 0;
	virtual void	release() = 0;
};

// =====================================华丽的 IQuest <T>类 分割线=============
// 让 T 作为任务
class IQuest: public IPrototype<IQuest>
{
public:
	virtual bool	run()		= 0;
	virtual IQuest*	clone()		= 0;
	virtual void	release()	= 0;
};

// =====================================华丽的 CQuestRunner <T> 分割线============= 
// 任务环形队列
// 流程:
// 1. 主线程推入任务			push
// 2. 处理线程等待任务			wait
// 3. 处理线程处理任务			IQuest::run
// 4. 处理线程后移任务			next
// 5. 处理线程弹出已处理的任务  pop
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
		TerminateThread(m_hProcessThread,0);// 强关线程
		CloseHandle(m_hProcessThread);
		_reset();
		delete [] m_ppItems;
	};

	// 推入操作不能多线程操作，主线程推入
	bool					push(IQuest* pQuest)
	{
		// 清理已加载元素，单线程释放
		while( m_nBase != m_nLoadedTop )
			_pop();

		// 找到队列首部前一个位置 ---
		int nTopNext = m_nTop+1;
		nTopNext = nTopNext >= m_nTotalSize ? 0:nTopNext;

		// 后备缓冲的内容填充到队列中，直到无法填充位置
		while(nTopNext != m_nBase && m_xBakArray.m_nCount)
		{
			m_ppItems[m_nTop]	= m_xBakArray.m_ppItems[0];
			m_nTop				= nTopNext;

			m_xBakArray.Extract(0);	

			nTopNext			= m_nTop+1;
			nTopNext			= nTopNext >= m_nTotalSize ? 0:nTopNext;
		}

		// 自己推进队列
		if(nTopNext != m_nBase)
		{
			m_ppItems[m_nTop] = pQuest->clone();
			m_nTop			  = nTopNext;
			return true;
		}
		// 自己进不了队列只能进后备缓冲
		// ????假如没有人再主动来推一把的话，就会陷入永久在后备缓冲等待情况。
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
	int						m_nTotalSize;	// 队列大小
	int						m_nTop;			// 队列首部
	int						m_nBase;        // 队列尾部
	int						m_nLoadedTop;	// 加载项位置
	IQuest**				m_ppItems;		// 队列内存
	JArray< IQuest >		m_xBakArray;	// 后备缓冲
	void					_reset()
	{
		while(m_nBase != m_nTop)// 推出元素
		{
			int nBaseNext	= m_nBase+1;
			nBaseNext		= nBaseNext >= m_nTotalSize ? 0:nBaseNext;
			delete			m_ppItems[m_nBase];
			m_nBase			= nBaseNext;
		}
	};

	IQuest*					_wait()		// 任务处理线程调用
	{
		if( m_nLoadedTop == m_nTop)	// 加载到底了
			return NULL;

		return m_ppItems[m_nLoadedTop];
	}

	void					_next()		// 移动处理游标
	{
		int nLoadNext	= m_nLoadedTop+1;
		m_nLoadedTop	= nLoadNext >= m_nTotalSize ? 0:nLoadNext;
	}

	bool					_pop()			// 主线程弹出,弹出尾部元素
	{
		if( m_nBase == m_nLoadedTop)// 空了
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
// 任务执行系统 -------------------------------------
// ------------------------------------------------------------------------------------------

// =====================================华丽的 声明 分割线=============
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
	virtual bool	_onLoadStep1 ( char* szKey) = 0;					// 加载器第一步加载,返回是否成功
	virtual bool	_onLoadStep2 ( char* szKey, bool isMutiThread) = 0;	// 加载器第二步加载,返回是否成功
private:
	bool			m_bLoadFinish;
};

// ------------------------------------------------------------------------------------------
// 节点句柄系统 ------ 句柄有任务功能，clone()一个句柄作为任务扔给任务执行者
// =====================================华丽的 COnlyReadCacheNodeHandle <T> 实现分割线============= 
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
	bool							HasData()  { return m_pNode != NULL;}	// 兼容老的，以后别用咯。意义不对
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
	bool							_load(char* szKey, T* pObj,bool isMutiThread);								// spr主线程加载轮廓
	void							_attach(COnlyReadCacheNode<T>* pNode);
	void							_detach();

	COnlyReadCacheNode<T>*			m_pNode;
};

// =====================================华丽的 CCacheNode类 分割线=============
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

	char			 *				m_szKey;	// 键
	T				 *				m_pObj;		// 对象	
	long							m_nRef;		// 引用次数
	COnlyReadCache<T>*				m_pParent;

	// 不允许外部创建node和外部赋值和拷贝构造
	COnlyReadCacheNode (T* pObj, char*   szKey, COnlyReadCache<T>* pCache = NULL);

	template <class T>
		COnlyReadCacheNode (const COnlyReadCacheNode<T>& pNode)				{}

		template <class T>
			COnlyReadCacheNode<T>& operator= (const COnlyReadCacheNode<T>& pNode)	{}
};


// =====================================华丽的 COnlyReadCache类 分割线=============
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
	COnlyReadCacheNode<T>*		_add(T* pObj, char*  szKey);					// 添加进来肯定要用的,放到已用列表.
	COnlyReadCacheNode<T>*		_get(char* szKey);								// 看看缓冲区有没有这个东西.
	void						_2LoadWithMultiThread(COnlyReadCacheNodeHandle<T>*);

	void						_markUnuse( COnlyReadCacheNode<T>* pObj );		// 没人用了人,扔回使用列表
	void						_markUsed ( COnlyReadCacheNode<T>* pObj );		// 有人用了,拿回来使用

	JArray< COnlyReadCacheNode<T> >		m_xUsedArray;
	JArray< COnlyReadCacheNode<T> >		m_xUnUseArray;
	int									m_nCacheSize;
	CQuestRunner*						m_pLoadQuestRunner;
};

// =====================================华丽的 COnlyReadCaches类 分割线=============
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
// 节点句柄系统 ------ 
// ------------------------------------------------------------------------------------------

// =====================================华丽的 COnlyReadCacheNodeHandle <T> 实现分割线============= 
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
			return true;	// 同名
		_detach();			// 不同名就重新找
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
		// 添加到缓冲区
		pNode = COnlyReadCaches<T>::instance()->getCache(CACHEMASK(Hash(szKey)))->_add(pObj, szKey);
		// 挂钩Node---- PS. 这里必须先挂钩Node然后才能多线程加载，如果顺序调换可能会造成 加载完后(加载线程) 挂钩Node前(主线程) pNode被释放了。
		_attach(pNode);
		// 加载另外一部分
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

// =====================================华丽的 CCacheNode实现 分割线=============
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


// =====================================华丽的 COnlyReadCache 实现 分割线=============
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
	// 插入多线程加载
	m_pLoadQuestRunner->push( pNodeHandle );
}

template <class T>
COnlyReadCacheNode<T>* COnlyReadCache<T>::_get( char* szKey )
{
	// 无用列表中查询
	for(int i=0; i<m_xUnUseArray.m_nCount; i++)
	{
		if(strcmp(szKey, m_xUnUseArray.m_ppItems[i]->m_szKey) == 0)
			return m_xUnUseArray.m_ppItems[i];
	}
	// 已用列表中查询
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
			// 到达上限,把结点插入尾部
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
