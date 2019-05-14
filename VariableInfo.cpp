// VariableInfo.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "VariableInfo.h"


// CVariableInfo dialog

IMPLEMENT_DYNAMIC(CVariableInfo, CDialog)

CVariableInfo::CVariableInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CVariableInfo::IDD, pParent)
	, m_varName(_T(""))
	, m_default_value(0)
	, m_varType(_T(""))
{

}

CVariableInfo::~CVariableInfo()
{
}

void CVariableInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_varName);
	DDX_Text(pDX, IDC_EDIT2, m_default_value);
	DDX_CBString(pDX, IDC_varType, m_varType);
}


BEGIN_MESSAGE_MAP(CVariableInfo, CDialog)
END_MESSAGE_MAP()


// CVariableInfo message handlers

BOOL CVariableInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->SetWindowTextA(m_title);
	if ( m_title == "添加输出变量" )
	{
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	}

	CComboBox *pComboBox;
	pComboBox = (CComboBox*)this->GetDlgItem(IDC_varType);
	pComboBox->AddString("double");
	pComboBox->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
