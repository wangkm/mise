// SplashWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "SplashWnd.h"


// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	m_bitmap.LoadBitmap(IDB_STARTUP);//拷贝资源位图
	m_bitmap.GetBitmap(&bm);//得到位图结构中的大小信息 

}

CSplashWnd::~CSplashWnd()
{
}

BOOL CSplashWnd::CreateSplashWnd()
{
	return CreateEx(	0,
						AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW)),//注册类
						"ZeroMise",//窗口标题
						WS_POPUP,//窗口为弹出式
						0,0,bm.bmWidth,bm.bmHeight, //建立大小与位图大小相同的窗口
						NULL,
						NULL,
						NULL 
					);
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CSplashWnd message handlers



void CSplashWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	MemDC.CreateCompatibleDC(NULL);//建立一个和dc兼容的内存DC放置位图
	old_bitmap=MemDC.SelectObject(&m_bitmap);//将创建的位图选入内存DC

	//随机积木图像显示特技算法
	int stepx, stepy, dispnum, x, y; 
	int data[20][20];      //数组记录已显示过的数据组 

	for ( int i = 0; i<20; i++ ) 
	{
		for ( int j = 0; j<20; j++ ) 
		{
			data[i][j] = 0; 
		}
	}
	stepx=bm.bmWidth/20; 
	stepy=bm.bmHeight/20; 
	srand( (unsigned)time( NULL ) ); 
	dispnum=0; //记录已显示过的数据组的个数 
	while(1) { 
		x=rand() % 20; 
		y=rand() % 20; 
		if ( data[x][y] ) //如果为1，则已经显示了，跳出循环。
			continue; 
		data[x][y] = 1; //显示，设置为1
		dc.StretchBlt(x*stepx,	//目标设备逻辑横坐标
					  y*stepy,  //目标设备逻辑纵坐标
					  stepx,	//显示位图的像素宽
					  stepy,    //显示位图的像素高度 
					  &MemDC,   //位图内存设备对象 
					  x*stepx,  //位图的起始横
					  y*stepy,  //位图的起始纵坐标 
					  stepx,    //位图的像素宽
					  stepy,    //位图的像素高度 
					  SRCCOPY); 
		dispnum++; 
		if ( dispnum >= 400 ) 
			break; 
		Sleep(2); 
	}
	MemDC.SelectObject(old_bitmap);   
}
