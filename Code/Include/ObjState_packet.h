#ifndef _OBJ_STATE_PACKET_H
#define _OBJ_STATE_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum{
	ADD_STATE,
	REMOVE_STATE,	
	UPDATE_STATE,
	ALL_STATE,
	REQUEST_REMOVE_STATE,
//add by hjt
	SEND_STATEFLAG,
//end add 
};

struct STATEEFFECT_ADD_STATE_Data {
	WORD		wStateID;
	DWORD		dwUin;
	BYTE		bType;
	DWORD   dwStateFlag;	//add at 2006-11-7
	DWORD		dwInterval;
	BYTE		bDisapper;
	BYTE		bLevel;
};

//add by hjt
struct STATEEFFECT_SEND_STATEFLAG_Data {
	DWORD		dwUin;
	DWORD   dwStateFlag;
};
//end add

struct STATEEFFECT_REMOVE_STATE_Data {
	WORD		wStateID;
	DWORD		dwUin;
	BYTE		bType;
	DWORD   dwStateFlag;	//add at 2006-11-7
};

struct STATEEFFECT_UPDATE_STATE_Data			//add at 2006-11-7
{
	DWORD		wStateID;
	DWORD		dwUin;
	BYTE		bType;
	DWORD		dwInterval;	
	BYTE		bLevel;
};

struct PACKET_STATE_INFO
{
	WORD		wStateID;
	DWORD		dwInterval;
	BYTE		bDisapper;
	BYTE		bLvl;
};

struct STATEEFFECT_ALL_STATE_Data {
	DWORD				dwUin;
	BYTE				bType;
	DWORD				dwStateFlag;	//add at 2006-11-7
	int					nStateCnt;
	PACKET_STATE_INFO	xStates[1];
};

struct STATEEFFECT_REQUEST_REMOVE_STATE_Data
{
	WORD		wStateID;
	DWORD		dwUin;
	BYTE		bType;
};
#pragma pack (pop)
#endif