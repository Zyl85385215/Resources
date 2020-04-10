#ifndef _GAME_STRING_H
#define _GAME_STRING_H

//#include "xnlist.h"
#include <windows.h>
#include "xnlist.h"
#define MAX_LINE 100   //�����ɵ��ı�����

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

	BOOL			m_bChangeLine;    //�Ƿ�ǿ�ƻ���
	BOOL			m_bIndent;        //�Ƿ�����
	BYTE			m_bCheckMouse;    //���״̬
protected:
	char *			m_pString;
    void *			m_pData;

	int				m_nViewH;
	int				m_nViewW;
	POINT			m_ptView;		  //���λ��,����ʱ��ȷ��
};

struct LineInfoA
{
	int index;						  //�к�
	int height;						  //�и�
	int indent;						  //���Զ����е�����λ��
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

	void	 Analyze();       //�ɶ�����ת���ַ�����
	void	 synthesize();    //���ַ�����ת�ض�����  

	xnList*  m_pStringList;
protected:

//=============================================================
//�༭�ı�
public:
	void  	 SetViewRect(POINT *pt,int w);							 //��������λ��

	void	 DrawHEditView(POINT *pt,int w,COLORREF color,int alpha);//��ʾ����
	void  	 InitEditText();										 //��ʼ�����ⴰ��
	int      GetPosX();
protected:
	int		 m_dxStart;
	int		 m_dxPos;
	int		 m_nMaxLength;

//=============================================================
//��ʾ��ֱ�����ı�
public:	
	void  	 SetViewRect(RECT *rect);								//��������λ��	
	int 	 GetLineNum();
	int		 GetViewLine();											//��ȡ����Ŀ,���¹�����	
	int 	 SetEndLine(int nPos);
	int 	 MoveNextLine();
	int 	 MovePreLine();											//���¹�����λ��&&��ʾ���ⴰ����Ϣ�Ŀɼ�����     
	void	 DrawVScrollView(RECT *rect,COLORREF color,int alpha);	//��ʾ����	
	void	 DrawVScrollViewDown(RECT *rect,COLORREF color,int alpha);
	void  	 InitViewText(BOOL bAllInit = FALSE);					//��ʼ�����ⴰ��

	int	 	 m_dxEndLine;											//�ɼ�������ʾ��ĩβλ��
protected:		
	//�������ⴰ������
	LineInfoA m_ptrLine[MAX_LINE];									//����Ϣ	
	int		 m_lineCount;											//���ⴰ�ڵ�����  ӦС��MAX_LINE	

	int		 m_nInitedCount;										//�Ѿ���ʼ���Ķ������,�´�InitViewText��ʱ��Ӹø�����ʼInitViewText

//================================================================
//Item Edit
protected:
	virtual void	DrawItem(CGameStringItem *pItem,int x, int y,int w, COLORREF color,int alpha);
	virtual int		GetWidth(CGameStringItem *pItem);
    virtual int		GetHeight(CGameStringItem *pItem);

protected:
	int      m_nWordSpace;											//������
	int      m_nLineSpace;											//�м��
	
    //�ɼ���������
	POINT	 m_viewPt;												//�ɼ�����λ��
	int		 m_viewH;               
	int		 m_viewW;												//�ɼ����ڴ�С
	int      m_viewLine;											//�ɼ���������

	char*    m_pszCaption;											//���ӻ�ͼ��Caption,ʵ���������,�༭�ִ�
};

 
// void xnGameStringBegin(char * editBuf, int bufSize, int inputType = SINGLE_BYTE_CHAR);
// void xnGameStringEnd(void);
// void xnGameStringInsert(char * insertStr);
// void xnGameStringSetStr(char * editStr);
// void xnGameStringLinkData(char* szName,char* pData);
// void xnGameStringInsertFace(BYTE bIndex);
// BOOLEAN xnGameStringCheckEditEnd(void);

#endif