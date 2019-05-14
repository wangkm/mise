// TransFunDefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "TransFunDefDialog.h"
#include "Model.h"
#include "MyDataDef.h"



// CTransFunDefDialog dialog

IMPLEMENT_DYNAMIC(CTransFunDefDialog, CDialog)

CTransFunDefDialog::CTransFunDefDialog(Mise_cont_transfunction *pTranFun, CWnd* pParent /*=NULL*/)
	: CDialog(CTransFunDefDialog::IDD, pParent)
{
	// 读取原来的系数
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

}

CTransFunDefDialog::~CTransFunDefDialog()
{
}

void CTransFunDefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_upParameters);
	DDX_Text(pDX, IDC_EDIT2, m_downParameters);
}


BEGIN_MESSAGE_MAP(CTransFunDefDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CTransFunDefDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CTransFunDefDialog message handlers

void CTransFunDefDialog::OnBnClickedOk()
{
	// 清空以前数据
	m_upList.clear();
	m_downList.clear();

	// 更新数据
	UpdateData(TRUE);
	// 获取分子系数
	if(!ParseParameters(m_upParameters, m_upList) || m_upList.size() == 0)
	{
		AfxMessageBox("获取分子系数失败");
		return;
	}

	// 获取分母系数
	if(!ParseParameters(m_downParameters, m_downList) || m_downList.size() == 0)
	{
		AfxMessageBox("获取分母系数失败");
		return;
	}


	OnOK();
}
