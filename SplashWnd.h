#pragma once


// CSplashWnd

class CSplashWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplashWnd)

public:
	CSplashWnd();
	virtual ~CSplashWnd();
	BOOL CreateSplashWnd();

public:
	CDC MemDC;            //�����ڴ�DC
	BITMAP bm;            //����λͼ�ṹ����
	CBitmap m_bitmap;     //����λͼ����
	CBitmap *old_bitmap;  //����λͼ����ָ��

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


