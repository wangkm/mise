// ComTree.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "ComTree.h"
#include "MyDataDef.h"
#include ".\comtree.h"
#include "MiseLibs.h"
#include "shlwapi.h"

#import <msxml3.dll>
using namespace MSXML2;

// CComTree

IMPLEMENT_DYNCREATE(CComTree, CTreeView)

CComTree::CComTree()
{
	g_pApp->m_pComTree = this;
	m_bDragging = FALSE;
}

CComTree::~CComTree()
{
}

BEGIN_MESSAGE_MAP(CComTree, CTreeView)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CComTree::OnTvnSelchanged)
END_MESSAGE_MAP()


// CComTree diagnostics

#ifdef _DEBUG
void CComTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CComTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

// ���½�ģ�����
BOOL CComTree::UpdateComponentTree(void)
{
	//�õ�modules��Ϣ�ļ���·��
	CString xmlFileName;
	xmlFileName = g_pApp->m_appPath;
	xmlFileName += "\\components\\";
	xmlFileName += COMPONENT_CFG_FILE;
	
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr;

	hr = pXMLDom.CreateInstance(__uuidof(DOMDocument30));
	if (FAILED(hr)) 
	{
		AfxMessageBox("��ʼ��DOMDocument30��ʧ��\n");
		return FALSE;
	}

	pXMLDom->async = VARIANT_FALSE;
	if ( pXMLDom->load(xmlFileName.GetBuffer(0)) != VARIANT_TRUE)
	{
		CString strTmp = "��ȡģ������ģ���ļ�ʧ��\n��ȷ����componentsĿ¼�´���components.xml�ļ�";
		strTmp += (LPCSTR)pXMLDom->parseError->Getreason();
		AfxMessageBox(strTmp);
		return FALSE;
	}

	//=========================== ����component ======================================
	CString strQryLib;
	MSXML2::IXMLDOMNodePtr pNodeLib;
	HTREEITEM hLibItem;	//component lib

	UINT i = 0;
	int counter = 0;	//���ڶԴ�����component�ڵ���м���
	strQryLib.Format("//componentLib[%d]", i++);
	pNodeLib = pXMLDom->selectSingleNode(strQryLib.GetBuffer(0));
	while ( pNodeLib != NULL )
	{
		//��ȡ���������
		CString libCaption = ((MSXML2::IXMLDOMElementPtr)pNodeLib)->getAttribute("caption");

		//�����д����ڵ�
		hLibItem = GetTreeCtrl().InsertItem(libCaption, 0, 0, TVI_ROOT, TVI_LAST);

		//����ӽڵ�
		UINT j = 0;
		CString strQryComponent;
		MSXML2::IXMLDOMNodePtr pNodeComponent;

		strQryComponent.Format("%s/component[%d]", strQryLib, j++);
		pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));
		while ( pNodeComponent != NULL )
		{
			//��ȡ�ڵ�����
			CString componentCaption = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("caption");

			//��ȡ�ڵ��������Ϣ
			comInfo[counter].name = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("name");;
			comInfo[counter].type = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("type");

			//dllPathǰ����ϵͳ·��
			CString dllPathTmp = g_pApp->m_appPath;
			dllPathTmp += "\\";
			dllPathTmp += ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("dllPath");
			comInfo[counter].dllPath = dllPathTmp;
			comInfo[counter].funcName = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("funcName");
			if ( comInfo[counter].type == UDF_COMPONENT )
			{
				comInfo[counter].width = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("width");
				comInfo[counter].height = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("height");
				comInfo[counter].inputs_min = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("inputs_min");
				comInfo[counter].inputs_max = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("inputs_max");
				comInfo[counter].outputs_min = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("outputs_min");
				comInfo[counter].outputs_max = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("outputs_max");
				comInfo[counter].display_port = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("display_port");
			}

			CString strQryIO;
			strQryIO = strQryComponent + "/" + "inputs";
			MSXML2::IXMLDOMNodePtr pNodeInputs = pXMLDom->selectSingleNode(strQryIO.GetBuffer(0));
			comInfo[counter].inputs = ((MSXML2::IXMLDOMElementPtr)pNodeInputs)->text.GetBSTR();

			strQryIO = strQryComponent + "/" + "outputs";
			MSXML2::IXMLDOMNodePtr pNodeOutputs = pXMLDom->selectSingleNode(strQryIO.GetBuffer(0));
			comInfo[counter].outputs = ((MSXML2::IXMLDOMElementPtr)pNodeOutputs)->text.GetBSTR();



			//�����д����ڵ�
			HTREEITEM hComponentItem;
			hComponentItem = GetTreeCtrl().InsertItem(componentCaption, 1, 1, hLibItem, TVI_LAST);
			GetTreeCtrl().SetItemData(hComponentItem, (DWORD)(&(comInfo[counter++])));
			

			//������ѯ
			strQryComponent.Format("%s/component[%d]", strQryLib, j++);
			pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));
		}

		//������ѯ
		strQryLib.Format("//componentLib[%d]", i++);
		pNodeLib = pXMLDom->selectSingleNode(strQryLib.GetBuffer(0));
	}
	//============================ end of ���� component ===========================================

	//��ѯ��ϣ��ͷ���Դ
	pXMLDom.Release();

	return TRUE;

}


