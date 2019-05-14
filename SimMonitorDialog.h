#pragma once
#include "resource.h"

// CSimMonitorDialog dialog

class CSimMonitorDialog : public CDialog
{
	DECLARE_DYNAMIC(CSimMonitorDialog)

public:
	vector<double>				*m_pSimTime;
	vector< vector<double> >	*m_pVarValues;
	double						*m_pTop;
	double						*m_pBottom;
	double						*m_pScaleUp;
	double						*m_pScaleDown;
	LOGFONT	m_mylf;				//×ÖÌå
public:
	CSimMonitorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSimMonitorDialog();

// Dialog Data
	enum { IDD = IDD_MONITOR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
