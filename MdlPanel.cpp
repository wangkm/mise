// MdlPanel.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "MdlPanel.h"
#include "MyDataDef.h"
#include "ComInfoDialog.h"
#include <algorithm>
#include "MiseLibs.h"


// CMdlPanel

IMPLEMENT_DYNCREATE(CMdlPanel, CView)

CMdlPanel::CMdlPanel()
{
	g_pApp->m_pMdlPanel = this;

	//创建显示端口名称的小字体
	m_mylf.lfHeight = -12;
	m_mylf.lfWidth = 0;
	m_mylf.lfEscapement = 0;
	m_mylf.lfOrientation = 0;
	m_mylf.lfWeight = 400;
	m_mylf.lfItalic = 0; 
	m_mylf.lfUnderline = 0;
	m_mylf.lfStrikeOut = 0;
	m_mylf.lfCharSet = 134;
	m_mylf.lfOutPrecision = 3;
	m_mylf.lfClipPrecision = 2;
	m_mylf.lfQuality = 1;
	m_mylf.lfPitchAndFamily = 2;
	strcpy(m_mylf.lfFaceName, "宋体");

}

CMdlPanel::~CMdlPanel()
{
}

BEGIN_MESSAGE_MAP(CMdlPanel, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_PM_COMINFO, &CMdlPanel::OnPmCominfo)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_Menu_COMDef, &CMdlPanel::OnMenuComdef)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CMdlPanel drawing

