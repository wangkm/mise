// FilerDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "FilerDefDialog.h"
#include ".\filerdefdialog.h"


// CFilerDefDialog dialog

IMPLEMENT_DYNAMIC(CFilerDefDialog, CDialog)
CFilerDefDialog::CFilerDefDialog(Mise_output_filer *pFiler, CWnd* pParent /*=NULL*/)
	: CDialog(CFilerDefDialog::IDD, pParent)
{
	m_fileFullPath	= pFiler->m_fileName;
	m_withTime		= pFiler->m_withTime;
	m_withTitle		= pFiler->m_withTitle;
}

CFilerDefDialog::~CFilerDefDialog()
{
}

void CFilerDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fileFullPath);
	DDX_Check(pDX, IDC_CHECK1, m_withTime);
	DDX_Check(pDX, IDC_CHECK3, m_withTitle);
}


BEGIN_MESSAGE_MAP(CFilerDefDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CFilerDefDialog message handlers

void CFilerDefDialog::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CFileDialog dlgFile(FALSE, "txt", "*.txt", OFN_OVERWRITEPROMPT, "文本文件(*.txt)|*.txt||");
	if(dlgFile.DoModal() == IDOK)
	{
		m_fileFullPath = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}

