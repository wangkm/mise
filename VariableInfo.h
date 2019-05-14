#pragma once


// CVariableInfo dialog

class CVariableInfo : public CDialog
{
	DECLARE_DYNAMIC(CVariableInfo)

public:
	CVariableInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVariableInfo();

// Dialog Data
	enum { IDD = IDD_VARIABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString		m_title;			//title of the dialog
	CString		m_varName;			//name of variable
	double		m_default_value;	//default value of variable
	CString		m_varType;			//type of variable
public:
	virtual BOOL OnInitDialog();
};
