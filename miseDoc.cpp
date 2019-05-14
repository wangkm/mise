// miseDoc.cpp :  CmiseDoc ���ʵ��
//

#include "stdafx.h"
#include "mise.h"

#include "miseDoc.h"
#include "MyDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* CmiseDoc::szFiltersForSave = "��ѧ��ģ��(*.msm)|*.msm||";
char* CmiseDoc::szFiltersForOpen = "��ѧ��ģ��(*.msm)|*.msm||";
// CmiseDoc

IMPLEMENT_DYNCREATE(CmiseDoc, CDocument)

BEGIN_MESSAGE_MAP(CmiseDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CmiseDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CmiseDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, &CmiseDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CmiseDoc::OnFileNew)
END_MESSAGE_MAP()


// CmiseDoc ����/����

CmiseDoc::CmiseDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_filePathName = "";
}

CmiseDoc::~CmiseDoc()
{
}

BOOL CmiseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CmiseDoc ���л�

void CmiseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CmiseDoc ���

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


// CmiseDoc ����

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
	//�����ǰû�����ݣ����Լ�����
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

	//������Ҫ�û�ȷ���Ƿ񱣴浱ǰģ��
	int choice = AfxMessageBox("�Ƿ񱣴浱ǰģ�ͣ�", MB_YESNOCANCEL);
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
					AfxMessageBox("ģ�ͱ������");

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
				AfxMessageBox("ģ�ͱ������");

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

	int choice = AfxMessageBox("�Ƿ񱣴浱ǰģ�ͣ�", MB_YESNOCANCEL);
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
