// miseView.cpp : CmiseView 类的实现
//

#include "stdafx.h"
#include "mise.h"

#include "miseDoc.h"
#include "miseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmiseView

IMPLEMENT_DYNCREATE(CmiseView, CView)

BEGIN_MESSAGE_MAP(CmiseView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CmiseView 构造/析构

CmiseView::CmiseView()
{
	// TODO: 在此处添加构造代码

}

CmiseView::~CmiseView()
{
}

BOOL CmiseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CmiseView 绘制

void CmiseView::OnDraw(CDC* /*pDC*/)
{
	CmiseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CmiseView 打印

BOOL CmiseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CmiseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CmiseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CmiseView 诊断

#ifdef _DEBUG
void CmiseView::AssertValid() const
{
	CView::AssertValid();
}

void CmiseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmiseDoc* CmiseView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmiseDoc)));
	return (CmiseDoc*)m_pDocument;
}
#endif //_DEBUG


// CmiseView 消息处理程序
