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

	//������ʾ�˿����Ƶ�С����
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
	strcpy(m_mylf.lfFaceName, "����");

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
	//�����µ�����
	CFont font;
	VERIFY(font.CreateFontIndirect(&m_mylf));
	HFONT hFont = (HFONT)font;
	HFONT hOldFont = (HFONT)::SelectObject(pDC->m_hDC, hFont);

	pDC->SetBkMode(TRANSPARENT);	//���ñ���͸������ֹ�����ɫͬ�������ɫ����
	//====================== ������ ==================================================
	for (unsigned int i = 0; i < g_pApp->m_miseModel.connlines.size(); i++)
	{
		//��ѡ�е�����
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

		//δ��ѡ�е�����
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
	//========================== end of ������ ==================================================

	//===================== ����� ==============================================
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
		//��ʾģ�����ƺ�ִ��˳��
		nameRect.top	= g_pApp->m_miseModel.components[i]->GetRect().bottom + 1;
		nameRect.bottom	= nameRect.top + 10;
		nameRect.left	= g_pApp->m_miseModel.components[i]->GetRect().left - 20;
		nameRect.right	= g_pApp->m_miseModel.components[i]->GetRect().right + 20;
		CString strNameAndOrder;
		strNameAndOrder.Format("%s [%d]", g_pApp->m_miseModel.components[i]->GetName(), i);
		pDC->DrawText(strNameAndOrder, strNameAndOrder.GetLength(), &nameRect, DT_TOP | DT_CENTER | DT_NOCLIP);

		//--------------- ����������ӿ�-------------------------------------------------
		for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
		{
			pDC->MoveTo(g_pApp->m_miseModel.components[i]->inputs[j].pos.x - IO_ARROW_SIZE, g_pApp->m_miseModel.components[i]->inputs[j].pos.y - IO_ARROW_SIZE);
			pDC->LineTo(g_pApp->m_miseModel.components[i]->inputs[j].pos.x, g_pApp->m_miseModel.components[i]->inputs[j].pos.y);
			pDC->LineTo(g_pApp->m_miseModel.components[i]->inputs[j].pos.x - IO_ARROW_SIZE, g_pApp->m_miseModel.components[i]->inputs[j].pos.y + IO_ARROW_SIZE);

			if ( g_pApp->m_miseModel.components[i]->m_displayPortName )
			{
				//������ʾ�ӿ����Ƶ�����
				nameRect.top	= g_pApp->m_miseModel.components[i]->inputs[j].pos.y -5;
				nameRect.bottom	= g_pApp->m_miseModel.components[i]->inputs[j].pos.y + 5;
				nameRect.left	= g_pApp->m_miseModel.components[i]->inputs[j].pos.x + 2;
				nameRect.right	= (g_pApp->m_miseModel.components[i]->GetRect().left + g_pApp->m_miseModel.components[i]->GetRect().right) / 2 - 1;

				//�ӷ������⴦��ֻ��ʾ������
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
				//������ʾ�ӿ����Ƶ�����
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
		//----------------- end of ����������ӿ� ------------------------------------

		//���ݲ�ͬ���͵������ʾ��ͬ����Ϣ�����ڹ۲�
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
	//====================== end of ����� ===========================================
	//�ָ�����
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

	
	m_oldPoint = point;	//��¼�˴ε������λ��

	//------------ ��������ĳ�������ĳ������˿��ڣ���Ϊ���߲��� ----------------------------
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
				m_action = CONN_COMP;	//��ʶ��ǰΪ���߲���
				m_startPoint.x = g_pApp->m_miseModel.components[i]->outputs[j].pos.x + IO_ARROW_SIZE;	//��¼�������
				m_startPoint.y = g_pApp->m_miseModel.components[i]->outputs[j].pos.y;
				m_outportName.Empty();
				m_outportName.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->outputs[j].name);
				return;
			}
		}
	}
	//------------ end of ���� --------------------------------------------------------------------


	//------------ ��������ĳ�������Χ�ڣ���Ϊѡ����������߲��� -------------------------------------
	BOOL sign = FALSE;			//ȷ���ڲ���ctrl��ʱ��ֻѡ��һ�����
	
	m_action = SLCT_COMP;	//��ʶ��ǰΪѡ������Ĳ���

	//�������ctrl������ʾ��ѡ�����һ��ѡ�У��������ȡ��ѡ��
	if ( nFlags & MK_CONTROL ){
		unsigned int i;
		//ѡ�����
		//��Ϊi���޷���������������Զ����С���㣬��������ҪС��
		for ( i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
		{
			if (g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point))
			{
				g_pApp->m_miseModel.components[i - 1]->isSelected = g_pApp->m_miseModel.components[i - 1]->isSelected ? FALSE : TRUE;
			}
		}
		//ѡ������
		for( i = g_pApp->m_miseModel.connlines.size(); i > 0; i-- )
		{
			if (g_pApp->m_miseModel.connlines[i - 1].NeedBeSelected(point))
			{
				g_pApp->m_miseModel.connlines[i - 1].m_isSelected = g_pApp->m_miseModel.connlines[i - 1].m_isSelected? FALSE :TRUE;
			}
		}
	}
	//���û�а���ctrl����Ϊ��ѡ�����϶�������������Ժ��ٿ��ǣ�
	else
	{
		unsigned int i;
		BOOL sign = FALSE;	//��ʶ��ȷ��ͬһ������ֻ����һ����������߱�ѡ��
		//------------- ������� ---------------------------------------------------------
		//��Ϊi���޷���������������Զ����С���㣬��������ҪС��
		for (i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
		{
			if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
			{
				break;
			}
		}
		//���Ƚ����������Ϊδѡ��״̬
		for (unsigned int j = g_pApp->m_miseModel.components.size(); j > 0 ; j-- )
		{
			g_pApp->m_miseModel.components[j - 1]->isSelected = FALSE;
		}
		//����������һ�����Χ�ڣ��򽫴������Ϊѡ��
		if ( i > 0 )
		{
			g_pApp->m_miseModel.components[i - 1]->isSelected = TRUE;
			sign = TRUE;

			// ������е�λ�þ���߿�ܽ�������Ϊ�Ǹı�ģ���С�Ĳ���----------------------
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
			// end of �ı�ģ���С --------------------------------------------------------

			//��ѡ�е�component�ᵽ��ǰ�棬�൱��vector�����һ��Ԫ��
			if ( nFlags & MK_SHIFT )
			{
				rotate(g_pApp->m_miseModel.components.begin() + i - 1, g_pApp->m_miseModel.components.begin() + i, g_pApp->m_miseModel.components.end() );
			}
		}




		////���δ�������һ�����Χ�ڣ����������δ����ѡ��
		//if ( i == 0 || !g_pApp->m_miseModel.components[i - 1]->isSelected)
		//{
		//	//���Ƚ����������Ϊδѡ��
		//	for (unsigned int j = g_pApp->m_miseModel.components.size(); j > 0 ; j-- )
		//	{
		//		g_pApp->m_miseModel.components[j - 1]->isSelected = FALSE;
		//	}
		//	//��������ĳ�������Χ�ڣ��򽫴������Ϊѡ��
		//	if ( i > 0 )
		//	{
		//		g_pApp->m_miseModel.components[i - 1]->isSelected = TRUE;
		//		sign = TRUE;

		//		// ������е�λ�þ���߿�ܽ�������Ϊ�Ǹı�ģ���С�Ĳ���----------------------
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
		//		// end of �ı�ģ���С --------------------------------------------------------

		//		//��ѡ�е�component�ᵽ��ǰ�棬�൱��vector�����һ��Ԫ��
		//		if ( nFlags & MK_SHIFT )
		//		{
		//			rotate(g_pApp->m_miseModel.components.begin() + i - 1, g_pApp->m_miseModel.components.begin() + i, g_pApp->m_miseModel.components.end() );
		//		}
		//	}
		//}
		//------------------ end of ������� -----------------------------------------

		//------------------ �������� ------------------------------------------------
		if( sign )	//�Ѿ��������ѡ�У�������������ߵ�ѡ�б��
		{
			for ( i = g_pApp->m_miseModel.connlines.size(); i > 0; i-- )
			{
				g_pApp->m_miseModel.connlines[i - 1].m_isSelected = FALSE;
			}


		}
		else		//û�������ѡ�У������Ƿ�ѡ������
		{
			for( i = g_pApp->m_miseModel.connlines.size(); i > 0; i-- )
			{
				if (g_pApp->m_miseModel.connlines[i - 1].NeedBeSelected(point))
				{
					break;
				}
			}
			//���û�����߱�ѡ�У���ѡ�е�����ԭ״̬δ��ѡ��
			if ( i == 0 || !g_pApp->m_miseModel.connlines[i - 1].m_isSelected)
			{
				//���Ƚ�����������Ϊδѡ��
				for (unsigned int j = g_pApp->m_miseModel.connlines.size(); j > 0 ; j-- )
				{
					g_pApp->m_miseModel.connlines[j - 1].m_isSelected = FALSE;
				}
				//��������ĳ�����߷�Χ�ڣ��򽫴�������Ϊѡ��
				if ( i > 0 )
				{
					g_pApp->m_miseModel.connlines[i - 1].m_isSelected = TRUE;
					m_action = SLCT_LINE;	//��ʶ��ǰΪѡ�����ߵĲ���
				}
			}
			//�����ǰ�����Ѵ���ѡ��״̬����Ҳ��ʶ��ǰΪѡ�����ߵĲ���
			if ( i > 0 && g_pApp->m_miseModel.connlines[i - 1].m_isSelected)
			{
				m_action = SLCT_LINE;	//��ʶ��ǰΪѡ�����ߵĲ���
			}
		}
		//---------------- end of �������� --------------------------------------------
	}


	//ˢ��ģ��
	this->Invalidate();

	//------------- end of ѡ�� -----------------------------------

	CView::OnLButtonDown(nFlags, point);
}

void CMdlPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	unsigned int i;
	BOOL sign = FALSE;	//��ʶ��ȷ��ͬһ������ֻ����һ����������߱�ѡ��
	//------------- ������� ---------------------------------------------------------
	//��Ϊi���޷���������������Զ����С���㣬��������ҪС��
	for (i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
	{
		if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
		{
			break;
		}
	}
	//��������ĳ�������Χ�ڣ��򽫴������Ϊѡ��
	if ( i > 0 )
	{
		g_pApp->m_miseModel.components[i - 1]->Display();
	}
}
void CMdlPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// û���°���ʱ��ļ�⣬�����ڲ�ͬλ����ʾ��ͬ���
	if ( (nFlags & MK_LBUTTON) == 0 && (nFlags & MK_LBUTTON) == 0)
	{
		m_action = NONE_ACTION;
		//------------ ���������ĳ�������ĳ������˿��ڣ�����Խ������߲��� ----------------------------
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
					m_action = CONN_COMP;	//��ʶ��ǰΪ���߲���
					return;
				}
			}
		}
		// ------------- �������ĳ��ģ��ı��߷�Χ�ڣ�����Խ��иı��С�Ĳ��� -----------------------------
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



	//---------------- �϶�������� ----------------------------------------------------
	if (m_action == SLCT_COMP)		//��ǰΪѡ������Ĳ���
	{
		//��ס���ʱ���϶�
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
	//----------------- end of �϶�������� --------------------------------------------

	//----------------- ���߲��� -------------------------------------------------------
	else if ( nFlags == MK_LBUTTON && m_action == CONN_COMP)
	{
		int break1x;		//��һ���յ��x����
		int break2y;		//�ڶ����յ��y����
		int break3x;		//�������յ��x���꣬���Ϊ-1�����޵������͵��ĸ��յ�
		BOOL need4breaks = FALSE;	//��ʶ�Ƿ���Ҫ�ĸ��յ�

		CDC* pDC;
		pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);	//�趨����ģʽ�����ڲ���ԭ��������

		//���Ȳ���ԭ����
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

		//���»���
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
	//------------ end of ���� --------------------------------------------------------------------
	
	//------------ �϶����ߵĲ��� -----------------------------------------------------------------
	else if ( nFlags == MK_LBUTTON && m_action == SLCT_LINE)
	{
		CDC* pDC;
		pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);	//�趨����ģʽ�����ڲ���ԭ��������
		unsigned int i;
		//ֻ�����һ����ѡ�е����ߣ������ҵ���������
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
			//���ֻ�������յ�
			if(g_pApp->m_miseModel.connlines[i].break2y == -1 )
			{
				//ֻ����Եڶ��ε��϶�
				if(g_pApp->m_miseModel.connlines[i].selectedPart == 2)
				{
					//����ԭ����
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//����break1x
					g_pApp->m_miseModel.connlines[i].break1x = point.x;

					//��������
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}
			}
			//������ĸ��յ�
			else
			{
				//����ڶ��ε��϶�
				if(g_pApp->m_miseModel.connlines[i].selectedPart == 2)
				{
					//����ԭ����
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//����break1x
					g_pApp->m_miseModel.connlines[i].break1x = point.x;

					//��������
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}

				//��������ε��϶�
				else if(g_pApp->m_miseModel.connlines[i].selectedPart == 3)
				{
					//����ԭ����
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//����break1x
					g_pApp->m_miseModel.connlines[i].break2y = point.y;

					//��������
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);
				}

				//������Ķε��϶�
				else if(g_pApp->m_miseModel.connlines[i].selectedPart == 4)
				{
					//����ԭ����
					pDC->MoveTo(g_pApp->m_miseModel.connlines[i].startPoint);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x,g_pApp->m_miseModel.connlines[i].startPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break1x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].break2y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].break3x, g_pApp->m_miseModel.connlines[i].endPoint.y);
					pDC->LineTo(g_pApp->m_miseModel.connlines[i].endPoint);

					//����break1x
					g_pApp->m_miseModel.connlines[i].break3x = point.x;

					//��������
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

	//------------ end of �϶����� ----------------------------------------------------------------

	//------------ �ı�ģ���С -------------------------------------------------------------------
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
	//------------- ������߲��� ---------------------------------
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
				//����ͷ�λ����ĳ��ģ�������˿ڷ�Χ��
				if(rectTmp.PtInRect(m_oldPoint))
				{
					//------------ �ж��Ƿ��Ѿ����������ӵ�������˿ڡ�ԭ����һ������˿�ֻ�ܽ���һ������ -------------------------------
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
						AfxMessageBox("����һ������˿�ֻ�ܽ���һ�����룡");
						this->Invalidate();
						return;
					}
					//------------ end of �ж� -------------------------------------------------------------------------------------------

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
					//����������ӵ�����������
					pConnLine->parent = &(g_pApp->m_miseModel);
					g_pApp->m_miseModel.connlines.push_back(*pConnLine);
					
				}
			}
		}
		this->Invalidate();
	}
	//------------- end of ���� ----------------------------------
	m_action = NONE_ACTION;
	CView::OnLButtonUp(nFlags, point);
}

