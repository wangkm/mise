// miseView.h : CmiseView ��Ľӿ�
//


#pragma once


class CmiseView : public CView
{
protected: // �������л�����
	CmiseView();
	DECLARE_DYNCREATE(CmiseView)

// ����
public:
	CmiseDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CmiseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // miseView.cpp �ĵ��԰汾
inline CmiseDoc* CmiseView::GetDocument() const
   { return reinterpret_cast<CmiseDoc*>(m_pDocument); }
#endif

