#pragma once
#include "MiseLibs.h"


// CTransFunDefDialog dialog

class CTransFunDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CTransFunDefDialog)

public:
	CTransFunDefDialog(Mise_cont_transfunction *pTranFun, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTransFunDefDialog();

// Dialog Data
	enum { IDD = IDD_DEF_TRANSFUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_upParameters;
	CString m_downParameters;

	vector<double> m_upList, m_downList;
public:
	afx_msg void OnBnClickedOk();
};
