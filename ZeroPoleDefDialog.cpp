// ZeroPoleDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "ZeroPoleDefDialog.h"
#include "Model.h"
#include "MyDataDef.h"

// CZeroPoleDefDialog dialog

IMPLEMENT_DYNAMIC(CZeroPoleDefDialog, CDialog)

CZeroPoleDefDialog::CZeroPoleDefDialog(Mise_cont_ZeroPole *pTranFun, CWnd* pParent /*=NULL*/)
	: CDialog(CZeroPoleDefDialog::IDD, pParent)
	, m_plus(0)
{
	// ��ȡԭ����ϵ��
	m_upParameters = "[";
	m_downParameters = "[";
	m_upList.clear();
	m_downList.clear();
	for ( unsigned int i = 0; i < pTranFun->m_upList.size(); i++)
	{
		m_upList.push_back(pTranFun->m_upList[i]);
		CString strTmp;
		strTmp.Format(" %f", pTranFun->m_upList[i]);
		DelEndZeroFromStr(strTmp);
		m_upParameters += strTmp;
	}
	m_upParameters += " ]";
	for ( unsigned int i = 0; i < pTranFun->m_downList.size(); i++)
	{
		m_downList.push_back(pTranFun->m_downList[i]);
		CString strTmp;
		strTmp.Format(" %f", pTranFun->m_downList[i]);
		DelEndZeroFromStr(strTmp);
		m_downParameters += strTmp;
	}
	m_downParameters += " ]";

	m_plus = pTranFun->m_plus;

}

CZeroPoleDefDialog::~CZeroPoleDefDialog()
{
}

void CZeroPoleDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_upParameters);
	DDX_Text(pDX, IDC_EDIT2, m_downParameters);
	DDX_Text(pDX, IDC_EDIT3, m_plus);
}


BEGIN_MESSAGE_MAP(CZeroPoleDefDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CZeroPoleDefDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CZeroPoleDefDialog message handlers

void CZeroPoleDefDialog::OnBnClickedOk()
{
	// �����ǰ����
	m_upList.clear();
	m_downList.clear();

	// ��������
	UpdateData(TRUE);
	// ��ȡ����ϵ��
	if(!ParseParameters(m_upParameters, m_upList))
	{
		AfxMessageBox("��ȡ����ϵ��ʧ��");
		return;
	}

	// ��ȡ��ĸϵ��
	if(!ParseParameters(m_downParameters, m_downList))
	{
		AfxMessageBox("��ȡ��ĸϵ��ʧ��");
		return;
	}

	//��������
	if ( fabs(m_plus ) < 1E-100)
	{
		AfxMessageBox("���治��Ϊ��");
		return;
	}

	OnOK();
}
