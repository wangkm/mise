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
	CDC MemDC;            //创建内存DC
	BITMAP bm;            //创建位图结构变量
	CBitmap m_bitmap;     //创建位图对象
	CBitmap *old_bitmap;  //创建位图对象指针

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


