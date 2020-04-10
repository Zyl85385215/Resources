#ifndef _JCH_PROCESS_CMD_H
#define _JCH_PROCESS_CMD_H
enum
{
	WM_SETCTRLWND = WM_USER + 1,
	WM_APPSTATE,
	WM_STOPAPP,
};

enum
{
	state_init,
	state_inactive,
	state_stoped,
	state_active,
	state_created,
	state_starting,
	state_stoping,
	//file state
	state_downloadfinished,
	state_downloadfailed,
	state_starddownload,
	state_downloading,
	state_max
};
#endif