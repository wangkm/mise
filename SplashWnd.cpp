// SplashWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mise.h"
#include "SplashWnd.h"


// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	m_bitmap.LoadBitmap(IDB_STARTUP);//������Դλͼ
	m_bitmap.GetBitmap(&bm);//�õ�λͼ�ṹ�еĴ�С��Ϣ 

}

CSplashWnd::~CSplashWnd()
{
}

BOOL CSplashWnd::CreateSplashWnd()
{
	return CreateEx(	0,
						AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW)),//ע����
						"ZeroMise",//���ڱ���
						WS_POPUP,//����Ϊ����ʽ
						0,0,bm.bmWidth,bm.bmHeight, //������С��λͼ��С��ͬ�Ĵ���
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
	MemDC.CreateCompatibleDC(NULL);//����һ����dc���ݵ��ڴ�DC����λͼ
	old_bitmap=MemDC.SelectObject(&m_bitmap);//��������λͼѡ���ڴ�DC

	//�����ľͼ����ʾ�ؼ��㷨
	int stepx, stepy, dispnum, x, y; 
	int data[20][20];      //�����¼����ʾ���������� 

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
	dispnum=0; //��¼����ʾ����������ĸ��� 
	while(1) { 
		x=rand() % 20; 
		y=rand() % 20; 
		if ( data[x][y] ) //���Ϊ1�����Ѿ���ʾ�ˣ�����ѭ����
			continue; 
		data[x][y] = 1; //��ʾ������Ϊ1
		dc.StretchBlt(x*stepx,	//Ŀ���豸�߼�������
					  y*stepy,  //Ŀ���豸�߼�������
					  stepx,	//��ʾλͼ�����ؿ�
					  stepy,    //��ʾλͼ�����ظ߶� 
					  &MemDC,   //λͼ�ڴ��豸���� 
					  x*stepx,  //λͼ����ʼ��
					  y*stepy,  //λͼ����ʼ������ 
					  stepx,    //λͼ�����ؿ�
					  stepy,    //λͼ�����ظ߶� 
					  SRCCOPY); 
		dispnum++; 
		if ( dispnum >= 400 ) 
			break; 
		Sleep(2); 
	}
	MemDC.SelectObject(old_bitmap);   
}
