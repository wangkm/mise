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
	CPoint	m_oldPoint;		//��¼��һ������λ�ã������ƶ�
	int		m_action;		//��ʶ��ǰ����������

	CPoint	m_startPoint;	//��ʶ����ʱ�����
	CPoint	m_endPoint;		//��ʶ���ߵ��յ�
	CString m_outportName;	//��¼����ʱ����������

	LOGFONT	m_mylf;			//��ʾIO�˿������õ�����

	MiseComponent*	m_selectedComponent;		//ָ��ǰѡ�е����
	ConnectLine*	m_selectedConnLine;			//ָ��ǰѡ�е�����

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