void CMdlPanel::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//------------ �����ĳ������ϵ���Ҽ����򵯳����Բ˵� -----------------
	unsigned int i;
	//��Ϊi���޷���������������Զ����С���㣬��������ҪС��
	for (i = g_pApp->m_miseModel.components.size(); i > 0 ; i-- )
	{
		if ( g_pApp->m_miseModel.components[i - 1]->rect.PtInRect(point) )
		{
			break;
		}
	}
	//����������һ�����Χ��
	if ( i > 0 )
	{
		//���Ƚ����������Ϊδѡ��
		for (unsigned int j = g_pApp->m_miseModel.components.size(); j > 0 ; j-- )
		{
			g_pApp->m_miseModel.components[j - 1]->isSelected = FALSE;
		}
		//���������Ϊѡ�У��ҵ������Բ˵�
		g_pApp->m_miseModel.components[i - 1]->isSelected = TRUE;
		this->Invalidate();

		//��¼��ǰѡ����������ڶ���������
		m_selectedComponent = g_pApp->m_miseModel.components[i - 1];

		//�������Բ˵�
		CMenu menu_bar;
		menu_bar.LoadMenu(IDR_PopMenu);
		CMenu* file_menu = menu_bar.GetSubMenu(0);    
		ASSERT(file_menu);
		ClientToScreen(&point);
		file_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	//--------------- end of ���Բ˵� ----------------------------------

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
		//ɾ�����
		for (int i = 0; i < int(g_pApp->m_miseModel.components.size()); i++ )
		{
			if ( g_pApp->m_miseModel.components[i]->isSelected )
			{
				CString strTmp;
				strTmp = "ȷʵҪɾ�������";
				strTmp += g_pApp->m_miseModel.components[i]->name;
				strTmp += "��";
				if(AfxMessageBox(strTmp, MB_OKCANCEL) == IDOK)	//let the user affirm
				{
					g_pApp->m_miseModel.DelComponent(i);
					i--;
					this->Invalidate();
				}
			}
		}
		//ɾ������
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

//����ģ������ԣ��������ƺͶ�Ӧ��dll�ļ���λ��
void CMdlPanel::OnMenuComdef()
{
	m_selectedComponent->Display();
	
	//���½���
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
