// ComDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "ComDefDialog.h"


// CComDefDialog dialog

IMPLEMENT_DYNAMIC(CComDefDialog, CDialog)

CComDefDialog::CComDefDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CComDefDialog::IDD, pParent)
	, m_name(_T(""))
	, m_dllPath(_T(""))
	, m_comment(_T(""))
	, m_funcName(_T(""))
{

}

CComDefDialog::CComDefDialog(MiseComponent *pComponent, CWnd* pParent /*=NULL*/)
	: CDialog(CComDefDialog::IDD, pParent)
{
	ASSERT(pComponent);
	m_pComponent = pComponent;
	m_name = pComponent->name;
	m_dllPath = pComponent->dllPath;
	m_funcName = pComponent->functionName;
	m_comment = pComponent->comment;
}

CComDefDialog::~CComDefDialog()
{
}

void CComDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_dllPath);
	DDX_Text(pDX, IDC_EDIT3, m_comment);
	DDX_Text(pDX, IDC_EDIT4, m_funcName);
}


BEGIN_MESSAGE_MAP(CComDefDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CComDefDialog::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CComDefDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CComDefDialog message handlers

void CComDefDialog::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "¶¯Ì¬Á´½Ó¿â(*.dll)|*.dll||");
	if(dlgFile.DoModal() == IDOK)
	{
		m_dllPath = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CComDefDialog::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strTmp;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(strTmp);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(strTmp);
}
