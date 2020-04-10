#ifndef _GAME_STRING_H
#define _GAME_STRING_H

//#include "xnlist.h"
#include <windows.h>
#include "xnlist.h"
#define MAX_LINE 100   //可容纳的文本行数

// enum
// {
// 	mouse_check_normal,
// 	mouse_check_on,
// 	mouse_check_press
// };

// enum
// {
// 	LBTN_DWN,
// 	RBTN_DWN,
// 	DLBTN_DWN,
// 	DRBTN_DWN,
// }
// struct MSGMAP_ENTRY {
// 	UINT nMessage;
// 	LONG (*pfn)(HWND, UINT, WPARAM, LPARAM);
// };
// 
// struct MSGMAP_ENTRY _commandEntries =
// {
// 	LBTN_DWN, OnLBtnDwn,
// 	RBTN_DWN, OnRBtnDwn,
// 	DLBTN_DWN, OnDLbtnDwn,
// 	DRBTN_DWN, OnDLbtnDwn,
// } ;
class CGameStringList;
class CGameStringItem
{
	friend class CGameStringList;
public:
	CGameStringItem();
	CGameStringItem(char* pszName,void* pData = NULL);
	~CGameStringItem();

	void			InitItem(char* pszName,void* pData = NULL);
	char*			GetString();
	void*			GetData();
	void			SetString(char* pString);
	void			SetData(void* pData);

	BOOL			m_bChangeLine;    //是否强制换行
	BOOL			m_bIndent;        //是否缩进
	BYTE			m_bCheckMouse;    //鼠标状态
protected:
	char *			m_pString;
    void *			m_pData;

	int				m_nViewH;
	int				m_nViewW;
	POINT			m_ptView;		  //起点位置,绘制时候确定
};

struct LineInfoA
{
	int index;						  //行号
	int height;						  //行高
	int indent;						  //非自动换行的缩进位置
};

class CGameStringList
{
public:
	CGameStringList();
	~CGameStringList();

	void	 SetEditStr(char *Caption);

	void	 AddItem(char* pszName,void* pData = NULL,BOOL bChangeLine=FALSE,BOOL bIndent=FALSE);
	void	 AddItem(CGameStringItem *pItem);
	void	 InsertItem(int nIndex,CGameStringItem *pItem);
	void	 InsertItem(int nIndex,char* pszName,void* pData = NULL,BOOL bChangeLine=FALSE,BOOL bIndent=FALSE);
	void	 InsertText(char* pszName,BOOL bChangeLine=FALSE,BOOL bIndent=FALSE);	
	void	 DelItem(int nIndex);
	void	 ClearItems();

	void	 Analyze();       //由二进制转回字符对象
	void	 synthesize();    //由字符对象转回二进制  

	xnList*  m_pStringList;
protected:

//=============================================================
//编辑文本
public:
	void  	 SetViewRect(POINT *pt,int w);							 //设置视区位置

	void	 DrawHEditView(POINT *pt,int w,COLORREF color,int alpha);//显示视区
	void  	 InitEditText();										 //初始化虚拟窗口
	int      GetPosX();
protected:
	int		 m_dxStart;
	int		 m_dxPos;
	int		 m_nMaxLength;

//=============================================================
//显示垂直滚动文本
public:	
	void  	 SetViewRect(RECT *rect);								//设置视区位置	
	int 	 GetLineNum();
	int		 GetViewLine();											//获取行数目,更新滚动条	
	int 	 SetEndLine(int nPos);
	int 	 MoveNextLine();
	int 	 MovePreLine();											//更新滚动条位置&&显示虚拟窗口信息的可见内容     
	void	 DrawVScrollView(RECT *rect,COLORREF color,int alpha);	//显示视区	
	void	 DrawVScrollViewDown(RECT *rect,COLORREF color,int alpha);
	void  	 InitViewText(BOOL bAllInit = FALSE);					//初始化虚拟窗口

	int	 	 m_dxEndLine;											//可见窗口显示的末尾位置
protected:		
	//内容虚拟窗口属性
	LineInfoA m_ptrLine[MAX_LINE];									//行信息	
	int		 m_lineCount;											//虚拟窗口的行数  应小于MAX_LINE	

	int		 m_nInitedCount;										//已经初始化的对象个数,下次InitViewText的时候从该个数开始InitViewText

//================================================================
//Item Edit
protected:
	virtual void	DrawItem(CGameStringItem *pItem,int x, int y,int w, COLORREF color,int alpha);
	virtual int		GetWidth(CGameStringItem *pItem);
    virtual int		GetHeight(CGameStringItem *pItem);

protected:
	int      m_nWordSpace;											//对象间距
	int      m_nLineSpace;											//行间距
	
    //可见窗口属性
	POINT	 m_viewPt;												//可见窗口位置
	int		 m_viewH;               
	int		 m_viewW;												//可见窗口大小
	int      m_viewLine;											//可见窗口行数

	char*    m_pszCaption;											//连接绘图的Caption,实现输入更新,编辑字串
};

 
// void xnGameStringBegin(char * editBuf, int bufSize, int inputType = SINGLE_BYTE_CHAR);
// void xnGameStringEnd(void);
// void xnGameStringInsert(char * insertStr);
// void xnGameStringSetStr(char * editStr);
// void xnGameStringLinkData(char* szName,char* pData);
// void xnGameStringInsertFace(BYTE bIndex);
// BOOLEAN xnGameStringCheckEditEnd(void);

#endif