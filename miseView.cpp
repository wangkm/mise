// miseView.cpp : CmiseView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CmiseView ����/����

CmiseView::CmiseView()
{
	// TODO: �ڴ˴���ӹ������

}

CmiseView::~CmiseView()
{
}

BOOL CmiseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CmiseView ����

void CmiseView::OnDraw(CDC* /*pDC*/)
{
	CmiseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CmiseView ��ӡ

BOOL CmiseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CmiseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CmiseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CmiseView ���

#ifdef _DEBUG
void CmiseView::AssertValid() const
{
	CView::AssertValid();
}

void CmiseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmiseDoc* CmiseView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmiseDoc)));
	return (CmiseDoc*)m_pDocument;
}
#endif //_DEBUG


// CmiseView ��Ϣ�������
