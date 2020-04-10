#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

struct xnLinkData {
	xnLinkData * next;
	xnLinkData * prev;
	void * data;
};

struct xnLinkList {
	xnLinkData * start;
	xnLinkData * end;
	xnLinkData * position;
	xnLinkData ** index;
	int	indexSize;
	int	totalRecord;
	void * data;
};

xnLinkList * xnLinkAlloc(BOOLEAN makeIndex = FALSE);
void xnLinkFree(xnLinkList * linkList);

xnLinkData * xnLinkInsert(xnLinkList * linkList, void * data, BOOLEAN (* compares)(void * d1, void * d2) = NULL);
void xnLinkDelete(xnLinkList * linkList, xnLinkData * linkData);

void * xnLinkGotoFirst(xnLinkList * linkList);
void * xnLinkGotoLast(xnLinkList * linkList);
void * xnLinkGotoNext(xnLinkList * linkList);
void * xnLinkGotoPrev(xnLinkList * linkList);

void * xnLinkGoto(xnLinkList * linkList, int index);
void * xnLinkFind(xnLinkList * linkList, BOOLEAN (* find)(void * data));

struct xnLinkObj {
	xnLinkObj * Next, * Prev;
	void * Obj;
};

struct xnLink {
	xnLinkObj * Start, * End;
	int		Count;
	static 	xnLink * Create(void);
	void	Free(void);
	xnLinkObj * InsertObj(void * p, BOOLEAN (* compares)(void * d1, void * d2) = NULL);
	void	DeleteObj(void * p);
	void	DeleteObj(xnLinkObj * p);
};


#endif
