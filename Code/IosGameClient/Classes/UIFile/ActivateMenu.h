#ifndef ActivateMenu_h__
#define ActivateMenu_h__

#include "CtrlObj.h"

class ActivateMenu
{
public:
	ActivateMenu();
	~ActivateMenu();
	void Open();
	void Close();
	DForm* m_pForm;
};


#endif // ActivateMenu_h__