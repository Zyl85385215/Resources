#ifndef __INPUT_H__
#define __INPUT_H__

#define	INPUT_COMMAND_BUFFER	256
// #define NEW_GAME_TEXT_
// 
// #ifdef NEW_GAME_TEXT_
// #include "GameText.h"
// #endif

enum  {
	DEVICE_KEYBOARD,
	DEVICE_JOYSTICK,
	DEVICE_MOUSE
};

enum  {
	KEY_PRESS,
	KEY_RELEASE
};

struct xnInputCmd {
	BYTE keyIndex;
	BYTE command;
	DWORD timeTick;
};

struct xnInput
{
	BOOLEAN * keyStatus;
	int useKeyNumber;
	BYTE *	useKey;
	BOOLEAN * keyPressFlag;
	xnInputCmd * keyCmdBuf;
	int keyWCmdIndex;
	int keyRCmdIndex;
	int device;

	void SendCmd(xnInputCmd * pCmd);
	xnInputCmd * RecvCmd(void);
	xnInputCmd * CheckCmd(void);
	void ClearCmd(int index);
	void ClearCmd(void);
};

void xnInputInit(void);
void xnInputShutDown(void);
void xnInputPoll(void);
void xnInputClear(void);
void xnInputIgnore(BOOLEAN bIgnore);

xnInput * xnInputAlloc(int keyNumber, BYTE * keyMap, int inputDevice = DEVICE_KEYBOARD);
void xnInputFree(xnInput * kGroup);

enum {
	NUM_ONLY,
	SINGLE_BYTE_CHAR,
	DOUBLE_BYTE_CHAR
};

enum
{
	str_flag_null,
	str_flag_color,
	str_flag_link_txt,
	str_flag_link_data,
	str_flag_face,
	str_flag_to_do_event, // 在该位置去做一些绘制事件
};

struct StringEdit {
	char *	editStr;
	BOOLEAN insertMode;
	int		editPos;
	int		maxLength;
	int		editType;
	//new edit 
	int		m_maxEditLength;
	char*	m_caption;
	int		m_startPos,m_endPos;	//顯示的位置
	int		m_editPos;		//保存操作前的位置
	bool	m_blEditEnd;

	static StringEdit * Create(char* editBuf,char * caption, int maxLength, int editType,int editSize);
	void Free(void);
	
	void Set(char * str);
	void Insert(char * str);
	void InsertChar(int charCode);
	void EditProcess(int keyCode);
	void EditKeyDownCmd(int keyCode);
	void EditKeyUpCmd(int keyCode);
	void InsertLinkData(char* szName,char* pData);
	void InsertFace(BYTE bIndex);
	void AdjustView();
};

extern StringEdit * pStringEdit;
// #ifdef NEW_GAME_TEXT_
// extern CGameTextView* g_pGTextView;
// #endif
void xnInputEditBegin(char* editBuf, char* captionBuf,int bufSize, int inputType = SINGLE_BYTE_CHAR,int editSize = -1);
void xnInputEditEnd(void);
void xnInputEditInsert(char * insertStr);
void xnInputEditSetStr(char * editStr);
void xnInputInsertLinkData(char* szName,char* pData);
void xnInputInsertFace(BYTE bIndex);
BOOLEAN xnInputCheckEditEnd(void);

char * xnInputGetImeName(void);

extern LANGID langid;
char * __fastcall Gb2Big(char * pszText);
char * __fastcall Big2Gb(char * pszText);

#pragma comment(lib,"imm32.lib")

#endif
