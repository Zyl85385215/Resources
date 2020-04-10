#pragma once
#include "afxwin.h"
#include "Resource.h"
#define WM_LUAOUTPUT (WM_USER+1000)
// LuaDialog dialog

class LuaDialog : public CDialogEx
{
	DECLARE_DYNAMIC(LuaDialog)

public:
	LuaDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~LuaDialog();

// Dialog Data
	enum { IDD = IDD_LUA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CEdit LuaString;
	afx_msg void OnBnClickedOk();
	CComboBox LuaTemplate;
	afx_msg void OnCbnSelchangeCombo1();
	CString LuaOutPut;
	afx_msg LRESULT OnUpdateOutput(WPARAM wParam, LPARAM lParam);
	void	AddOutput(const char* szTxt);
};
extern	LuaDialog* g_pLuaDialog;
