#ifndef _XN_ACTION_H_
#define _XN_ACTION_H_

#include "xnlist.h"

enum ActionStatus {
	ACTION_NONE,
	ACTION_PLAYING,
	ACTION_END,
	ACTION_LOOP
};

#pragma pack (push)
#pragma pack (1)

struct ActionFrameData 
{
	int	sprFrameIndex;
	int	sprFrameDelay;
};

struct ActionData 
{
	char *				name;
	int					totalFrame;
	ActionFrameData *	frameData;
	char **				event;
	BOOLEAN				loopFlag;
};


struct xnAction {
	int 				sprFrame;			// 画面帧，当前动作帧对应的画面帧
	int					actionFrameIndex;	// 动作帧
	DWORD				m_dwTick;			// 
	ActionData *		selectedAction;
	ActionStatus		status;
	int					frameDelay;

	void				SetEventCallback(void (*Event)(char * szEvent, void * data), void * data);	

	static	xnAction *	Load(char * actionFileName);
	void				Free();


	void				SetSpeed(int speed);			// 帧数，默认是25帧

	BOOLEAN				SetAction(char * actionName);
	void				SeekToEnd(void);
	void				SeekToBegin(void);
	void				SeekToFrame(int frameIndex);

	ActionStatus		Update(void);
private:
	void				_nextFrame();
	void				_setNullAction();
	void				_setAction(ActionData *	pAction);
	void				_callEvent(char *eventString);

	void *				m_pEventCallbackParam;
	void				(*m_funcEventCallback)(char * szEvent, void * data);

	xnList *			m_listAction;
	int					m_nSpeedPercent;
};

class ActionBase
{
public:
	ActionBase();
	~ActionBase();
	char *			   m_szName;
	xnList *           m_pList;
	BYTE *			   m_pReadBuf;
	bool               LoadAction(char * actionFileName);
};

class WorldAction
{
public:
	WorldAction();
	~WorldAction();
	void               AddAction(ActionBase * pActionBase);
	ActionBase*        GetActionBase(char * actionFileName);
	xnList*            m_pActionList;
};

#pragma pack (pop)
#endif
