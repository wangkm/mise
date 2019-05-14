#pragma once
#include "MyDataDef.h"

//�洢�ڵ���Ϣ�Ľṹ
typedef struct
{
	CString	name;
	int		type;
	CString	dllPath;
	CString funcName;
	CString inputs;
	CString outputs;
	int		width;
	int		height;
	int		inputs_min;
	int		inputs_max;
	int		outputs_min;
	int		outputs_max;
	bool	display_port;
}ComponentInfo;

// CComTree view

class CComTree : public CTreeView
{
	DECLARE_DYNCREATE(CComTree)

protected:
	CComTree();           // protected constructor used by dynamic creation
	virtual ~CComTree();

protected:
	CImageList*	m_pImageList;	//�ڵ�ͼ���б�
	CImageList*	m_pDragImage;	//�϶�ʱ��ʾ��ͼ���б�
	BOOL		m_bDragging;
	DWORD		m_dwDragStart;	//������������ʱ��
	CPoint		m_HoverPoint;	//���λ��
	UINT		m_nHoverTimerID;//������ж�ʱ��

	ComponentInfo comInfo[COM_TYPE_MAX_SIZE];	//���ڴ洢�����������Ϣ
	ComponentInfo *m_pCurrentComInfo;				//ָ��ǰѡȡ�������Ϣ


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	// ���½�ģ�����
	BOOL UpdateComponentTree(void);
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
//	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


