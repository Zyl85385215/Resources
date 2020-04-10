#ifndef __XENON_LIST_H__
#define __XENON_LIST_H__

#include <Windows.h>

struct xnList {
	int 	Count;
	void * *Items;

	static xnList * Create(void);
	void	 Free(void);

	int 	Add(void * Item);
	int		Insert(int Index, void * Item);
	void  * Delete(int Index);
	int 	Remove(void * Item);
	BOOLEAN	Exchange(int Index1, int Index2);
	int 	IndexOf(void * Item);
    void    Clear(void);
	void	Pack(void);
	void	Sort(int (*Compare)(const void * Item1, const void * Item2));
	void *	Find(void * item, int (*Find)(const void * Item1, const void * Item2));
	void *  Search(void * item, int (*Search)(const void * Item1, const void * Items));
};

#endif
