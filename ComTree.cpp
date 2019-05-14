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

// 更新建模组件树
BOOL CComTree::UpdateComponentTree(void)
{
	//得到modules信息文件的路径
	CString xmlFileName;
	xmlFileName = g_pApp->m_appPath;
	xmlFileName += "\\components\\";
	xmlFileName += COMPONENT_CFG_FILE;
	
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr;

	hr = pXMLDom.CreateInstance(__uuidof(DOMDocument30));
	if (FAILED(hr)) 
	{
		AfxMessageBox("初始化DOMDocument30类失败\n");
		return FALSE;
	}

	pXMLDom->async = VARIANT_FALSE;
	if ( pXMLDom->load(xmlFileName.GetBuffer(0)) != VARIANT_TRUE)
	{
		CString strTmp = "读取模型配置模型文件失败\n请确定在components目录下存在components.xml文件";
		strTmp += (LPCSTR)pXMLDom->parseError->Getreason();
		AfxMessageBox(strTmp);
		return FALSE;
	}

	//=========================== 加载component ======================================
	CString strQryLib;
	MSXML2::IXMLDOMNodePtr pNodeLib;
	HTREEITEM hLibItem;	//component lib

	UINT i = 0;
	int counter = 0;	//用于对创建的component节点进行计数
	strQryLib.Format("//componentLib[%d]", i++);
	pNodeLib = pXMLDom->selectSingleNode(strQryLib.GetBuffer(0));
	while ( pNodeLib != NULL )
	{
		//获取组件库名称
		CString libCaption = ((MSXML2::IXMLDOMElementPtr)pNodeLib)->getAttribute("caption");

		//在树中创建节点
		hLibItem = GetTreeCtrl().InsertItem(libCaption, 0, 0, TVI_ROOT, TVI_LAST);

		//添加子节点
		UINT j = 0;
		CString strQryComponent;
		MSXML2::IXMLDOMNodePtr pNodeComponent;

		strQryComponent.Format("%s/component[%d]", strQryLib, j++);
		pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));
		while ( pNodeComponent != NULL )
		{
			//获取节点名称
			CString componentCaption = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("caption");

			//获取节点的其他信息
			comInfo[counter].name = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("name");;
			comInfo[counter].type = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("type");

			//dllPath前加上系统路径
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



			//在树中创建节点
			HTREEITEM hComponentItem;
			hComponentItem = GetTreeCtrl().InsertItem(componentCaption, 1, 1, hLibItem, TVI_LAST);
			GetTreeCtrl().SetItemData(hComponentItem, (DWORD)(&(comInfo[counter++])));
			

			//继续查询
			strQryComponent.Format("%s/component[%d]", strQryLib, j++);
			pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));
		}

		//继续查询
		strQryLib.Format("//componentLib[%d]", i++);
		pNodeLib = pXMLDom->selectSingleNode(strQryLib.GetBuffer(0));
	}
	//============================ end of 加载 component ===========================================

	//查询完毕，释放资源
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
	
	//如果拖动的是根节点，不进行操作
	if ( GetItemLevel(&GetTreeCtrl(), theItem) == 0 )
	{
		return;
	}

	//设定拖动节点为选中状态
	GetTreeCtrl().SelectItem(theItem);
	
	//设定m_pCurrentComInfo指向拖动节点的附加信息结构
	m_pCurrentComInfo = (ComponentInfo*)(GetTreeCtrl().GetItemData(theItem));	//设定指向选中节点信息的指针

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

		//检测是否拖动到建模窗口内
		POINT   pt = point;
		CRect	rectMdlPanel;
		g_pApp->m_pMdlPanel->GetWindowRect(&rectMdlPanel);
		ClientToScreen(&pt);

		//如果拖动到建模窗口内，则创建组件
		if (rectMdlPanel.PtInRect(pt))
		{
			g_pApp->m_pMdlPanel->ScreenToClient(&pt);
			
			//添加组件，根据不同的类型创建不同的对象

			MiseComponent *pGenericComponent = NULL;
			switch( m_pCurrentComInfo->type )
			{

				case(SRC_CLOCK):	//sin 源
				{
					//创建组件
					Mise_source_clock *pSourceClock = new Mise_source_clock();
					//向上传递指针
					pGenericComponent = pSourceClock;
					break;
				}

				case(SRC_SIN):	//sin 源
				{
					//创建组件
					Mise_source_sin *pSourceSin = new Mise_source_sin();
					//向上传递指针
					pGenericComponent = pSourceSin;
					break;
				}
				
				case(SRC_COS):	//cos 源
				{
					//创建组件
					Mise_source_cos *pSourceCos = new Mise_source_cos();
					//向上传递指针
					pGenericComponent = pSourceCos;
					break;
				}
				case(SRC_CONST):	//const 源
				{
					//创建组件
					Mise_source_const *pSourceConst = new Mise_source_const();
					//向上传递指针
					pGenericComponent = pSourceConst;
					break;
				}

				case(SRC_STEP):		//step 源
				{
					//创建组件
					Mise_source_step *pSourceStep = new Mise_source_step();
					//向上传递指针
					pGenericComponent = pSourceStep;
					break;
				}

				case(MATH_ADD):	//加法器
				{
					//创建组件
					Mise_math_add *pMathAdd = new Mise_math_add();
					//向上传递指针
					pGenericComponent = pMathAdd;
					break;
				}

				case(MATH_MUL):	//乘法器
				{
					//创建组件
					Mise_math_multiply *pMathMul = new Mise_math_multiply();
					//向上传递指针
					pGenericComponent = pMathMul;
					break;
				}

				case(MATH_RECIPROCAL):	//取倒数
				{
					//创建组件
					Mise_math_reciprocal *pMathRec = new Mise_math_reciprocal();
					//向上传递指针
					pGenericComponent = pMathRec;
					break;
				}

				case(MATH_POW):			//幂运算
				{
					//创建组件
					Mise_math_pow *pMathPow = new Mise_math_pow();
					//向上传递指针
					pGenericComponent = pMathPow;
					break;
				}
				case(CONT_DIFF):	//取微分
				{
					//创建组件
					Mise_cont_differential *pContDiff = new Mise_cont_differential();
					//向上传递指针
					pGenericComponent = pContDiff;
					break;
				}

				case(CONT_INTEGRAL):	//取积分
				{
					//创建组件
					Mise_cont_integral *pContIntegral = new Mise_cont_integral();
					//向上传递指针
					pGenericComponent = pContIntegral;
					break;
				}

				case(CONT_TRANSFUN):	//一般传递函数
				{
					//创建组件
					Mise_cont_transfunction *pContTransFun = new Mise_cont_transfunction();
					//向上传递指针
					pGenericComponent = pContTransFun;
					break;
				}

				case(CONT_ZEROPOLE):	//零－极点分析
				{
					//创建组件
					Mise_cont_ZeroPole *pContZeroPole = new Mise_cont_ZeroPole();
					//向上传递指针
					pGenericComponent = pContZeroPole;
					break;
				}

				case(OUT_FILER):	//输出到文件
				{
					//创建组件
					Mise_output_filer *pOutputFiler = new Mise_output_filer();
					//向上传递指针
					pGenericComponent = pOutputFiler;
					break;
				}
				
				case(OUT_MONITOR):	//结果输出为曲线
				{
					//创建组件
					Mise_output_monitor *pOutputMonitor = new Mise_output_monitor();
					//向上传递指针
					pGenericComponent = pOutputMonitor;
					break;
				}

				default:
				{
					//创建组件
					MiseComponent *pUDFComponent = new MiseComponent();
					//向上传递指针
					pGenericComponent = pUDFComponent;
					break;
				}
			}

					
			//设定相关信息
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
			//添加输入接口
			strIOList = m_pCurrentComInfo->inputs.Trim();
			if ( strIOList != "" )
			{
				IOValue default_value;
				int index = strIOList.Find("#");
				while ( index != -1 )
				{
					CString strTmp = strIOList.Left(index);
					//继续解析端口信息
					CString strPortName;
					int index2 = strTmp.Find(':');
					int index3 = strTmp.ReverseFind(':');
					//端口名称
					strPortName = strTmp.Left(index2);
					//端口类型
					int type = StrToInt(strTmp.Mid(index2 + 1, index3));
					//端口缺省值
					char *buffer, *stopstring;
					CString strTmp2 = strTmp.Right(strTmp.GetLength() - index3 - 1);
					buffer = strTmp2.GetBuffer(0);
					double def_value = strtod(buffer, &stopstring);
					default_value.l = def_value;

					//添加输入端口
					pGenericComponent->AddInput(strPortName, type, default_value);
					
					//修改端口信息字符串，继续查找
					strIOList = strIOList.Right(strIOList.GetLength() - index - 1);
					index = strIOList.Find("#");
				}
				//加上最后一个变量
				CString strTmp = strIOList;
				//继续解析端口信息
				CString strPortName;
				int index2 = strTmp.Find(':');
				int index3 = strTmp.ReverseFind(':');
				//端口名称
				strPortName = strTmp.Left(index2);
				//端口类型
				int type = StrToInt(strTmp.Mid(index2 + 1, index3));
				//端口缺省值
				char *buffer, *stopstring;
				CString strTmp2 = strTmp.Right(strTmp.GetLength() - index3 - 1);
				buffer = strTmp2.GetBuffer(0);
				double def_value = strtod(buffer, &stopstring);
				default_value.l = def_value;

				//添加输入端口
				pGenericComponent->AddInput(strPortName, type, default_value);
			}


			//添加输出接口
			strIOList = m_pCurrentComInfo->outputs.Trim();
			if ( strIOList != "" )
			{
				int index = strIOList.Find("#");
				while ( index != -1 )
				{
					CString strTmp = strIOList.Left(index);
					//继续解析端口信息
					CString strPortName;
					int index2 = strTmp.Find(':');
					//端口名称
					strPortName = strTmp.Left(index2);
					//端口类型
					int type = StrToInt(strTmp.Right(strTmp.GetLength() - index2 - 1));

					//添加输入端口
					pGenericComponent->AddOutput(strPortName, type);
					
					//修改端口信息字符串，继续查找
					strIOList = strIOList.Right(strIOList.GetLength() - index - 1);
					index = strIOList.Find("#");
				}
				//加上最后一个变量
				CString strTmp = strIOList;
				//继续解析端口信息
				CString strPortName;
				int index2 = strTmp.Find(':');
				//端口名称
				strPortName = strTmp.Left(index2);
				//端口类型
				int type = StrToInt(strTmp.Right(strTmp.GetLength() - index2 - 1));

				//添加输入端口
				pGenericComponent->AddOutput(strPortName, type);
			}


			//添加到组件列表中
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
	
	//如果拖动的是根节点，则展开或关闭
	if ( GetItemLevel(&GetTreeCtrl(), theItem) == 0 )
	{
		GetTreeCtrl().Expand(theItem, TVE_EXPAND);
	}
}

