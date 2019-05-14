// SinCosDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "SinCosDefDialog.h"
#include ".\sincosdefdialog.h"


// CSinCosDefDialog dialog

IMPLEMENT_DYNAMIC(CSinCosDefDialog, CDialog)
CSinCosDefDialog::CSinCosDefDialog(MiseComponent *pComponent, CWnd* pParent /*=NULL*/)
	: CDialog(CSinCosDefDialog::IDD, pParent)
{
	m_type = pComponent->type;

	switch(m_type)
	{
		case(SRC_SIN):
		{
			Mise_source_sin *pSin = (Mise_source_sin*)pComponent;
			m_A = pSin->m_A;
			m_B = pSin->m_B;
			m_C = pSin->m_C;
			break;
		}

		case(SRC_COS):
		{
			Mise_source_cos *pCos = (Mise_source_cos*)pComponent;
			m_A = pCos->m_A;
			m_B = pCos->m_B;
			m_C = pCos->m_C;
			break;
		}

		default:
		{
//			AfxMessageBox("非法操作");
			break;
		}
	}
}

CSinCosDefDialog::~CSinCosDefDialog()
{
}

void CSinCosDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_A);
	DDX_Text(pDX, IDC_EDIT2, m_B);
	DDX_Text(pDX, IDC_EDIT3, m_C);
}


BEGIN_MESSAGE_MAP(CSinCosDefDialog, CDialog)
END_MESSAGE_MAP()


// CSinCosDefDialog message handlers

BOOL CSinCosDefDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	switch(m_type)
	{
		case(SRC_SIN):
		{
			GetDlgItem(IDC_EDIT4)->SetWindowText("正弦信号源");
			GetDlgItem(IDC_EDIT5)->SetWindowText("用法：output = A * sin( B * time + C )");
			break;
		}

		case(SRC_COS):
		{
			GetDlgItem(IDC_EDIT4)->SetWindowText("余弦信号源");
			GetDlgItem(IDC_EDIT5)->SetWindowText("用法：output = A * cos( B * time + C )");
			break;
		}

		default:
		{
//			AfxMessageBox("非法操作");
			break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
