// SimMonitorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "SimMonitorDialog.h"
#include ".\simmonitordialog.h"
#include "math.h"

#define SIMLINE_OFFSET_X	30
#define SIMLINE_OFFSET_Y	25


// CSimMonitorDialog dialog

IMPLEMENT_DYNAMIC(CSimMonitorDialog, CDialog)
CSimMonitorDialog::CSimMonitorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSimMonitorDialog::IDD, pParent)
{
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

CSimMonitorDialog::~CSimMonitorDialog()
{
}

void CSimMonitorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimMonitorDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSimMonitorDialog message handlers

void CSimMonitorDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	dc.SetBkMode(TRANSPARENT);
	if ( m_pSimTime->size() == 0 )
	{
		dc.TextOutA(10, 10, "没有仿真数据");
		return;
	}
	CString strTmp;
	CRect rect;
	GetClientRect(&rect);

	//客户区的高度和宽度
	int height = rect.Height();
	int width = rect.Width();

	//纯绘制曲线区域的高度和宽度
	int height2 = height - 2 * SIMLINE_OFFSET_Y;
	int width2	= width - 2 * SIMLINE_OFFSET_X;

	dc.Rectangle(SIMLINE_OFFSET_X, SIMLINE_OFFSET_Y, width - SIMLINE_OFFSET_X, height - SIMLINE_OFFSET_Y);

	//创建新的字体
	CFont font;
	VERIFY(font.CreateFontIndirect(&m_mylf));
	HFONT hFont = (HFONT)font;
	HFONT hOldFont = (HFONT)::SelectObject(dc.m_hDC, hFont);

	CRect nameRect;

	//显示时间轴
	strTmp.Format("%f", (*m_pSimTime)[0]);
	DelEndZeroFromStr(strTmp);
	strTmp += "s";
	nameRect.top	= height - SIMLINE_OFFSET_Y + 4;
	nameRect.bottom	= nameRect.top + 14;
	nameRect.left	= SIMLINE_OFFSET_X - 30;
	nameRect.right	= SIMLINE_OFFSET_X + 30;
	dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_BOTTOM | DT_CENTER);

	strTmp.Format("%f", (*m_pSimTime)[m_pSimTime->size() - 1]);
	DelEndZeroFromStr(strTmp);
	strTmp += "s";
	nameRect.top	= height - SIMLINE_OFFSET_Y + 4;
	nameRect.bottom	= nameRect.top + 14;
	nameRect.left	= width - SIMLINE_OFFSET_X - 30;
	nameRect.right	= width - SIMLINE_OFFSET_X + 30;
	dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_BOTTOM | DT_CENTER);

	//其他时间轴，便于观察
	double timeScale = floor(log10(g_pApp->m_end_time - g_pApp->m_start_time));
	for ( double sign = pow(10.0, timeScale); sign < g_pApp->m_end_time; sign += pow(10.0, timeScale))
	{
		double x = width2 * ( sign - g_pApp->m_start_time) / ( g_pApp->m_end_time - g_pApp->m_start_time ) + SIMLINE_OFFSET_X;

		dc.MoveTo(x, SIMLINE_OFFSET_Y);
		dc.LineTo(x, height -SIMLINE_OFFSET_Y);

		strTmp.Format("%f", sign);
		DelEndZeroFromStr(strTmp);
		strTmp += "s";
		nameRect.top	= height - SIMLINE_OFFSET_Y + 4;
		nameRect.bottom	= nameRect.top + 14;
		nameRect.left	= x - 30;
		nameRect.right	= x + 30;
		dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_BOTTOM | DT_CENTER);
	}
	for ( double sign = pow(10.0, timeScale); sign < -g_pApp->m_start_time; sign += pow(10.0, timeScale))
	{
		double x = width2 * ( (-sign) - g_pApp->m_start_time) / ( g_pApp->m_end_time - g_pApp->m_start_time ) + SIMLINE_OFFSET_X;

		dc.MoveTo(x, SIMLINE_OFFSET_Y);
		dc.LineTo(x, height -SIMLINE_OFFSET_Y);

		strTmp.Format("%f", -sign);
		DelEndZeroFromStr(strTmp);
		strTmp += "s";
		nameRect.top	= height - SIMLINE_OFFSET_Y + 4;
		nameRect.bottom	= nameRect.top + 14;
		nameRect.left	= x - 30;
		nameRect.right	= x + 30;
		dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_BOTTOM | DT_CENTER);
	}
	//画x＝0坐标轴
	if ( g_pApp->m_start_time <= 0 && g_pApp->m_end_time > 0 )
	{
		double x = width2 * -g_pApp->m_start_time / ( g_pApp->m_end_time - g_pApp->m_start_time ) + SIMLINE_OFFSET_X;;
		dc.MoveTo(x, SIMLINE_OFFSET_Y);
		dc.LineTo(x, height -SIMLINE_OFFSET_Y);

		strTmp.Format("%f", 0.0);
		DelEndZeroFromStr(strTmp);
		strTmp += "s";
		nameRect.top	= height - SIMLINE_OFFSET_Y + 4;
		nameRect.bottom	= nameRect.top + 14;
		nameRect.left	= x - 30;
		nameRect.right	= x + 30;
		dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_BOTTOM | DT_CENTER);
	}


	//显示数值轴
	strTmp.Format("%f", *m_pBottom);
	DelEndZeroFromStr(strTmp);
	nameRect.top	= height - SIMLINE_OFFSET_Y - 5;
	nameRect.bottom	= height - SIMLINE_OFFSET_Y + 5;
	nameRect.left	= SIMLINE_OFFSET_X - 60;
	nameRect.right	= SIMLINE_OFFSET_X - 5;
	dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_VCENTER | DT_RIGHT);

	strTmp.Format("%f", *m_pTop);
	DelEndZeroFromStr(strTmp);
	nameRect.top	= SIMLINE_OFFSET_Y - 5;
	nameRect.bottom	= SIMLINE_OFFSET_Y + 5;
	nameRect.left	= SIMLINE_OFFSET_X - 60;
	nameRect.right	= SIMLINE_OFFSET_X - 5;
	dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_VCENTER | DT_RIGHT);

	// 其他数值轴，便于观察曲线
	double valueScale = *m_pScaleUp >= *m_pScaleDown ? *m_pScaleUp : *m_pScaleDown;
	for ( double sign = pow(10.0, valueScale); sign <= *m_pTop; sign += pow(10.0, valueScale))
	{
		double y = (double)height2 - (double)( height2 * ( sign - *m_pBottom) ) / (double)( *m_pTop - *m_pBottom ) + SIMLINE_OFFSET_Y;
		dc.MoveTo(SIMLINE_OFFSET_X, y);
		dc.LineTo(width - SIMLINE_OFFSET_X, y);

		strTmp.Format("%f", sign);
		DelEndZeroFromStr(strTmp);
		nameRect.top	= y - 5;
		nameRect.bottom	= y + 5;
		nameRect.left	= SIMLINE_OFFSET_X - 60;
		nameRect.right	= SIMLINE_OFFSET_X - 5;
		dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_VCENTER | DT_RIGHT);
	}
	for ( double sign = pow(10.0, valueScale); sign <= -*m_pBottom; sign += pow(10.0, valueScale))
	{
		double y = (double)height2 - (double)( height2 * ( (-sign) - *m_pBottom) ) / (double)( *m_pTop - *m_pBottom ) + SIMLINE_OFFSET_Y;
		dc.MoveTo(SIMLINE_OFFSET_X, y);
		dc.LineTo(width - SIMLINE_OFFSET_X, y);

		strTmp.Format("%f", (-sign));
		DelEndZeroFromStr(strTmp);
		nameRect.top	= y - 5;
		nameRect.bottom	= y + 5;
		nameRect.left	= SIMLINE_OFFSET_X - 60;
		nameRect.right	= SIMLINE_OFFSET_X - 5;
		dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_VCENTER | DT_RIGHT);
	}
	
	//画y＝0坐标轴
	if (fabs( *m_pTop ) > 1E-100 && fabs( *m_pBottom ) > 1E-100 )
	{
		double y = (double)height2 + (double)( height2 *  *m_pBottom) / (double)( *m_pTop - *m_pBottom ) + SIMLINE_OFFSET_Y;
		dc.MoveTo(SIMLINE_OFFSET_X, y);
		dc.LineTo(width - SIMLINE_OFFSET_X, y);

		strTmp.Format("%f", 0.0);
		DelEndZeroFromStr(strTmp);
		nameRect.top	= y - 5;
		nameRect.bottom	= y + 5;
		nameRect.left	= SIMLINE_OFFSET_X - 60;
		nameRect.right	= SIMLINE_OFFSET_X - 5;
		dc.DrawText(strTmp, strTmp.GetLength(), &nameRect, DT_VCENTER | DT_RIGHT);
	}


	//画仿真曲线
	CPen newPen, *oldPen;
	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	oldPen = dc.SelectObject(&newPen);

	for(unsigned int i = 0; i < m_pVarValues->size(); i++)
	{
		double x = SIMLINE_OFFSET_X + width2 / (*m_pVarValues)[i].size();;
		double y = (double)height2 - (double)( height2 * ( (*m_pVarValues)[i][1] - *m_pBottom) ) / (double)( *m_pTop - *m_pBottom ) + SIMLINE_OFFSET_Y;

		dc.MoveTo(x, y);

		for(unsigned int j = 1; j < (*m_pVarValues)[i].size(); j++)
		{
			double x = SIMLINE_OFFSET_X + ( j ) * width2 / (*m_pVarValues)[i].size();
			double y = (double)height2 - (double)( height2 * ( (*m_pVarValues)[i][j] - *m_pBottom) ) / (double)( *m_pTop - *m_pBottom ) + SIMLINE_OFFSET_Y;
			dc.LineTo(x, y);
		}
	}
	dc.SelectObject(oldPen);
	newPen.DeleteObject();

	//恢复字体
	::SelectObject(dc.m_hDC, hOldFont);	//restore old font
	::DeleteObject(hFont);
}

void CSimMonitorDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	this->Invalidate();
}