void CMdlPanel::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	//创建新的字体
	CFont font;
	VERIFY(font.CreateFontIndirect(&m_mylf));
	HFONT hFont = (HFONT)font;
	HFONT hOldFont = (HFONT)::SelectObject(pDC->m_hDC, hFont);

	pDC->SetBkMode(TRANSPARENT);	//设置背景透明，防止字体底色同组件背景色不符
	//====================== 画连线 ==================================================
	for (unsigned int i = 0; i < g_pApp->m_miseModel.connlines.size(); i++)
	{
		//被选中的连线
		if (g_pApp->m_miseModel.connlines[i].m_isSelected)
		{
			CPen newPen, *oldPen;
			newPen.CreatePen(PS_SOLID, 1, SLCT_LINE_CLR);
			oldPen = pDC->SelectObject(&newPen);

			pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
			if ( g_pApp->m_miseModel.connlines[i].break2y == -1 )
			{
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].startPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].endPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
			}
			else
			{
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
			}
			pDC->SelectObject(oldPen);
			newPen.DeleteObject();
		}

		//未被选中的连线
		else
		{
			CPen newPen, *oldPen;
			newPen.CreatePen(PS_SOLID, 1, LINE_CLR);
			oldPen = pDC->SelectObject(&newPen);

			pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
			if ( g_pApp->m_miseModel.connlines[i].break2y == -1 )
			{
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].startPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].endPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
			}
			else
			{
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
				pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
			}
			pDC->SelectObject(oldPen);
			newPen.DeleteObject();
		}
	}
	//========================== end of 画连线 ==================================================

	//===================== 画组件 ==============================================
	for(unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		CRect nameRect;
		if ( g_pApp->m_miseModel.components[i]->isSelected )
		{
			CPen newPen, *pOldPen;
			newPen.CreatePen(PS_SOLID, 1, SLCT_COMP_CLR);
			pOldPen = pDC->SelectObject(&newPen);

			CBrush newBrush(COM_BKCLR);
			CBrush* pOldBrush = pDC->SelectObject(&newBrush);

			pDC->Rectangle(g_pApp->m_miseModel.components[i]->GetRect());
			
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
			newPen.DeleteObject();
			newBrush.DeleteObject();
		}
		else
		{
			CPen newPen, *pOldPen;
			newPen.CreatePen(PS_SOLID, 1, COMP_CLR);
			pOldPen = pDC->SelectObject(&newPen);

			CBrush newBrush(COM_BKCLR);
			CBrush* pOldBrush = pDC->SelectObject(&newBrush);

			pDC->Rectangle(g_pApp->m_miseModel.components[i]->GetRect());
			
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
			newPen.DeleteObject();
			newBrush.DeleteObject();
		}
		//显示模块名称和执行顺序
		nameRect.top	= g_pApp->m_miseModel.components[i]->GetRect().bottom + 1;
		nameRect.bottom	= nameRect.top + 10;
		nameRect.left	= g_pApp->m_miseModel.components[i]->GetRect().left - 20;
		nameRect.right	= g_pApp->m_miseModel.components[i]->GetRect().right + 20;
		CString strNameAndOrder;
		strNameAndOrder.Format("%s [%d]", g_pApp->m_miseModel.components[i]->GetName(), i);
		pDC->DrawText(strNameAndOrder, strNameAndOrder.GetLength(), &nameRect, DT_TOP | DT_CENTER | DT_NOCLIP);

		//--------------- 画输入输出接口-------------------------------------------------
		for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
		{
			pDC->MoveTo(g_pApp->m_miseModel.components[i]->inputs[j].pos.x - IO_ARROW_SIZE, g_pApp->m_miseModel.components[i]->inputs[j].pos.y - IO_ARROW_SIZE);
			pDC->LineTo(g_pApp->m_miseModel.components[i]->inputs[j].pos.x, g_pApp->m_miseModel.components[i]->inputs[j].pos.y);
			pDC->LineTo(g_pApp->m_miseModel.components[i]->inputs[j].pos.x - IO_ARROW_SIZE, g_pApp->m_miseModel.components[i]->inputs[j].pos.y + IO_ARROW_SIZE);

			if ( g_pApp->m_miseModel.components[i]->m_displayPortName )
			{
				//定义显示接口名称的区域
				nameRect.top	= g_pApp->m_miseModel.components[i]->inputs[j].pos.y -5;
				nameRect.bottom	= g_pApp->m_miseModel.components[i]->inputs[j].pos.y + 5;
				nameRect.left	= g_pApp->m_miseModel.components[i]->inputs[j].pos.x + 2;
				nameRect.right	= (g_pApp->m_miseModel.components[i]->GetRect().left + g_pApp->m_miseModel.components[i]->GetRect().right) / 2 - 1;

				//加法器特殊处理，只显示正负号
				if ( g_pApp->m_miseModel.components[i]->type == MATH_ADD)
				{
					if(g_pApp->m_miseModel.components[i]->inputs[j].name.GetAt(0) == '-')
					{
						pDC->DrawText("-", 1, &nameRect, DT_VCENTER | DT_LEFT);
					}
					else
					{
						pDC->DrawText("+", 1, &nameRect, DT_VCENTER | DT_LEFT);
					}

				}
				else
				{
					pDC->DrawText(g_pApp->m_miseModel.components[i]->inputs[j].name, 
						g_pApp->m_miseModel.components[i]->inputs[j].name.GetLength(), 
						&nameRect, 
						DT_VCENTER | DT_LEFT);
				}
			}
		}
		for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->outputs.size(); j++)
		{
			pDC->MoveTo(g_pApp->m_miseModel.components[i]->outputs[j].pos.x, g_pApp->m_miseModel.components[i]->outputs[j].pos.y - IO_ARROW_SIZE);
			pDC->LineTo(g_pApp->m_miseModel.components[i]->outputs[j].pos.x + IO_ARROW_SIZE, g_pApp->m_miseModel.components[i]->outputs[j].pos.y);
			pDC->LineTo(g_pApp->m_miseModel.components[i]->outputs[j].pos.x, g_pApp->m_miseModel.components[i]->outputs[j].pos.y + IO_ARROW_SIZE);

			if ( g_pApp->m_miseModel.components[i]->m_displayPortName &&  g_pApp->m_miseModel.components[i]->type != MATH_ADD && g_pApp->m_miseModel.components[i]->type != MATH_POW)
			{
				//定义显示接口名称的区域
				nameRect.top	= g_pApp->m_miseModel.components[i]->outputs[j].pos.y -5;
				nameRect.bottom	= g_pApp->m_miseModel.components[i]->outputs[j].pos.y + 5;
				nameRect.left	= (g_pApp->m_miseModel.components[i]->GetRect().left + g_pApp->m_miseModel.components[i]->GetRect().right) / 2 + 1;
				nameRect.right	= g_pApp->m_miseModel.components[i]->outputs[j].pos.x - 2;

				pDC->DrawText(g_pApp->m_miseModel.components[i]->outputs[j].name, 
					g_pApp->m_miseModel.components[i]->outputs[j].name.GetLength(), 
					&nameRect, 
					DT_VCENTER | DT_RIGHT);
			}

		}
		//----------------- end of 画输入输出接口 ------------------------------------

		//根据不同类型得组件显示不同的信息，便于观察
		switch ( g_pApp->m_miseModel.components[i]->type )
		{
			case(SRC_CLOCK):
			{
				Mise_source_clock *pSourceClock = (Mise_source_clock*)(g_pApp->m_miseModel.components[i]);
				CPoint ptTmp = pSourceClock->rect.CenterPoint();
				int radius = pSourceClock->rect.Width() < pSourceClock->rect.Height() ? pSourceClock->rect.Width() / 2 : pSourceClock->rect.Height() / 2;
				ASSERT (radius > 0);
				if(radius > 30 )
				{
					radius = 30;
				}
				CRect rectTmp( ptTmp.x - radius + 6, ptTmp.y - radius + 6, ptTmp.x + radius - 6, ptTmp.y + radius - 6 );

				pDC->Ellipse(&rectTmp);
				pDC->MoveTo(ptTmp.x, ptTmp.y - radius + 10 );
				pDC->LineTo(ptTmp);
				pDC->LineTo(ptTmp.x + radius - 13, ptTmp.y);
				break;
			}
			case(SRC_STEP):
			{
				Mise_source_step *pSourceStep = (Mise_source_step*)(g_pApp->m_miseModel.components[i]);
				CPoint ptTmp = pSourceStep->rect.CenterPoint();
				int radius = pSourceStep->rect.Width() < pSourceStep->rect.Height() ? pSourceStep->rect.Width() / 2 : pSourceStep->rect.Height() / 2;
				ASSERT (radius > 0);
				if(radius > 30 )
				{
					radius = 30;
				}

				pDC->MoveTo(ptTmp.x - radius + 10, ptTmp.y + radius - 15);
				pDC->LineTo(ptTmp.x, ptTmp.y + radius - 15);
				pDC->LineTo(ptTmp.x, ptTmp.y - radius + 15);
				pDC->LineTo(ptTmp.x + radius - 10, ptTmp.y - radius + 15);

				break;
			}
			case(SRC_SIN):
			{
				Mise_source_sin *pSourceSin = (Mise_source_sin*)(g_pApp->m_miseModel.components[i]);
				pDC->DrawText(pSourceSin->m_title, pSourceSin->m_title.GetLength(), &(pSourceSin->rect), DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				break;
			}
			case(SRC_COS):
			{
				Mise_source_cos *pSourceCos = (Mise_source_cos*)(g_pApp->m_miseModel.components[i]);
				pDC->DrawText(pSourceCos->m_title, pSourceCos->m_title.GetLength(), &(pSourceCos->rect), DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				break;
			}
			case(SRC_CONST):
			{
				Mise_source_const *pSourceConst = (Mise_source_const*)(g_pApp->m_miseModel.components[i]);
				CString strTmp;
				strTmp.Format("%f", pSourceConst->m_value);
				DelEndZeroFromStr(strTmp);
				pDC->DrawText(strTmp, strTmp.GetLength(), &(g_pApp->m_miseModel.components[i]->rect), DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				break;
			}
			case(CONT_TRANSFUN):
			{
				Mise_cont_transfunction *pContTranFun = (Mise_cont_transfunction*)(g_pApp->m_miseModel.components[i]);
				int maxLength = max(pContTranFun->m_title_up.GetLength(), pContTranFun->m_title_down.GetLength());
				CPoint ptTmp = pContTranFun->rect.CenterPoint();
				CRect rectTmp(pContTranFun->rect.left, ptTmp.y - 12, pContTranFun->rect.right, ptTmp.y - 2);
				pDC->DrawText(pContTranFun->m_title_up, pContTranFun->m_title_up.GetLength(), &(rectTmp), DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				rectTmp.top = ptTmp.y + 2;
				rectTmp.bottom = ptTmp.y + 12;
				pDC->DrawText(pContTranFun->m_title_down, pContTranFun->m_title_down.GetLength(), &(rectTmp), DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				CString strTmp;
				for ( int i = 0; i < maxLength + 1; i++ )
				{
					strTmp += "-";
				}
				rectTmp.top = ptTmp.y - 2;
				rectTmp.bottom = ptTmp.y + 2;

				pDC->DrawText(strTmp, strTmp.GetLength(), &(rectTmp), DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				break;
			}

			case(CONT_ZEROPOLE):
			{
				Mise_cont_ZeroPole *pContZeroPole = (Mise_cont_ZeroPole*)(g_pApp->m_miseModel.components[i]);
				int maxLength = max(pContZeroPole->m_title_up.GetLength(), pContZeroPole->m_title_down.GetLength());
				CPoint ptTmp = pContZeroPole->rect.CenterPoint();
				CRect rectTmp(pContZeroPole->rect.left, ptTmp.y - 12, pContZeroPole->rect.right, ptTmp.y - 2);
				pDC->DrawText(pContZeroPole->m_title_up, pContZeroPole->m_title_up.GetLength(), &(rectTmp), DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				rectTmp.top = ptTmp.y + 2;
				rectTmp.bottom = ptTmp.y + 12;
				pDC->DrawText(pContZeroPole->m_title_down, pContZeroPole->m_title_down.GetLength(), &(rectTmp), DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				CString strTmp;
				for ( int i = 0; i < maxLength + 1; i++ )
				{
					strTmp += "-";
				}
				rectTmp.top = ptTmp.y - 2;
				rectTmp.bottom = ptTmp.y + 2;

				pDC->DrawText(strTmp, strTmp.GetLength(), &(rectTmp), DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				break;
			}
			default:
			{
				break;
			}
		}
				
	}
	//====================== end of 画组件 ===========================================
	//恢复字体
	::SelectObject(pDC->m_hDC, hOldFont);	//restore old font
	::DeleteObject(hFont);

}


// CMdlPanel diagnostics

#ifdef _DEBUG
void CMdlPanel::AssertValid() const
{
	CView::AssertValid();
}

void CMdlPanel::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CMdlPanel message handlers

void CMdlPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	
	m_oldPoint = point;	//记录此次点击鼠标的位置

	//------------ 如果点击在某个组件的某个输出端口内，则为连线操作 ----------------------------
	for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		for ( unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->outputs.size(); j++)
		{
			CRect rectTmp(	g_pApp->m_miseModel.components[i]->outputs[j].pos.x,
							g_pApp->m_miseModel.components[i]->outputs[j].pos.y - IO_ARROW_SIZE,
							g_pApp->m_miseModel.components[i]->outputs[j].pos.x + IO_ARROW_SIZE,
							g_pApp->m_miseModel.components[i]->outputs[j].pos.y + IO_ARROW_SIZE
						 );
			if(rectTmp.PtInRect(m_oldPoint))
			{
				m_action = CONN_COMP;	//标识当前为连线操作
				m_startPoint.x = g_pApp->m_miseModel.components[i]->outputs[j].pos.x + IO_ARROW_SIZE;	//记录连线起点
				m_startPoint.y = g_pApp->m_miseModel.components[i]->outputs[j].pos.y;
				m_outportName.Empty();
				m_outportName.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->outputs[j].name);
				return;
			}
		}
	}
	//------------ end of 连线 --------------------------------------------------------------------


	//------------ 如果点击在某个组件范围内，则为选择组件或连线操作 -------------------------------------
	BOOL sign = FALSE;			//确保在不按ctrl的时候只选中一个组件
	
	m_action = SLCT_COMP;	//标识当前为选择组件的操作

	//如果按下ctrl键，表示多选。点击一次选中，点击两次取消选中
	if ( nFlags & MK_CONTROL ){
		unsigned int i;
		//选择组件
		//因为i是无符号型整数，即永远不会小于零，所以这里要小心
		for ( i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
		{
			if (g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point))
			{
				g_pApp->m_miseModel.components[i - 1]->isSelected = g_pApp->m_miseModel.components[i - 1]->isSelected ? FALSE : TRUE;
			}
		}
		//选择连线
		for( i = g_pApp->m_miseModel.connlines.size(); i > 0; i-- )
		{
			if (g_pApp->m_miseModel.connlines[i - 1].NeedBeSelected(point))
			{
				g_pApp->m_miseModel.connlines[i - 1].m_isSelected = g_pApp->m_miseModel.connlines[i - 1].m_isSelected? FALSE :TRUE;
			}
		}
	}
	//如果没有按下ctrl键，为单选，（拖动多个组件的情况以后再考虑）
	else
	{
		unsigned int i;
		BOOL sign = FALSE;	//标识，确保同一次至多只能有一个组件或连线被选中
		//------------- 处理组件 ---------------------------------------------------------
		//因为i是无符号型整数，即永远不会小于零，所以这里要小心
		for (i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
		{
			if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
			{
				break;
			}
		}
		//首先将所有组件置为未选中状态
		for (unsigned int j = g_pApp->m_miseModel.components.size(); j > 0 ; j-- )
		{
			g_pApp->m_miseModel.components[j - 1]->isSelected = FALSE;
		}
		//如果点击在任一组件范围内，则将此组件置为选中
		if ( i > 0 )
		{
			g_pApp->m_miseModel.components[i - 1]->isSelected = TRUE;
			sign = TRUE;

			// 如果点中的位置距离边框很近，则认为是改变模块大小的操作----------------------
			if ( point.x - g_pApp->m_miseModel.components[i - 1]->rect.left < SEL_SENSITIVITY )
			{
				m_action = RESZ_COMP_LEFT;
			}
			else if ( g_pApp->m_miseModel.components[i - 1]->rect.right - point.x < SEL_SENSITIVITY )
			{
				m_action = RESZ_COMP_RIGHT;
			}
			
			else if ( point.y - g_pApp->m_miseModel.components[i - 1]->rect.top < SEL_SENSITIVITY )
			{
				m_action = RESZ_COMP_TOP;
			}
			else if ( g_pApp->m_miseModel.components[i - 1]->rect.bottom - point.y < SEL_SENSITIVITY )
			{
				m_action = RESZ_COMP_BOTTOM;
			}
			// end of 改变模块大小 --------------------------------------------------------

			//将选中的component提到最前面，相当于vector的最后一个元素
			if ( nFlags & MK_SHIFT )
			{
				rotate(g_pApp->m_miseModel.components.begin() + i - 1, g_pApp->m_miseModel.components.begin() + i, g_pApp->m_miseModel.components.end() );
			}
		}




		////如果未点击在任一组件范围内，或点击的组件未被被选中
		//if ( i == 0 || !g_pApp->m_miseModel.components[i - 1]->isSelected)
		//{
		//	//首先将所有组件置为未选中
		//	for (unsigned int j = g_pApp->m_miseModel.components.size(); j > 0 ; j-- )
		//	{
		//		g_pApp->m_miseModel.components[j - 1]->isSelected = FALSE;
		//	}
		//	//如果点击在某个组件范围内，则将此组件置为选中
		//	if ( i > 0 )
		//	{
		//		g_pApp->m_miseModel.components[i - 1]->isSelected = TRUE;
		//		sign = TRUE;

		//		// 如果点中的位置距离边框很近，则认为是改变模块大小的操作----------------------
		//		if ( point.x - g_pApp->m_miseModel.components[i - 1]->rect.left < SEL_SENSITIVITY )
		//		{
		//			m_action = RESZ_COMP_LEFT;
		//		}
		//		else if ( g_pApp->m_miseModel.components[i - 1]->rect.right - point.x < SEL_SENSITIVITY )
		//		{
		//			m_action = RESZ_COMP_RIGHT;
		//		}
		//		
		//		else if ( point.y - g_pApp->m_miseModel.components[i - 1]->rect.top < SEL_SENSITIVITY )
		//		{
		//			m_action = RESZ_COMP_TOP;
		//		}
		//		else if ( g_pApp->m_miseModel.components[i - 1]->rect.bottom - point.y < SEL_SENSITIVITY )
		//		{
		//			m_action = RESZ_COMP_BOTTOM;
		//		}
		//		// end of 改变模块大小 --------------------------------------------------------

		//		//将选中的component提到最前面，相当于vector的最后一个元素
		//		if ( nFlags & MK_SHIFT )
		//		{
		//			rotate(g_pApp->m_miseModel.components.begin() + i - 1, g_pApp->m_miseModel.components.begin() + i, g_pApp->m_miseModel.components.end() );
		//		}
		//	}
		//}
		//------------------ end of 处理组件 -----------------------------------------

		//------------------ 处理连线 ------------------------------------------------
		if( sign )	//已经有组件被选中，则清除所有连线的选中标记
		{
			for ( i = g_pApp->m_miseModel.connlines.size(); i > 0; i-- )
			{
				g_pApp->m_miseModel.connlines[i - 1].m_isSelected = FALSE;
			}


		}
		else		//没有组件被选中，则检测是否选择连线
		{
			for( i = g_pApp->m_miseModel.connlines.size(); i > 0; i-- )
			{
				if (g_pApp->m_miseModel.connlines[i - 1].NeedBeSelected(point))
				{
					break;
				}
			}
			//如果没有连线被选中，或被选中的连线原状态未被选中
			if ( i == 0 || !g_pApp->m_miseModel.connlines[i - 1].m_isSelected)
			{
				//首先将所有连线置为未选中
				for (unsigned int j = g_pApp->m_miseModel.connlines.size(); j > 0 ; j-- )
				{
					g_pApp->m_miseModel.connlines[j - 1].m_isSelected = FALSE;
				}
				//如果点击在某个连线范围内，则将此连线置为选中
				if ( i > 0 )
				{
					g_pApp->m_miseModel.connlines[i - 1].m_isSelected = TRUE;
					m_action = SLCT_LINE;	//标识当前为选择连线的操作
				}
			}
			//如果当前连线已处于选中状态，则也标识当前为选择连线的操作
			if ( i > 0 && g_pApp->m_miseModel.connlines[i - 1].m_isSelected)
			{
				m_action = SLCT_LINE;	//标识当前为选择连线的操作
			}
		}
		//---------------- end of 处理连线 --------------------------------------------
	}


	//刷新模型
	this->Invalidate();

	//------------- end of 选择 -----------------------------------

	CView::OnLButtonDown(nFlags, point);
}

void CMdlPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	unsigned int i;
	BOOL sign = FALSE;	//标识，确保同一次至多只能有一个组件或连线被选中
	//------------- 处理组件 ---------------------------------------------------------
	//因为i是无符号型整数，即永远不会小于零，所以这里要小心
	for (i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
	{
		if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
		{
			break;
		}
	}
	//如果点击在某个组件范围内，则将此组件置为选中
	if ( i > 0 )
	{
		g_pApp->m_miseModel.components[i - 1]->Display();
	}
}
void CMdlPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// 没按下按键时候的检测，用于在不同位置显示不同光标
	if ( (nFlags & MK_LBUTTON) == 0 && (nFlags & MK_LBUTTON) == 0)
	{
		m_action = NONE_ACTION;
		//------------ 如果落在在某个组件的某个输出端口内，则可以进行连线操作 ----------------------------
		for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
		{
			for ( unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->outputs.size(); j++)
			{
				CRect rectTmp(	g_pApp->m_miseModel.components[i]->outputs[j].pos.x,
								g_pApp->m_miseModel.components[i]->outputs[j].pos.y - IO_ARROW_SIZE,
								g_pApp->m_miseModel.components[i]->outputs[j].pos.x + IO_ARROW_SIZE,
								g_pApp->m_miseModel.components[i]->outputs[j].pos.y + IO_ARROW_SIZE
							 );
				if(rectTmp.PtInRect(point))
				{
					m_action = CONN_COMP;	//标识当前为连线操作
					return;
				}
			}
		}
		// ------------- 如果落在某个模块的边线范围内，则可以进行改变大小的操作 -----------------------------
		for (unsigned int i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
		{
			if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
			{
				if ( point.x - g_pApp->m_miseModel.components[i - 1]->rect.left < SEL_SENSITIVITY )
				{
					m_action = RESZ_COMP_LEFT;
				}
				else if ( g_pApp->m_miseModel.components[i - 1]->rect.right - point.x < SEL_SENSITIVITY )
				{
					m_action = RESZ_COMP_RIGHT;
				}
				
				else if ( point.y - g_pApp->m_miseModel.components[i - 1]->rect.top < SEL_SENSITIVITY )
				{
					m_action = RESZ_COMP_TOP;
				}
				else if ( g_pApp->m_miseModel.components[i - 1]->rect.bottom - point.y < SEL_SENSITIVITY )
				{
					m_action = RESZ_COMP_BOTTOM;
				}
				break;
			}
		}

		return CView::OnMouseMove(nFlags, point);
	}



	//---------------- 拖动组件操作 ----------------------------------------------------
	if (m_action == SLCT_COMP)		//当前为选择组件的操作
	{
		//按住左键时的拖动
		if ( nFlags == MK_LBUTTON )
		{
			BOOL needRefresh = FALSE;
			MiseComponent *pComponent = 0;
			for ( unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++ )
			{
				if (g_pApp->m_miseModel.components[i]->isSelected)
				{
					pComponent = g_pApp->m_miseModel.components[i];
					pComponent->Move(point.x - m_oldPoint.x, point.y - m_oldPoint.y);
					needRefresh = TRUE;
				}
			}
			if (needRefresh)
			{
				this->Invalidate();
			}
		}
	}
	//----------------- end of 拖动组件操作 --------------------------------------------

	//----------------- 连线操作 -------------------------------------------------------
	else if ( nFlags == MK_LBUTTON && m_action == CONN_COMP)
	{
		int break1x;		//第一个拐点的x坐标
		int break2y;		//第二个拐点的y坐标
		int break3x;		//第三个拐点的x坐标，如果为-1，则无第三个和第四个拐点
		BOOL need4breaks = FALSE;	//标识是否需要四个拐点

		CDC* pDC;
		pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);	//设定画线模式，便于擦除原来的连线

		//首先擦除原连线
		if ( m_oldPoint.x - m_startPoint.x > BREAK_SPACE )
		{
			break1x = (m_startPoint.x + m_oldPoint.x) / 2;
			need4breaks = FALSE;
		}
		else
		{
			break1x = m_startPoint.x + BREAK_SPACE / 2;
			break2y = (m_startPoint.y + m_oldPoint.y) / 2;
			break3x = m_oldPoint.x - BREAK_SPACE / 2;
			need4breaks = TRUE;
		}


		pDC->MoveTo(m_startPoint);
		if ( !need4breaks )
		{
			pDC->LineTo(break1x, m_startPoint.y);
			pDC->LineTo(break1x, m_oldPoint.y);
			pDC->LineTo(m_oldPoint);
		}
		else
		{
			pDC->LineTo(break1x, m_startPoint.y);
			pDC->LineTo(break1x, break2y);
			pDC->LineTo(break3x, break2y);
			pDC->LineTo(break3x, m_oldPoint.y);
			pDC->LineTo(m_oldPoint);
		}

		//重新画线
		if ( point.x - m_startPoint.x > BREAK_SPACE )
		{
			break1x = (m_startPoint.x + point.x) / 2;
			need4breaks = FALSE;
		}
		else
		{
			break1x = m_startPoint.x + BREAK_SPACE / 2;
			break2y = (m_startPoint.y + point.y) / 2;
			break3x = point.x - BREAK_SPACE / 2;
			need4breaks = TRUE;
		}

		pDC->MoveTo(m_startPoint);
		if ( !need4breaks )
		{
			pDC->LineTo(break1x, m_startPoint.y);
			pDC->LineTo(break1x, point.y);
			pDC->LineTo(point);
		}
		else
		{
			pDC->LineTo(break1x, m_startPoint.y);
			pDC->LineTo(break1x, break2y);
			pDC->LineTo(break3x, break2y);
			pDC->LineTo(break3x, point.y);
			pDC->LineTo(point);
		}

		ReleaseDC(pDC);
	}
	//------------ end of 连线 --------------------------------------------------------------------
	
	//------------ 拖动连线的操作 -----------------------------------------------------------------
	else if ( nFlags == MK_LBUTTON && m_action == SLCT_LINE)
	{
		CDC* pDC;
		pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);	//设定画线模式，便于擦除原来的连线
		unsigned int i;
		//只处理第一条被选中的连线，首先找到这条连线
		for( i = 0; i < g_pApp->m_miseModel.connlines.size(); i++)
		{
			if(g_pApp->m_miseModel.connlines[i].m_isSelected)
			{
				break;
			}
		}

		if(i < g_pApp->m_miseModel.connlines.size())
		{
			CPen newPen, *oldPen;
			newPen.CreatePen(PS_SOLID, 1, SLCT_LINE_CLR);
			oldPen = pDC->SelectObject(&newPen);
			//如果只有两个拐点
			if(g_pApp->m_miseModel.connlines[i].break2y == -1 )
			{
				//只处理对第二段的拖动
				if(g_pApp->m_miseModel.connlines[i].selectedPart == 2)
				{
					//擦除原连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//修正break1x
					g_pApp->m_miseModel.connlines[i].break1x = point.x;

					//重新连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}
			}
			//如果有四个拐点
			else
			{
				//处理第二段的拖动
				if(g_pApp->m_miseModel.connlines[i].selectedPart == 2)
				{
					//擦除原连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//修正break1x
					g_pApp->m_miseModel.connlines[i].break1x = point.x;

					//重新连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}

				//处理第三段的拖动
				else if(g_pApp->m_miseModel.connlines[i].selectedPart == 3)
				{
					//擦除原连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//修正break1x
					g_pApp->m_miseModel.connlines[i].break2y = point.y;

					//重新连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}

				//处理第四段的拖动
				else if(g_pApp->m_miseModel.connlines[i].selectedPart == 4)
				{
					//擦除原连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//修正break1x
					g_pApp->m_miseModel.connlines[i].break3x = point.x;

					//重新连线
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}
			}
			pDC->SelectObject(oldPen);
			newPen.DeleteObject();

		}
	}

	//------------ end of 拖动连线 ----------------------------------------------------------------

	//------------ 改变模块大小 -------------------------------------------------------------------
	else if(m_action == RESZ_COMP_RIGHT )
	{
		for ( unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++ )
		{
			if (g_pApp->m_miseModel.components[i]->isSelected)
			{
				if ( point.x - g_pApp->m_miseModel.components[i]->rect.left > COM_MINSIZE )
				{
					CRect rect = g_pApp->m_miseModel.components[i]->rect;
					rect.right = point.x;

					g_pApp->m_miseModel.components[i]->MoveEx(rect);
					this->Invalidate();
				}
			}
		}
	}
	else if(m_action == RESZ_COMP_LEFT )
	{
		for ( unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++ )
		{
			if (g_pApp->m_miseModel.components[i]->isSelected)
			{
				if ( g_pApp->m_miseModel.components[i]->rect.right - point.x > COM_MINSIZE )
				{
					CRect rect = g_pApp->m_miseModel.components[i]->rect;
					rect.left = point.x;

					g_pApp->m_miseModel.components[i]->MoveEx(rect);
					this->Invalidate();
				}
			}
		}
	}
	else if(m_action == RESZ_COMP_TOP )
	{
		for ( unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++ )
		{
			if (g_pApp->m_miseModel.components[i]->isSelected)
			{
				if ( g_pApp->m_miseModel.components[i]->rect.bottom - point.y > COM_MINSIZE )
				{
					CRect rect = g_pApp->m_miseModel.components[i]->rect;
					rect.top = point.y;

					g_pApp->m_miseModel.components[i]->MoveEx(rect);
					this->Invalidate();
				}
			}
		}
	}
	else if(m_action == RESZ_COMP_BOTTOM )
	{
		for ( unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++ )
		{
			if (g_pApp->m_miseModel.components[i]->isSelected)
			{
				if ( point.y - g_pApp->m_miseModel.components[i]->rect.top > COM_MINSIZE )
				{
					CRect rect = g_pApp->m_miseModel.components[i]->rect;
					rect.bottom = point.y;

					g_pApp->m_miseModel.components[i]->MoveEx(rect);
					this->Invalidate();
				}
			}
		}
	}

	m_oldPoint = point;

	CView::OnMouseMove(nFlags, point);
}

void CMdlPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//------------- 完成连线操作 ---------------------------------
	if (  m_action == CONN_COMP )
	{
		for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
		{
			for ( unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
			{
				CRect rectTmp(	g_pApp->m_miseModel.components[i]->inputs[j].pos.x - IO_ARROW_SIZE,
								g_pApp->m_miseModel.components[i]->inputs[j].pos.y - IO_ARROW_SIZE,
								g_pApp->m_miseModel.components[i]->inputs[j].pos.x,
								g_pApp->m_miseModel.components[i]->inputs[j].pos.y + IO_ARROW_SIZE
							 );
				//鼠标释放位置在某个模块的输入端口范围内
				if(rectTmp.PtInRect(m_oldPoint))
				{
					//------------ 判断是否已经有连线连接到该输入端口。原则上一个输入端口只能接受一个输入 -------------------------------
					CString inportName;
					inportName.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->inputs[j].name);
					unsigned int k;
					for(k = 0; k < g_pApp->m_miseModel.connlines.size(); k++)
					{
						if ( g_pApp->m_miseModel.connlines[k].inport == inportName )
						{
							break;
						}
					}
					if ( k < g_pApp->m_miseModel.connlines.size() )
					{
						AfxMessageBox("错误，一个输入端口只能接受一个输入！");
						this->Invalidate();
						return;
					}
					//------------ end of 判断 -------------------------------------------------------------------------------------------

					m_endPoint = g_pApp->m_miseModel.components[i]->inputs[j].pos; 
					ConnectLine* pConnLine = new ConnectLine;
					pConnLine->outport = m_outportName;
					pConnLine->inport = inportName;
					pConnLine->startPoint = m_startPoint;
					pConnLine->endPoint = m_endPoint;

					if ( m_endPoint.x - m_startPoint.x > BREAK_SPACE )
					{
						pConnLine->break1x = (m_startPoint.x + m_endPoint.x) / 2;
						pConnLine->break2y = -1;
					}
					else
					{
						pConnLine->break1x = m_startPoint.x + BREAK_SPACE / 2;
						pConnLine->break2y = (m_startPoint.y + m_endPoint.y) / 2;
						pConnLine->break3x = m_endPoint.x - BREAK_SPACE / 2;
					}
					//将此连线添加到连线序列中
					pConnLine->parent = &(g_pApp->m_miseModel);
					g_pApp->m_miseModel.connlines.push_back(*pConnLine);
					
				}
			}
		}
		this->Invalidate();
	}
	//------------- end of 连线 ----------------------------------
	m_action = NONE_ACTION;
	CView::OnLButtonUp(nFlags, point);
}

void CMdlPanel::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//------------ 如果在某个组件上点击右键，则弹出属性菜单 -----------------
	unsigned int i;
	//因为i是无符号型整数，即永远不会小于零，所以这里要小心
	for (i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
	{
		if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
		{
			break;
		}
	}
	//如果点击在任一组件范围内
	if ( i > 0 )
	{
		//首先将所有组件置为未选中
		for (unsigned int j = g_pApp->m_miseModel.components.size(); j > 0 ; j-- )
		{
			g_pApp->m_miseModel.components[j - 1]->isSelected = FALSE;
		}
		//将此组件置为选中，且弹出属性菜单
		g_pApp->m_miseModel.components[i - 1]->isSelected = TRUE;
		this->Invalidate();

		//记录当前选中组件，用于定义其属性
		m_selectedComponent = g_pApp->m_miseModel.components[i - 1];

		//弹出属性菜单
		CMenu menu_bar;
		menu_bar.LoadMenu(IDR_PopMenu);
		CMenu* file_menu = menu_bar.GetSubMenu(0);    
		ASSERT(file_menu);
		ClientToScreen(&point);
		file_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	//--------------- end of 属性菜单 ----------------------------------

	CView::OnRButtonDown(nFlags, point);
}


void CMdlPanel::OnPmCominfo()
{
	// TODO: Add your command handler code here
	CComInfoDialog dlgComInfo(m_selectedComponent);
	dlgComInfo.DoModal();
	this->Invalidate();
}

void CMdlPanel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE)
	{
		//删除组件
		for (int i = 0; i < int(g_pApp->m_miseModel.components.size()); i++ )
		{
			if ( g_pApp->m_miseModel.components[i]->isSelected )
			{
				CString strTmp;
				strTmp = "确实要删除组件：";
				strTmp += g_pApp->m_miseModel.components[i]->name;
				strTmp += "？";
				if(AfxMessageBox(strTmp, MB_OKCANCEL) == IDOK)	//let the user affirm
				{
					g_pApp->m_miseModel.DelComponent(i);
					i--;
					this->Invalidate();
				}
			}
		}
		//删除连线
		for (int i = 0; i < g_pApp->m_miseModel.connlines.size(); i++)
		{
			if (g_pApp->m_miseModel.connlines[i].m_isSelected)
			{
				g_pApp->m_miseModel.connlines.erase(g_pApp->m_miseModel.connlines.begin() + i);
				i--;
				this->Invalidate();
			}
		}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//定义模块的属性，包括名称和对应的dll文件的位置
void CMdlPanel::OnMenuComdef()
{
	m_selectedComponent->Display();
	
	//更新界面
	this->Invalidate();
}

BOOL CMdlPanel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	switch ( m_action )
	{
	case(CONN_COMP):
		SetCursor(g_pApp->LoadCursor(IDC_CUR_CROSS));
		return true;

	case(RESZ_COMP_LEFT):	case(RESZ_COMP_RIGHT):
		SetCursor(g_pApp->LoadCursor(IDC_CUR_RSZ_LR));
		return true;

	case(RESZ_COMP_TOP):	case(RESZ_COMP_BOTTOM):
		SetCursor(g_pApp->LoadCursor(IDC_CUR_RSZ_TB));
		return true;
		
	default:
		break;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}
