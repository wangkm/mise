#pragma once
#include "Model.h"

// CComDefDialog dialog

class CComDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CComDefDialog)

public:
	CComDefDialog(CWnd* pParent = NULL);   // standard constructor
	CComDefDialog(MiseComponent *pComponent, CWnd* pParent = NULL);
	virtual ~CComDefDialog();

// Dialog Data
	enum { IDD = IDD_COMDEF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	MiseComponent *m_pComponent;
	CString m_name;
	CString m_dllPath;
	afx_msg void OnBnClickedButton1();
public:
	CString m_comment;
	CString m_funcName;
public:
	afx_msg void OnEnChangeEdit1();
};
