#pragma once
#include "MiseLibs.h"


// CZeroPoleDefDialog dialog

class CZeroPoleDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CZeroPoleDefDialog)

public:
	CZeroPoleDefDialog(Mise_cont_ZeroPole *pTranFun, CWnd* pParent = NULL);   // standard constructor
	virtual ~CZeroPoleDefDialog();

// Dialog Data
	enum { IDD = IDD_DEF_ZEROPOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CString m_upParameters;
	CString m_downParameters;

	vector<double> m_upList, m_downList;
public:
	double m_plus;
};
