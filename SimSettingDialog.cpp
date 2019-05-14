// SimSettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "SimSettingDialog.h"


// CSimSettingDialog dialog

IMPLEMENT_DYNAMIC(CSimSettingDialog, CDialog)

CSimSettingDialog::CSimSettingDialog(double startTime, double endTime, double stepTime, CWnd* pParent /*=NULL*/)
	: CDialog(CSimSettingDialog::IDD, pParent)
{
	m_startTime = startTime;
	m_endTime	= endTime;
	m_stepTime	= stepTime;
}

CSimSettingDialog::~CSimSettingDialog()
{
}

void CSimSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_startTime);
	DDX_Text(pDX, IDC_EDIT2, m_endTime);
	DDX_Text(pDX, IDC_EDIT3, m_stepTime);
}


BEGIN_MESSAGE_MAP(CSimSettingDialog, CDialog)
END_MESSAGE_MAP()


// CSimSettingDialog message handlers
