// miseDoc.cpp :  CmiseDoc 类的实现
//

#include "stdafx.h"
#include "mise.h"

#include "miseDoc.h"
#include "MyDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* CmiseDoc::szFiltersForSave = "多学科模型(*.msm)|*.msm||";
char* CmiseDoc::szFiltersForOpen = "多学科模型(*.msm)|*.msm||";
// CmiseDoc

IMPLEMENT_DYNCREATE(CmiseDoc, CDocument)

BEGIN_MESSAGE_MAP(CmiseDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CmiseDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CmiseDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, &CmiseDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CmiseDoc::OnFileNew)
END_MESSAGE_MAP()


// CmiseDoc 构造/析构

CmiseDoc::CmiseDoc()
{
	// TODO: 在此添加一次性构造代码
	m_filePathName = "";
}

CmiseDoc::~CmiseDoc()
{
}

BOOL CmiseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CmiseDoc 序列化

void CmiseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CmiseDoc 诊断

#ifdef _DEBUG
void CmiseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CmiseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CmiseDoc 命令

void CmiseDoc::OnFileSave()
{
	if ( g_pApp->m_miseModel.components.size() == 0 )
	{
		return;
	}

	if ( m_filePathName == "" )
	{
		CFileDialog dlgFile(FALSE, "msm", "*.msm", OFN_OVERWRITEPROMPT, szFiltersForSave);
		if(dlgFile.DoModal() == IDCANCEL)
		{
			return;
		}
		m_filePathName = dlgFile.GetPathName();
	}
	OnSaveDocument(m_filePathName);
}

void CmiseDoc::OnFileSaveAs()
{
	CFileDialog dlgFile(FALSE, "msm", this->m_strPathName, OFN_OVERWRITEPROMPT, szFiltersForSave);
	if(dlgFile.DoModal() == IDCANCEL)
		return;

	m_filePathName = dlgFile.GetPathName();
	OnSaveDocument(m_filePathName);
}

BOOL CmiseDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	g_pApp->m_miseModel.Save(lpszPathName);	
	return TRUE;
}


void CmiseDoc::OnFileOpen()
{
	//如果当前没有数据，则自己加载
	if ( g_pApp->m_miseModel.components.size() == 0 )
	{
		g_pApp->m_miseModel.Clear();
		CFileDialog dlgFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFiltersForOpen);
		if(dlgFile.DoModal() == IDCANCEL)
			return;

		m_filePathName = dlgFile.GetPathName();
		g_pApp->m_miseModel.Load(m_filePathName);
		g_pApp->m_pMdlPanel->Invalidate();
		return;
	}

	//否则需要用户确认是否保存当前模型
	int choice = AfxMessageBox("是否保存当前模型？", MB_YESNOCANCEL);
	switch (choice)
	{
	case(IDYES):
		{
			if ( m_filePathName == "" )
			{
				CFileDialog dlgFile(FALSE, "msm", "*.msm", OFN_OVERWRITEPROMPT, szFiltersForSave);
				if(dlgFile.DoModal() == IDOK)
				{
					m_filePathName = dlgFile.GetPathName();
					OnSaveDocument(m_filePathName);
					AfxMessageBox("模型保存完毕");

					g_pApp->m_miseModel.Clear();
					CFileDialog dlgFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFiltersForOpen);
					if(dlgFile.DoModal() == IDCANCEL)
						return;

					m_filePathName = dlgFile.GetPathName();
					g_pApp->m_miseModel.Load(m_filePathName);
					g_pApp->m_pMdlPanel->Invalidate();
					return;
				}
			}
			else
			{
				OnSaveDocument(m_filePathName);
				AfxMessageBox("模型保存完毕");

				g_pApp->m_miseModel.Clear();
				CFileDialog dlgFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFiltersForOpen);
				if(dlgFile.DoModal() == IDCANCEL)
					return;

				m_filePathName = dlgFile.GetPathName();
				g_pApp->m_miseModel.Load(m_filePathName);
				g_pApp->m_pMdlPanel->Invalidate();
				return;
			}
			break;
		}
	case(IDNO):
		{
			g_pApp->m_miseModel.Clear();
			CFileDialog dlgFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFiltersForOpen);
			if(dlgFile.DoModal() == IDCANCEL)
				return;

			m_filePathName = dlgFile.GetPathName();
			g_pApp->m_miseModel.Load(m_filePathName);
			g_pApp->m_pMdlPanel->Invalidate();
			return;
		}
	case(IDCANCEL):
		break;
	default:
		break;
	}
}

void CmiseDoc::OnFileNew()
{
	// TODO: Add your command handler code here
	if ( g_pApp->m_miseModel.components.size() == 0 )
	{
		return;
	}

	int choice = AfxMessageBox("是否保存当前模型？", MB_YESNOCANCEL);
	switch (choice)
	{
	case(IDYES):
		{
			if ( m_filePathName == "" )
			{
				CFileDialog dlgFile(FALSE, "msm", "*.msm", OFN_OVERWRITEPROMPT, szFiltersForSave);
				if(dlgFile.DoModal() == IDOK)
				{
					m_filePathName = dlgFile.GetPathName();
					OnSaveDocument(m_filePathName);
					m_filePathName = "";
					g_pApp->m_miseModel.Clear();
					g_pApp->m_pMdlPanel->Invalidate();
				}
			}
			else
			{
				OnSaveDocument(m_filePathName);
				m_filePathName = "";
				g_pApp->m_miseModel.Clear();
				g_pApp->m_pMdlPanel->Invalidate();
			}
			break;
		}
	case(IDNO):
		{
			m_filePathName = "";
			g_pApp->m_miseModel.Clear();
			g_pApp->m_pMdlPanel->Invalidate();
			break;
		}
	case(IDCANCEL):
		break;
	default:
		break;
	}
}
