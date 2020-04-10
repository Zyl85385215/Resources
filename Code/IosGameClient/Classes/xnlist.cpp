//#include <search.h>
#include "xnlist.h"
#include "cocos2d.h"
#include "GlobalDefine.h"

struct xnListData:xnList {
//	HANDLE	semaphore;
	int		bufSize;
};

xnList *  xnList::Create(void)
{
	xnListData * p;
	p = (xnListData * )malloc(sizeof(xnListData));
	p->Count = 0;
	p->bufSize = 8;
//	p->semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	p->Items = (void **)malloc(sizeof(void*) * p->bufSize);
	return p;
}

void xnList::Free(void)
{
//	CloseHandle(((xnListData*)this)->semaphore);
	if (this != NULL)
	{
		free(this->Items);
		free(this);
	}
}

int xnList::Add(void * Item)
{
	if (this == NULL)
		return -1;
//	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
	int Count = this->Count;
	if (Count+1 >= ((xnListData*)this)->bufSize)
	{
		int oldBufSize = ((xnListData*)this)->bufSize;
		int newBufSize = oldBufSize * 2;

		void ** pNewItemBuf = (void**)malloc(sizeof(void*) * newBufSize);
		memcpy(pNewItemBuf, this->Items, sizeof(void*) * oldBufSize);

		free(this->Items);
		this->Items = pNewItemBuf;
		((xnListData*)this)->bufSize = newBufSize;
//		this->Items = (void**)realloc(this->Items, sizeof(void*) * ((xnListData*)this)->bufSize);
	}
	this->Items[Count] = Item;
	this->Count++;
//	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
	return Count;
}

int xnList::Insert(int Index, void * Item)
{
	if ((this == NULL) || (Index < 0))
		return -1;
	int addIndex = this->Add(Item);
	if (Index >= addIndex)
		return addIndex;
//	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
	int moveSize = (addIndex - Index) * sizeof(void*);
	memmove(&this->Items[Index+1], &this->Items[Index], moveSize);
	this->Items[Index] = Item;
//	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
	return Index;
}

void * xnList::Delete(int Index)
{
	if ((this == NULL) || (Index < 0) || (Index >= this->Count))
		return NULL;
//	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
	void * p = this->Items[Index];
	if (Index < this->Count - 1)
	{
		int moveSize = (this->Count -1 - Index) * sizeof(void*);
		memmove(&this->Items[Index], &this->Items[Index+1], moveSize);
	}
	this->Count--;
//	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
	return p;
}

int xnList::Remove(void * Item)
{
	if (this == NULL)
		return -1;
	for (int i=0; i<this->Count; i++)
	{
		if (this->Items[i] == Item)
		{
			this->Delete(i);
			return i;
		}
	}
	return -1;
}

bool xnList::Exchange(int Index1, int Index2)
{
	if ((this == NULL) || (Index1 >= this->Count) || (Index2 >= this->Count))
		return FALSE;
//	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
	void *temp = this->Items[Index1];
	this->Items[Index1] = this->Items[Index2];
	this->Items[Index2] = temp;
//	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
	return TRUE;
}

int xnList::IndexOf(void * Item)
{
	if (this == NULL)
		return -1;
	for (int i=0; i<Count; i++)
	{
		if (Items[i] == Item)
			return i;
	}
	return -1;
}

void xnList::Clear(void)
{
	if (this == NULL)
		return;
//	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
	this->Count = 0;
//	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
}

void xnList::Pack(void)
{
	if (this == NULL)
		return;
	int index = 0;
	while (index < this->Count)
	{
		if (this->Items[index] == NULL)
			this->Delete(index);
		else
			index++;
	}
}


/*
#define CUTOFF 8
#define swap(a, b, width)	\
{							\
	char tmp;				\
	if (a != b)				\
	{						\
		while (width--)		\
		{					\
			tmp = a;		\
			a++ = b;		\
			b++ = tmp;		\
		}					\
	}						\
}

void qsort(void *base, unsigned num, unsigned width, int (*comp)(const void *, const void *))
{
  char *lo, *hi;
  char *mid;
  char *loguy, *higuy;
  unsigned size;
  char *lostk[30], *histk[30];
  int stkptr;

  if (num < 2 || width == 0) return;
  stkptr = 0;

  lo = (char*)base;
  hi = (char*)base + width * (num - 1);

recurse:
  size = (hi - lo) / width + 1;

  if (size <= CUTOFF) 
  {
	  // short sort ----------------------------------
	  char *p, *max;
	  while (hi > lo) 
	  {
		  max = lo;
		  for (p = lo+width; p <= hi; p += width) 
			  if (comp(p, max) > 0) 
				swap(max, hi, width);
		  hi -= width;
	  }
  }
  else 
  {
    mid = lo + (size / 2) * width;
    swap(mid, lo, width);

    loguy = lo;
    higuy = hi + width;

    for (;;) 
    {
      do { loguy += width; } while (loguy <= hi && comp(loguy, lo) <= 0);
      do { higuy -= width; } while (higuy > lo && comp(higuy, lo) >= 0);
      if (higuy < loguy) break;
      swap(loguy, higuy, width);
    }

    swap(lo, higuy, width);

    if (higuy - 1 - lo >= hi - loguy) 
    {
      if (lo + width < higuy) 
      {
        lostk[stkptr] = lo;
        histk[stkptr] = higuy - width;
        ++stkptr;
      }

      if (loguy < hi) 
      {
        lo = loguy;
        goto recurse;
      }
    }
    else
    {
      if (loguy < hi) 
      {
        lostk[stkptr] = loguy;
        histk[stkptr] = hi;
        ++stkptr;
      }

      if (lo + width < higuy) 
      {
        hi = higuy - width;
        goto recurse;
      }
    }
  }

  --stkptr;
  if (stkptr >= 0) 
  {
    lo = lostk[stkptr];
    hi = histk[stkptr];
    goto recurse;
  }
  else
    return;
}
*/

void xnList::Sort(int (*Compare)(const void * Item1, const void * Item2))
{
	if (this == NULL)
		return;
//	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
	qsort(this->Items, this->Count, sizeof(void*), Compare);
//	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
}

//void * xnList::Find(void * item, int (*Find)(const void * Item1, const void * Item2))
//{
//	if (this == NULL)
//		return NULL;
//	void * returnPtr;
////	WaitForSingleObject(((xnListData*)this)->semaphore, INFINITE);
//	returnPtr = lfind(item, this->Items, (unsigned int *)(&this->Count), sizeof(void *), Find);
////	ReleaseSemaphore(((xnListData*)this)->semaphore, 1, NULL);
//	return returnPtr;
//}

void * xnList::Search(void * item, int (*Search)(const void * Item1, const void * Item2))
{
	if (this == NULL)
		return NULL;
    int low = 0;
    int high = this->Count-1;
    while (low <= high)
    {
        int mid = (low+high)/2;
        int k = Search(item, &this->Items[mid]);
        if (k == 0)
            return (&this->Items[mid]);
        if (k < 0)
            high = mid-1;
        if (k > 0)
            low = mid +1;
    }
    return NULL;
}

int xnList::CheckMyOnly( xnList* pOther,xnList * pRtl )
{
	pRtl->Clear ();
	for (int i = 0; i < Count; i++)
	{
		bool bFind	= false;
		for (int j = 0; j < pOther->Count; j++)
		{
			if(Items[i] == pOther->Items[j])
			{
				bFind	= true;
				break;
			}
		}
		if(!bFind)
			pRtl->Add (Items[i]);
	}
	return pRtl->Count;
}