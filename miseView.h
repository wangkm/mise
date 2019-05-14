// miseView.h : CmiseView 类的接口
//


#pragma once


class CmiseView : public CView
{
protected: // 仅从序列化创建
	CmiseView();
	DECLARE_DYNCREATE(CmiseView)

// 属性
public:
	CmiseDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CmiseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // miseView.cpp 的调试版本
inline CmiseDoc* CmiseView::GetDocument() const
   { return reinterpret_cast<CmiseDoc*>(m_pDocument); }
#endif

