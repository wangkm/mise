// mise.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "mise.h"
#include "MainFrm.h"

#include "miseDoc.h"
#include "miseView.h"

#include "SimSettingDialog.h"
#include "SplashWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmiseApp

BEGIN_MESSAGE_MAP(CmiseApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_SIMSTART, &CmiseApp::OnSimStart)
	ON_COMMAND(ID_SIMSETTING, &CmiseApp::OnSimSetting)
	ON_UPDATE_COMMAND_UI(ID_SIMSTART, &CmiseApp::OnUpdateSimstart)
END_MESSAGE_MAP()


// CmiseApp ����

CmiseApp::CmiseApp()
{

	//��ȡӦ�ó������ڵ�Ŀ¼
	TCHAR exeFullPath[MAX_PATH];
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	CString strTmp = _T(exeFullPath);
	int index	= strTmp.ReverseFind('\\');
	m_appPath	= strTmp.Left(index);

	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_start_time	= 0.0;	//���濪ʼʱ��
	m_end_time		= 3.0;	//�������ʱ��
	m_step_time		= 0.01;	//���沽��

	//COM��ʼ��
	CoInitialize(NULL);
}

CmiseApp::~CmiseApp()
{
	CoUninitialize();
}

// Ψһ��һ�� CmiseApp ����

CmiseApp theApp;

// CmiseApp ��ʼ��

BOOL CmiseApp::InitInstance()
{
    //���ӵĴ���
	CSplashWnd *pStartWnd = new CSplashWnd();	//����һ���´��ڶ���

	pStartWnd->CreateSplashWnd();				//��������
	pStartWnd->CenterWindow();					//����Ļ����
	pStartWnd->ShowWindow(SW_SHOW);				//��ʾ����
	pStartWnd->UpdateWindow();					//���´��ڣ�����OnPait����
	Sleep(1500);								//�ȴ�����ָ������
	if ( pStartWnd ) 
	{
		pStartWnd->SendMessage (WM_CLOSE); //�رմ���
	}
    //�������

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	//LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CmiseDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CmiseView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CmiseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CmiseApp ��Ϣ�������


void CmiseApp::OnSimStart()
{
	// TODO: Add your command handler code here
	if ( !m_engine.Init(m_start_time, m_end_time, m_step_time) )
	{
		AfxMessageBox("�����ʼ��ʧ��");
		return;
	}
	if ( !m_engine.Run() )
	{
		AfxMessageBox("��������ʧ��");
		return;
	}

	if ( !m_engine.PostRun() )
	{
		AfxMessageBox("�������ʧ��");
		return;
	}

//	((CMainFrame*)m_pMainWnd)->SetSimInfoOnStatusBar("����ɹ����");
}

void CmiseApp::OnSimSetting()
{
	// TODO: Add your command handler code here
	CSimSettingDialog dlgSimSetting(m_start_time, m_end_time, m_step_time);
	if ( dlgSimSetting.DoModal() == IDOK )
	{
		m_start_time	= dlgSimSetting.m_startTime;
		m_end_time		= dlgSimSetting.m_endTime;
		m_step_time		= dlgSimSetting.m_stepTime;
	}
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CmiseApp::OnUpdateSimstart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_miseModel.components.size() == 0)
	{
		pCmdUI->Enable(false);
	}
}