void CComTree::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_pImageList = new CImageList();

	// 16x16 pixels, only 1 currently
	m_pImageList->Create(16, 16, ILC_COLOR16, 2, 2);

	m_pImageList->Add(AfxGetApp()->LoadIcon(IDR_ICON_COMLIB));
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDR_ICON_COMP));
	this->GetTreeCtrl().SetImageList( m_pImageList, TVSIL_NORMAL );

	UpdateComponentTree();
}

BOOL CComTree::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;	

	return CWnd::Create(lpszClassName, lpszWindowName, 
					dwStyle, rect, pParentWnd, nID, pContext);
}

int GetItemLevel(CTreeCtrl *pTree, HTREEITEM theItem)
{
	int level;
	HTREEITEM itemTmp;
	itemTmp = pTree->GetParentItem(theItem);

	for ( level = 0; itemTmp != NULL; level++)
	{
		itemTmp = pTree->GetParentItem(itemTmp);
	}

	return level;
}
void CComTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM theItem = pNMTreeView->itemNew.hItem;
	
	//����϶����Ǹ��ڵ㣬�����в���
	if ( GetItemLevel(&GetTreeCtrl(), theItem) == 0 )
	{
		return;
	}

	//�趨�϶��ڵ�Ϊѡ��״̬
	GetTreeCtrl().SelectItem(theItem);
	
	//�趨m_pCurrentComInfoָ���϶��ڵ�ĸ�����Ϣ�ṹ
	m_pCurrentComInfo = (ComponentInfo*)(GetTreeCtrl().GetItemData(theItem));	//�趨ָ��ѡ�нڵ���Ϣ��ָ��

	m_pDragImage = GetTreeCtrl().CreateDragImage( theItem );
	if( !m_pDragImage )
		return;

	m_bDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(8,8) );
	CPoint pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(g_pApp->m_pMainWnd, pt);
	SetCapture();

}

void CComTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bDragging)
	{
		m_bDragging = FALSE;
		m_pDragImage->DragLeave(g_pApp->m_pMainWnd);
		m_pDragImage->EndDrag();
		ReleaseCapture();
		delete m_pDragImage;

		//����Ƿ��϶�����ģ������
		POINT   pt = point;
		CRect	rectMdlPanel;
		g_pApp->m_pMdlPanel->GetWindowRect(&rectMdlPanel);
		ClientToScreen(&pt);

		//����϶�����ģ�����ڣ��򴴽����
		if (rectMdlPanel.PtInRect(pt))
		{
			g_pApp->m_pMdlPanel->ScreenToClient(&pt);
			
			//�����������ݲ�ͬ�����ʹ�����ͬ�Ķ���

			MiseComponent *pGenericComponent = NULL;
			switch( m_pCurrentComInfo->type )
			{

				case(SRC_CLOCK):	//sin Դ
				{
					//�������
					Mise_source_clock *pSourceClock = new Mise_source_clock();
					//���ϴ���ָ��
					pGenericComponent = pSourceClock;
					break;
				}

				case(SRC_SIN):	//sin Դ
				{
					//�������
					Mise_source_sin *pSourceSin = new Mise_source_sin();
					//���ϴ���ָ��
					pGenericComponent = pSourceSin;
					break;
				}
				
				case(SRC_COS):	//cos Դ
				{
					//�������
					Mise_source_cos *pSourceCos = new Mise_source_cos();
					//���ϴ���ָ��
					pGenericComponent = pSourceCos;
					break;
				}
				case(SRC_CONST):	//const Դ
				{
					//�������
					Mise_source_const *pSourceConst = new Mise_source_const();
					//���ϴ���ָ��
					pGenericComponent = pSourceConst;
					break;
				}

				case(SRC_STEP):		//step Դ
				{
					//�������
					Mise_source_step *pSourceStep = new Mise_source_step();
					//���ϴ���ָ��
					pGenericComponent = pSourceStep;
					break;
				}

				case(MATH_ADD):	//�ӷ���
				{
					//�������
					Mise_math_add *pMathAdd = new Mise_math_add();
					//���ϴ���ָ��
					pGenericComponent = pMathAdd;
					break;
				}

				case(MATH_MUL):	//�˷���
				{
					//�������
					Mise_math_multiply *pMathMul = new Mise_math_multiply();
					//���ϴ���ָ��
					pGenericComponent = pMathMul;
					break;
				}

				case(MATH_RECIPROCAL):	//ȡ����
				{
					//�������
					Mise_math_reciprocal *pMathRec = new Mise_math_reciprocal();
					//���ϴ���ָ��
					pGenericComponent = pMathRec;
					break;
				}

				case(MATH_POW):			//������
				{
					//�������
					Mise_math_pow *pMathPow = new Mise_math_pow();
					//���ϴ���ָ��
					pGenericComponent = pMathPow;
					break;
				}
				case(CONT_DIFF):	//ȡ΢��
				{
					//�������
					Mise_cont_differential *pContDiff = new Mise_cont_differential();
					//���ϴ���ָ��
					pGenericComponent = pContDiff;
					break;
				}

				case(CONT_INTEGRAL):	//ȡ����
				{
					//�������
					Mise_cont_integral *pContIntegral = new Mise_cont_integral();
					//���ϴ���ָ��
					pGenericComponent = pContIntegral;
					break;
				}

				case(CONT_TRANSFUN):	//һ�㴫�ݺ���
				{
					//�������
					Mise_cont_transfunction *pContTransFun = new Mise_cont_transfunction();
					//���ϴ���ָ��
					pGenericComponent = pContTransFun;
					break;
				}

				case(CONT_ZEROPOLE):	//�㣭�������
				{
					//�������
					Mise_cont_ZeroPole *pContZeroPole = new Mise_cont_ZeroPole();
					//���ϴ���ָ��
					pGenericComponent = pContZeroPole;
					break;
				}

				case(OUT_FILER):	//������ļ�
				{
					//�������
					Mise_output_filer *pOutputFiler = new Mise_output_filer();
					//���ϴ���ָ��
					pGenericComponent = pOutputFiler;
					break;
				}
				
				case(OUT_MONITOR):	//������Ϊ����
				{
					//�������
					Mise_output_monitor *pOutputMonitor = new Mise_output_monitor();
					//���ϴ���ָ��
					pGenericComponent = pOutputMonitor;
					break;
				}

				default:
				{
					//�������
					MiseComponent *pUDFComponent = new MiseComponent();
					//���ϴ���ָ��
					pGenericComponent = pUDFComponent;
					break;
				}
			}

					
			//�趨�����Ϣ
			pGenericComponent->name = m_pCurrentComInfo->name;
			pGenericComponent->type	= m_pCurrentComInfo->type;
			if ( pGenericComponent->type == UDF_COMPONENT )
			{
				pGenericComponent->m_width = m_pCurrentComInfo->width;
				pGenericComponent->m_height = m_pCurrentComInfo->height;
				pGenericComponent->m_inputs_min = m_pCurrentComInfo->inputs_min;
				pGenericComponent->m_inputs_max = m_pCurrentComInfo->inputs_max;
				pGenericComponent->m_outputs_min = m_pCurrentComInfo->outputs_min;
				pGenericComponent->m_outputs_max = m_pCurrentComInfo->outputs_max;
				pGenericComponent->dllPath = m_pCurrentComInfo->dllPath;
				pGenericComponent->functionName = m_pCurrentComInfo->funcName;
				pGenericComponent->m_displayPortName = m_pCurrentComInfo->display_port;
			}
			pGenericComponent->rect.left = pt.x;
			pGenericComponent->rect.top = pt.y;
			pGenericComponent->rect.right = pt.x + pGenericComponent->m_width;
			pGenericComponent->rect.bottom = pt.y + pGenericComponent->m_height;

			CString strIOList;
			//�������ӿ�
			strIOList = m_pCurrentComInfo->inputs.Trim();
			if ( strIOList != "" )
			{
				IOValue default_value;
				int index = strIOList.Find("#");
				while ( index != -1 )
				{
					CString strTmp = strIOList.Left(index);
					//���������˿���Ϣ
					CString strPortName;
					int index2 = strTmp.Find(':');
					int index3 = strTmp.ReverseFind(':');
					//�˿�����
					strPortName = strTmp.Left(index2);
					//�˿�����
					int type = StrToInt(strTmp.Mid(index2 + 1, index3));
					//�˿�ȱʡֵ
					char *buffer, *stopstring;
					CString strTmp2 = strTmp.Right(strTmp.GetLength() - index3 - 1);
					buffer = strTmp2.GetBuffer(0);
					double def_value = strtod(buffer, &stopstring);
					default_value.l = def_value;

					//�������˿�
					pGenericComponent->AddInput(strPortName, type, default_value);
					
					//�޸Ķ˿���Ϣ�ַ�������������
					strIOList = strIOList.Right(strIOList.GetLength() - index - 1);
					index = strIOList.Find("#");
				}
				//�������һ������
				CString strTmp = strIOList;
				//���������˿���Ϣ
				CString strPortName;
				int index2 = strTmp.Find(':');
				int index3 = strTmp.ReverseFind(':');
				//�˿�����
				strPortName = strTmp.Left(index2);
				//�˿�����
				int type = StrToInt(strTmp.Mid(index2 + 1, index3));
				//�˿�ȱʡֵ
				char *buffer, *stopstring;
				CString strTmp2 = strTmp.Right(strTmp.GetLength() - index3 - 1);
				buffer = strTmp2.GetBuffer(0);
				double def_value = strtod(buffer, &stopstring);
				default_value.l = def_value;

				//�������˿�
				pGenericComponent->AddInput(strPortName, type, default_value);
			}


			//�������ӿ�
			strIOList = m_pCurrentComInfo->outputs.Trim();
			if ( strIOList != "" )
			{
				int index = strIOList.Find("#");
				while ( index != -1 )
				{
					CString strTmp = strIOList.Left(index);
					//���������˿���Ϣ
					CString strPortName;
					int index2 = strTmp.Find(':');
					//�˿�����
					strPortName = strTmp.Left(index2);
					//�˿�����
					int type = StrToInt(strTmp.Right(strTmp.GetLength() - index2 - 1));

					//�������˿�
					pGenericComponent->AddOutput(strPortName, type);
					
					//�޸Ķ˿���Ϣ�ַ�������������
					strIOList = strIOList.Right(strIOList.GetLength() - index - 1);
					index = strIOList.Find("#");
				}
				//�������һ������
				CString strTmp = strIOList;
				//���������˿���Ϣ
				CString strPortName;
				int index2 = strTmp.Find(':');
				//�˿�����
				strPortName = strTmp.Left(index2);
				//�˿�����
				int type = StrToInt(strTmp.Right(strTmp.GetLength() - index2 - 1));

				//�������˿�
				pGenericComponent->AddOutput(strPortName, type);
			}


			//��ӵ�����б���
			g_pApp->m_miseModel.AppendComponent(pGenericComponent);

			g_pApp->m_pMdlPanel->Invalidate();
		}

	}
	CTreeView::OnLButtonUp(nFlags, point);
}

void CComTree::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( m_bDragging )
	{
		POINT   pt = point;
	
		CRect rectMainWnd, rectComTree;
		g_pApp->m_pMainWnd->GetWindowRect(&rectMainWnd);
		this->GetWindowRect(&rectComTree);
		pt.x -= (rectMainWnd.left - rectComTree.left);
		pt.y -= (rectMainWnd.top - rectComTree.top);
		
		m_pDragImage->DragMove(pt);
		m_pDragImage->DragShowNolock( TRUE );
	}

	CTreeView::OnMouseMove(nFlags, point);
}

void CComTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CTreeView::OnLButtonDown(nFlags, point);
}

void CComTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM theItem = pNMTreeView->itemNew.hItem;
	
	//����϶����Ǹ��ڵ㣬��չ����ر�
	if ( GetItemLevel(&GetTreeCtrl(), theItem) == 0 )
	{
		GetTreeCtrl().Expand(theItem, TVE_EXPAND);
	}
}

