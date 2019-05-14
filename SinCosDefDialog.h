#pragma once
#include "Model.h"
#include "MiseLibs.h"
#include "MyDataDef.h"


// CSinCosDefDialog dialog

class CSinCosDefDialog : public CDialog
{
	DECLARE_DYNAMIC(CSinCosDefDialog)

public:
	CSinCosDefDialog(MiseComponent *pComponent, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSinCosDefDialog();

// Dialog Data
	enum { IDD = IDD_DEF_SINCOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double	m_A;
	double	m_B;
	double	m_C;
	int		m_type;
	virtual BOOL OnInitDialog();
};
