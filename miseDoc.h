// miseDoc.h :  CmiseDoc 类的接口
//


#pragma once

class CmiseDoc : public CDocument
{
protected: // 仅从序列化创建
	CmiseDoc();
	DECLARE_DYNCREATE(CmiseDoc)

// 属性
public:
	CString m_filePathName;
	static char *szFiltersForSave;
	static char *szFiltersForOpen;

// 操作
public:

// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CmiseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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


