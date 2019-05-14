// mise.h : mise Ӧ�ó������ͷ�ļ�
//
#pragma once


#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������
#include "ComTree.h"
#include "MdlPanel.h"
#include "Model.h"
#include "MiseRTDB.h"
#include "MiseEngine.h"
// CmiseApp:
// �йش����ʵ�֣������ mise.cpp
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
	MiseModel	m_miseModel;	//Miseģ��
	MiseRTDB	m_RTDB;			//ʵʱ���ݿ�
	MiseEngine	m_engine;		//��������

	//����ʱ����趨
	double		m_start_time;	//���濪ʼʱ��
	double		m_end_time;		//�������ʱ��
	double		m_step_time;	//���沽��

	CString		m_appPath;		//Ӧ�ó������ڵ�Ŀ¼


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSimStart();
	afx_msg void OnSimSetting();
public:
	afx_msg void OnUpdateSimstart(CCmdUI *pCmdUI);
};

extern CmiseApp theApp;
