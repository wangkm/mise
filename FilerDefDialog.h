#pragma once
#include "MiseLibs.h"

// CFilerDefDialog dialog

class CFilerDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CFilerDefDialog)

public:
	CFilerDefDialog(Mise_output_filer *pFiler, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilerDefDialog();

// Dialog Data
	enum { IDD = IDD_DEF_FILER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_fileFullPath;
	BOOL m_withTime;
	BOOL m_withTitle;
	afx_msg void OnBnClickedButton1();
};
