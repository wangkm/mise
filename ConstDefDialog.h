#pragma once
#include "MiseLibs.h"


// CConstDefDialog dialog

class CConstDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CConstDefDialog)

public:
	CConstDefDialog(Mise_source_const *pConst, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConstDefDialog();

// Dialog Data
	enum { IDD = IDD_DEF_CONST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_value;
};
