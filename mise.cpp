// mise.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_SIMSTART, &CmiseApp::OnSimStart)
	ON_COMMAND(ID_SIMSETTING, &CmiseApp::OnSimSetting)
	ON_UPDATE_COMMAND_UI(ID_SIMSTART, &CmiseApp::OnUpdateSimstart)
END_MESSAGE_MAP()


// CmiseApp 构造

CmiseApp::CmiseApp()
{

	//获取应用程序所在的目录
	TCHAR exeFullPath[MAX_PATH];
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	CString strTmp = _T(exeFullPath);
	int index	= strTmp.ReverseFind('\\');
	m_appPath	= strTmp.Left(index);

	// 将所有重要的初始化放置在 InitInstance 中
	m_start_time	= 0.0;	//仿真开始时间
	m_end_time		= 3.0;	//仿真结束时间
	m_step_time		= 0.01;	//仿真步长

	//COM初始化
	CoInitialize(NULL);
}

CmiseApp::~CmiseApp()
{
	CoUninitialize();
}

// 唯一的一个 CmiseApp 对象

CmiseApp theApp;

// CmiseApp 初始化

BOOL CmiseApp::InitInstance()
{
    //增加的代码
	CSplashWnd *pStartWnd = new CSplashWnd();	//建立一个新窗口对象

	pStartWnd->CreateSplashWnd();				//创建窗口
	pStartWnd->CenterWindow();					//在屏幕中央
	pStartWnd->ShowWindow(SW_SHOW);				//显示窗口
	pStartWnd->UpdateWindow();					//更新窗口，激活OnPait函数
	Sleep(1500);								//等待函数指定秒钟
	if ( pStartWnd ) 
	{
		pStartWnd->SendMessage (WM_CLOSE); //关闭窗口
	}
    //代码结束

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	//LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CmiseDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CmiseView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CmiseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CmiseApp 消息处理程序


void CmiseApp::OnSimStart()
{
	// TODO: Add your command handler code here
	if ( !m_engine.Init(m_start_time, m_end_time, m_step_time) )
	{
		AfxMessageBox("仿真初始化失败");
		return;
	}
	if ( !m_engine.Run() )
	{
		AfxMessageBox("仿真运行失败");
		return;
	}

	if ( !m_engine.PostRun() )
	{
		AfxMessageBox("仿真后处理失败");
		return;
	}

//	((CMainFrame*)m_pMainWnd)->SetSimInfoOnStatusBar("仿真成功完成");
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
