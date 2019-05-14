// StepDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "StepDefDialog.h"


// CStepDefDialog dialog

IMPLEMENT_DYNAMIC(CStepDefDialog, CDialog)

CStepDefDialog::CStepDefDialog(Mise_source_step *pStep, CWnd* pParent /*=NULL*/)
	: CDialog(CStepDefDialog::IDD, pParent)
{
	m_T = pStep->m_T;
	m_A = pStep->m_A;
	m_B = pStep->m_B;
}

CStepDefDialog::~CStepDefDialog()
{
}

void CStepDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_T);
	DDX_Text(pDX, IDC_EDIT2, m_A);
	DDX_Text(pDX, IDC_EDIT3, m_B);
}


BEGIN_MESSAGE_MAP(CStepDefDialog, CDialog)
END_MESSAGE_MAP()


// CStepDefDialog message handlers
