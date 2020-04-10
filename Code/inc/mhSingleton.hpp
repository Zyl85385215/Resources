#ifndef MSINGLETON_H
#define MSINGLETON_H

#include <assert.h>

//! 急型单件 ======== 需要最初时候调用，初始化的
#define EAGER_SINGLETON(singleton) static singleton *instance(){ static singleton  self; return &self; }

//! 缓型单件 ======== 
#define LAZY_SINGLETON_IN_DECL_CLASS(singleton)\
public:\
	static singleton *init()	{ if(NULL == s_pInstance) s_pInstance = new singleton;return s_pInstance; }\
	static void		 *deinit(); { if(s_pInstance) { delete s_pInstance; s_pInstance = NULL; } }\
	static singleton *instance(){ return s_pInstance; }\
private:\
	static singleton *s_pInstance;\

#define LAZY_SINGLETON_TO_DECL_NULL(singleton) singleton *singleton::s_pInstance = NULL;

//! 模板单件 ======== 
// ==================Ascent中的模板单件法===================
/* 个人感觉缺点:
 * 缺点：
 * 构造函数不为私有函数，暴露这个类，多次构造会出现assert( s_pSingleton == 0 );
 * 优点：
 *    1.继承减少代码量 
 *    2.构造函数不为私有的优点：初始化可以多样，
 *         可急型(定义全局变量createFileSingleton，不用管生存周期)
 *         可缓型(初始化变量initialiseSingleton后进行new构造和delete析构
 *                特点,只能构造一个对象,  构造第二次new就出现assert( s_pSingleton == 0 ))
*/

// 缓型宏后要new 和 delete对象
#define MInitialiseSingleton( type ) template <> type * CMSingleton < type > :: s_pSingleton = 0
// 急型宏后不用管该对象的初始化和释放
#define MCreateSingleton( type ) MInitialiseSingleton( type ); type the##type

#ifndef NULL
#define NULL 0
#endif

template < class type > 
class   CMSingleton {
public:
	CMSingleton( ) {
		// 异常捕捉：s_pSingleton不为空
		assert( s_pSingleton == NULL );
		s_pSingleton = static_cast<type *>(this);
	}
	virtual ~CMSingleton( ) { s_pSingleton = NULL;	}

	static type & getInstance()		
	{ 
		// 异常捕捉：s_pSingleton为空
		assert( s_pSingleton ); 
		return *s_pSingleton; 
	}
	static type * getInstancePtr( )	{ return s_pSingleton; }

protected:
	static type * s_pSingleton;
};

#endif