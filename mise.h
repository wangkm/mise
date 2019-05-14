// mise.h : mise 应用程序的主头文件
//
#pragma once


#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号
#include "ComTree.h"
#include "MdlPanel.h"
#include "Model.h"
#include "MiseRTDB.h"
#include "MiseEngine.h"
// CmiseApp:
// 有关此类的实现，请参阅 mise.cpp
//

class CmiseApp : public CWinApp
{
public:
	CmiseApp();
	~CmiseApp();

//attributes
public:
	CComTree*	m_pComTree;
	CMdlPanel*	m_pMdlPanel;
	MiseModel	m_miseModel;	//Mise模型
	MiseRTDB	m_RTDB;			//实时数据库
	MiseEngine	m_engine;		//运行引擎

	//仿真时间的设定
	double		m_start_time;	//仿真开始时间
	double		m_end_time;		//仿真结束时间
	double		m_step_time;	//仿真步长

	CString		m_appPath;		//应用程序所在的目录


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSimStart();
	afx_msg void OnSimSetting();
public:
	afx_msg void OnUpdateSimstart(CCmdUI *pCmdUI);
};

extern CmiseApp theApp;
