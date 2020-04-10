#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>

// ==================Ascent�е�ģ�嵥����===================
// ���˸о�ȱ��:���캯����Ϊ˽�к�����ȱ�㣺��¶����࣬��ι�������assert( s_pSingleton == 0 );
// �÷��ŵ㣺
//    1.�̳м��ٴ����� 
//    2.���캯����Ϊ˽�е��ŵ㣺��ʼ�����Զ�����
//         �ɼ���(����ȫ�ֱ���createFileSingleton�����ù���������)
//         �ɻ���(��ʼ������initialiseSingleton�����new�����delete����
//                �ص�,ֻ�ܹ���һ������,  ����ڶ���new�ͳ���assert( s_pSingleton == 0 ))

// ���ͺ��Ҫnew �� delete����
#define InitialiseSingleton( type ) \
	template <> type * CSingleton < type > :: s_pSingleton = 0

// ���ͺ���ùܸö���ĳ�ʼ�����ͷ�
#define CreateSingleton( type ) \
	InitialiseSingleton( type ); \
	type the##type

template < class type > 
class CSingleton {
public:
	CSingleton( ) {
		assert( s_pSingleton == 0 );
		s_pSingleton = static_cast<type *>(this);
	}
	virtual ~CSingleton( ) {	s_pSingleton = 0;	}

	inline static type & GetInstance()		{ assert( s_pSingleton ); return *s_pSingleton; }
	inline static type * GetInstancePtr( )	{ return s_pSingleton; }

protected:
	static type * s_pSingleton;
};

#endif