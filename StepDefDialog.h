#pragma once
#include "MiseLibs.h"


// CStepDefDialog dialog

class CStepDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CStepDefDialog)

public:
	CStepDefDialog(Mise_source_step *pStep, CWnd* pParent = NULL);   // standard constructor
	virtual ~CStepDefDialog();

// Dialog Data
	enum { IDD = IDD_DEF_STEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_T;
public:
	double m_A;
public:
	double m_B;
};
