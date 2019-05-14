// ComInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "ComInfoDialog.h"
#include "VariableInfo.h"
#include "MyDataDef.h"
#include <stdlib.h>


// CComInfoDialog dialog

IMPLEMENT_DYNAMIC(CComInfoDialog, CDialog)

CComInfoDialog::CComInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CComInfoDialog::IDD, pParent)
	, m_ComponentName(_T(""))
{

}

CComInfoDialog::CComInfoDialog(MiseComponent *pComponent, CWnd* pParent /*=NULL*/)
	: CDialog(CComInfoDialog::IDD, pParent)
	, m_ComponentName(_T(""))
{
	ASSERT(pComponent);
	m_pComponent = pComponent;
}

CComInfoDialog::~CComInfoDialog()
{
}

void CComInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CompName, m_ComponentName);
	DDX_Control(pDX, IDC_LIST_Input, m_list_inputs);
	DDX_Control(pDX, IDC_LIST_Output, m_list_outputs);
}


BEGIN_MESSAGE_MAP(CComInfoDialog, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDINPUT, &CComInfoDialog::OnBnClickedBtnAddinput)
	ON_BN_CLICKED(IDC_BTN_ADDOUTPUT, &CComInfoDialog::OnBnClickedBtnAddoutput)
	ON_BN_CLICKED(IDC_BTN_INPUT, &CComInfoDialog::OnBnClickedBtnInput)
	ON_BN_CLICKED(IDC_BTN_DELOUTPUT, &CComInfoDialog::OnBnClickedBtnDeloutput)
	ON_BN_CLICKED(IDC_BTN_ALTINPUT, &CComInfoDialog::OnBnClickedBtnAltinput)
	ON_BN_CLICKED(IDC_BTN_ALTOUTPUT, &CComInfoDialog::OnBnClickedBtnAltoutput)
END_MESSAGE_MAP()


// CComInfoDialog message handlers

BOOL CComInfoDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//修改列表的外观
	m_list_inputs.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_outputs.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_list_inputs.InsertColumn(0, "名称", LVCFMT_RIGHT, 100);
	m_list_inputs.InsertColumn(1, "类型", LVCFMT_CENTER, 60);
	m_list_inputs.InsertColumn(2, "缺省值", LVCFMT_LEFT, 90);

	m_list_outputs.InsertColumn(0, "名称", LVCFMT_RIGHT, 100);
	m_list_outputs.InsertColumn(1, "类型", LVCFMT_CENTER, 60);
	m_list_outputs.InsertColumn(2, "缺省值", LVCFMT_LEFT, 90);

	UpdateInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CComInfoDialog::UpdateInfo()
{
	//获取组件名称
	m_ComponentName = m_pComponent->name;

	//清除原来的输入输出参数列表
	m_list_inputs.DeleteAllItems();
	m_list_outputs.DeleteAllItems();

	//获取组件输入参数
	for ( unsigned int i = 0; i < m_pComponent->inputs.size(); i++ )
	{
		int nItem;
		nItem = m_list_inputs.InsertItem(i, m_pComponent->inputs[i].name);
		CString strTmp;
		switch (m_pComponent->inputs[i].type)
		{
		case (MISE_DOUBLE):
			strTmp = "double";
			break;
		default:
			strTmp = "unknown";
			break;
		}
		m_list_inputs.SetItemText(nItem, 1, strTmp);

		strTmp.Format("%lf", m_pComponent->inputs[i].default_value.l);
		m_list_inputs.SetItemText(nItem, 2, strTmp);
	}

	//获取组件输出参数
	for ( unsigned int i = 0; i < m_pComponent->outputs.size(); i++ )
	{
		int nItem;
		nItem = m_list_outputs.InsertItem(i, m_pComponent->outputs[i].name);
		CString strTmp;
		switch (m_pComponent->outputs[i].type)
		{
		case (MISE_DOUBLE):
			strTmp = "double";
			break;
		default:
			strTmp = "unknown";
			break;
		}
		m_list_outputs.SetItemText(nItem, 1, strTmp);
		m_list_outputs.SetItemText(nItem, 2, "N/A");
	}
	//更新自身界面
	UpdateData(FALSE);
	//更新建模桌面
	g_pApp->m_pMdlPanel->Invalidate();
	
}

// 添加一个输入变量
void CComInfoDialog::OnBnClickedBtnAddinput()
{
	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "添加输入变量";
	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("请输入变量名称");
			return;
		}
		int type;
		if ( dlgVarInfo.m_varType == "double" )
		{
			type = MISE_DOUBLE;
		}
		else
		{
			type = MISE_UNKNOWN;
		}

		IOValue default_value;
		default_value.l = dlgVarInfo.m_default_value;
		m_pComponent->AddInput(dlgVarInfo.m_varName, type, default_value);
		UpdateInfo();
	}
}

