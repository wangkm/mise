// miseDoc.h :  CmiseDoc ��Ľӿ�
//


#pragma once

class CmiseDoc : public CDocument
{
protected: // �������л�����
	CmiseDoc();
	DECLARE_DYNCREATE(CmiseDoc)

// ����
public:
	CString m_filePathName;
	static char *szFiltersForSave;
	static char *szFiltersForOpen;

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CmiseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
public:
	afx_msg void OnFileSave();
public:
	afx_msg void OnFileSaveAs();
public:
	afx_msg void OnFileOpen();
public:
	afx_msg void OnFileNew();
};


