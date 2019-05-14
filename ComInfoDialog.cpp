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

	//�޸��б�����
	m_list_inputs.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_outputs.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_list_inputs.InsertColumn(0, "����", LVCFMT_RIGHT, 100);
	m_list_inputs.InsertColumn(1, "����", LVCFMT_CENTER, 60);
	m_list_inputs.InsertColumn(2, "ȱʡֵ", LVCFMT_LEFT, 90);

	m_list_outputs.InsertColumn(0, "����", LVCFMT_RIGHT, 100);
	m_list_outputs.InsertColumn(1, "����", LVCFMT_CENTER, 60);
	m_list_outputs.InsertColumn(2, "ȱʡֵ", LVCFMT_LEFT, 90);

	UpdateInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CComInfoDialog::UpdateInfo()
{
	//��ȡ�������
	m_ComponentName = m_pComponent->name;

	//���ԭ����������������б�
	m_list_inputs.DeleteAllItems();
	m_list_outputs.DeleteAllItems();

	//��ȡ����������
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

	//��ȡ����������
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
	//�����������
	UpdateData(FALSE);
	//���½�ģ����
	g_pApp->m_pMdlPanel->Invalidate();
	
}

// ���һ���������
void CComInfoDialog::OnBnClickedBtnAddinput()
{
	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "����������";
	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("�������������");
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

// ���һ���������
void CComInfoDialog::OnBnClickedBtnAddoutput()
{
	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "����������";
	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("�������������");
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

//ɾ���������
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
			strTmp = "ȷʵҪɾ�����������";
			strTmp += m_list_inputs.GetItemText(nItem, 0);
			strTmp += "��";
			if(AfxMessageBox(strTmp, MB_OKCANCEL) == IDOK)	//let the user affirm
			{
				m_pComponent->DelInput(nItem);
				UpdateInfo();
			}
		}
	}

}

//ɾ���������
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
			strTmp = "ȷʵҪɾ�����������";
			strTmp += m_list_outputs.GetItemText(nItem, 0);
			strTmp += "��";
			if(AfxMessageBox(strTmp, MB_OKCANCEL) == IDOK)	//let the user affirm
			{
				m_pComponent->DelOutput(nItem);
				UpdateInfo();
			}
		}
	}
}

//�޸��������
void CComInfoDialog::OnBnClickedBtnAltinput()
{
	UINT uSelectedCount = m_list_inputs.GetSelectedCount();
	int  nItem = -1;
	if ( uSelectedCount != 1 )
	{
		AfxMessageBox("����������ѡ��һ�����޸ĵı���");
		return;
	}
	
	nItem = m_list_inputs.GetNextItem(nItem, LVNI_SELECTED);
	ASSERT(nItem != -1);

	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "�޸��������";
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
			AfxMessageBox("�������������");
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

//�޸��������
void CComInfoDialog::OnBnClickedBtnAltoutput()
{
	UINT uSelectedCount = m_list_outputs.GetSelectedCount();
	int  nItem = -1;
	if ( uSelectedCount != 1 )
	{
		AfxMessageBox("����������ѡ��һ�����޸ĵı���");
		return;
	}
	
	nItem = m_list_outputs.GetNextItem(nItem, LVNI_SELECTED);
	ASSERT(nItem != -1);

	CVariableInfo dlgVarInfo;
	dlgVarInfo.m_title = "�޸��������";
	dlgVarInfo.m_varName = m_list_outputs.GetItemText(nItem, 0);
	dlgVarInfo.m_varType = m_list_outputs.GetItemText(nItem, 1);

	if ( dlgVarInfo.DoModal() == IDOK )
	{
		if ( dlgVarInfo.m_varName.Trim() == "")
		{
			AfxMessageBox("�������������");
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