// 添加一个输出变量
void CComInfoDialog::OnBnClickedBtnAddoutput()
{
	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "添加输出变量";
	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("请输入变量名称");
			return;
		}
		int type;
		if ( dlgVarInfo.m_varType == "double" )
		{
			type = MISE_DOUBLE;
		}
		else
		{
			type = MISE_UNKNOWN;
		}

		m_pComponent->AddOutput(dlgVarInfo.m_varName, type);
		UpdateInfo();
	}
}

//删除输入变量
void CComInfoDialog::OnBnClickedBtnInput()
{
	UINT i, uSelectedCount = m_list_inputs.GetSelectedCount();
	int  nItem = -1;

	// Update all of the selected items.
	if (uSelectedCount > 0)
	{
		for (i = 0; i < uSelectedCount; i++)
		{
			nItem = m_list_inputs.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			CString strTmp;
			strTmp = "确实要删除输入变量：";
			strTmp += m_list_inputs.GetItemText(nItem, 0);
			strTmp += "？";
			if(AfxMessageBox(strTmp, MB_OKCANCEL) == IDOK)	//let the user affirm
			{
				m_pComponent->DelInput(nItem);
				UpdateInfo();
			}
		}
	}

}

//删除输出变量
void CComInfoDialog::OnBnClickedBtnDeloutput()
{
	UINT i, uSelectedCount = m_list_outputs.GetSelectedCount();
	int  nItem = -1;

	// Update all of the selected items.
	if (uSelectedCount > 0)
	{
		for (i = 0; i < uSelectedCount; i++)
		{
			nItem = m_list_outputs.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			CString strTmp;
			strTmp = "确实要删除输出变量：";
			strTmp += m_list_outputs.GetItemText(nItem, 0);
			strTmp += "？";
			if(AfxMessageBox(strTmp, MB_OKCANCEL) == IDOK)	//let the user affirm
			{
				m_pComponent->DelOutput(nItem);
				UpdateInfo();
			}
		}
	}
}

//修改输入变量
void CComInfoDialog::OnBnClickedBtnAltinput()
{
	UINT uSelectedCount = m_list_inputs.GetSelectedCount();
	int  nItem = -1;
	if ( uSelectedCount != 1 )
	{
		AfxMessageBox("错误，请首先选中一个待修改的变量");
		return;
	}
	
	nItem = m_list_inputs.GetNextItem(nItem, LVNI_SELECTED);
	ASSERT(nItem != -1);

	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "修改输入变量";
	dlgVarInfo.m_varName = m_list_inputs.GetItemText(nItem, 0);
	dlgVarInfo.m_varType = m_list_inputs.GetItemText(nItem, 1);

	CString strTmp = m_list_inputs.GetItemText(nItem, 2);
	char *buffer, *stopstring;
	buffer = strTmp.GetBuffer(0);
	dlgVarInfo.m_default_value = strtod(buffer, &stopstring);

	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("请输入变量名称");
			return;
		}
		int type;
		if ( dlgVarInfo.m_varType == "double" )
		{
			type = MISE_DOUBLE;
		}
		else
		{
			type = MISE_UNKNOWN;
		}

		IOValue default_value;
		default_value.l = dlgVarInfo.m_default_value;
		m_pComponent->AltInput(nItem, dlgVarInfo.m_varName, type, default_value);
		UpdateInfo();
	}
}

//修改输出变量
void CComInfoDialog::OnBnClickedBtnAltoutput()
{
	UINT uSelectedCount = m_list_outputs.GetSelectedCount();
	int  nItem = -1;
	if ( uSelectedCount != 1 )
	{
		AfxMessageBox("错误，请首先选中一个待修改的变量");
		return;
	}
	
	nItem = m_list_outputs.GetNextItem(nItem, LVNI_SELECTED);
	ASSERT(nItem != -1);

	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "修改输入变量";
	dlgVarInfo.m_varName = m_list_outputs.GetItemText(nItem, 0);
	dlgVarInfo.m_varType = m_list_outputs.GetItemText(nItem, 1);

	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("请输入变量名称");
			return;
		}
		int type;
		if ( dlgVarInfo.m_varType == "double" )
		{
			type = MISE_DOUBLE;
		}
		else
		{
			type = MISE_UNKNOWN;
		}

		m_pComponent->AltOutput(nItem, dlgVarInfo.m_varName, type);
		UpdateInfo();
	}
}
