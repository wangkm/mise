#pragma once
#include "Model.h"

// CMdlPanel view

class CMdlPanel : public CView
{
	DECLARE_DYNCREATE(CMdlPanel)

protected:
	CMdlPanel();           // protected constructor used by dynamic creation
	virtual ~CMdlPanel();

protected:
	CPoint	m_oldPoint;		//记录上一次鼠标的位置，用来移动
	int		m_action;		//标识当前操作的内容

	CPoint	m_startPoint;	//标识连线时的起点
	CPoint	m_endPoint;		//标识连线的终点
	CString m_outportName;	//记录连线时输出点的名称

	LOGFONT	m_mylf;			//显示IO端口名称用的字体

	MiseComponent*	m_selectedComponent;		//指向当前选中的组件
	ConnectLine*	m_selectedConnLine;			//指向当前选中的连线

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags,CPoint point );
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnPmCominfo();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMenuComdef();
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


