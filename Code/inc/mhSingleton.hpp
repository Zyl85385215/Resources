#ifndef MSINGLETON_H
#define MSINGLETON_H

#include <assert.h>

//! ���͵��� ======== ��Ҫ���ʱ����ã���ʼ����
#define EAGER_SINGLETON(singleton) static singleton *instance(){ static singleton  self; return &self; }

//! ���͵��� ======== 
#define LAZY_SINGLETON_IN_DECL_CLASS(singleton)\
public:\
	static singleton *init()	{ if(NULL == s_pInstance) s_pInstance = new singleton;return s_pInstance; }\
	static void		 *deinit(); { if(s_pInstance) { delete s_pInstance; s_pInstance = NULL; } }\
	static singleton *instance(){ return s_pInstance; }\
private:\
	static singleton *s_pInstance;\

#define LAZY_SINGLETON_TO_DECL_NULL(singleton) singleton *singleton::s_pInstance = NULL;

//! ģ�嵥�� ======== 
// ==================Ascent�е�ģ�嵥����===================
/* ���˸о�ȱ��:
 * ȱ�㣺
 * ���캯����Ϊ˽�к�������¶����࣬��ι�������assert( s_pSingleton == 0 );
 * �ŵ㣺
 *    1.�̳м��ٴ����� 
 *    2.���캯����Ϊ˽�е��ŵ㣺��ʼ�����Զ�����
 *         �ɼ���(����ȫ�ֱ���createFileSingleton�����ù���������)
 *         �ɻ���(��ʼ������initialiseSingleton�����new�����delete����
 *                �ص�,ֻ�ܹ���һ������,  ����ڶ���new�ͳ���assert( s_pSingleton == 0 ))
*/

// ���ͺ��Ҫnew �� delete����
#define MInitialiseSingleton( type ) template <> type * CMSingleton < type > :: s_pSingleton = 0
// ���ͺ���ùܸö���ĳ�ʼ�����ͷ�
#define MCreateSingleton( type ) MInitialiseSingleton( type ); type the##type

#ifndef NULL
#define NULL 0
#endif

template < class type > 
class   CMSingleton {
public:
	CMSingleton( ) {
		// �쳣��׽��s_pSingleton��Ϊ��
		assert( s_pSingleton == NULL );
		s_pSingleton = static_cast<type *>(this);
	}
	virtual ~CMSingleton( ) { s_pSingleton = NULL;	}

	static type & getInstance()		
	{ 
		// �쳣��׽��s_pSingletonΪ��
		assert( s_pSingleton ); 
		return *s_pSingleton; 
	}
	static type * getInstancePtr( )	{ return s_pSingleton; }

protected:
	static type * s_pSingleton;
};

#endif