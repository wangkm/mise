#pragma once
#include "Model.h"
#include "afxcmn.h"



// CComInfoDialog dialog

class CComInfoDialog : public CDialog
{
	DECLARE_DYNAMIC(CComInfoDialog)

public:
	CComInfoDialog(CWnd* pParent = NULL);   // standard constructor
	CComInfoDialog(MiseComponent *pComponent, CWnd* pParent = NULL);
	virtual ~CComInfoDialog();

// Dialog Data
	enum { IDD = IDD_COMINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	MiseComponent *m_pComponent;
	CString m_ComponentName;
	CListCtrl m_list_inputs;
	CListCtrl m_list_outputs;
public:
	virtual BOOL OnInitDialog();

public:
	void UpdateInfo();
public:
	afx_msg void OnBnClickedBtnAddinput();
public:
	afx_msg void OnBnClickedBtnAddoutput();
public:
	afx_msg void OnBnClickedBtnInput();
public:
	afx_msg void OnBnClickedBtnDeloutput();
public:
	afx_msg void OnBnClickedBtnAltinput();
public:
	afx_msg void OnBnClickedBtnAltoutput();
};
