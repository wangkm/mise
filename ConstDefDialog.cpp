// ConstDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "ConstDefDialog.h"


// CConstDefDialog dialog

IMPLEMENT_DYNAMIC(CConstDefDialog, CDialog)
CConstDefDialog::CConstDefDialog(Mise_source_const *pConst, CWnd* pParent /*=NULL*/)
	: CDialog(CConstDefDialog::IDD, pParent)
{
	m_value = pConst->m_value;
}

CConstDefDialog::~CConstDefDialog()
{
}

void CConstDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_value);
}


BEGIN_MESSAGE_MAP(CConstDefDialog, CDialog)
END_MESSAGE_MAP()


// CConstDefDialog message handlers
