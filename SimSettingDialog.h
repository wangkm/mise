#pragma once


// CSimSettingDialog dialog

class CSimSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CSimSettingDialog)

public:
	CSimSettingDialog(double startTime, double endTime, double stepTime, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSimSettingDialog();

// Dialog Data
	enum { IDD = IDD_SIMSETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_startTime;
	double m_endTime;
	double m_stepTime;
};
