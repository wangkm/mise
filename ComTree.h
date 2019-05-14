#pragma once
#include "MyDataDef.h"

//存储节点信息的结构
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
	CImageList*	m_pImageList;	//节点图像列表
	CImageList*	m_pDragImage;	//拖动时显示的图象列表
	BOOL		m_bDragging;
	DWORD		m_dwDragStart;	//按下鼠标左键的时刻
	CPoint		m_HoverPoint;	//鼠标位置
	UINT		m_nHoverTimerID;//鼠标敏感定时器

	ComponentInfo comInfo[COM_TYPE_MAX_SIZE];	//用于存储组件的其他信息
	ComponentInfo *m_pCurrentComInfo;				//指向当前选取的组件信息


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 更新建模组件树
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


