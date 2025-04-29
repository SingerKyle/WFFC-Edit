#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <string>

class MFCMain;

// FileSelect dialog
class FileSelect : public CDialogEx
{
	DECLARE_DYNAMIC(FileSelect)

public:
	FileSelect(CWnd* pParent = nullptr); 
	virtual ~FileSelect();
	void SetObjectData(CEdit* targetEdit, const CString& allowedExtension);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_FILE };
#endif

protected:
	CEdit* m_TargetEdit;
	CString m_filePath, m_AllowedExtension;

	MFCMain* Main;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

	CString GetExecutableDirectory();

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_FileTree;
	HTREEITEM m_RootItem, m_TreeItem;

	CString GetParentDirectory();
	static UINT EnumerateFilesThread(LPVOID pParam);

	void PopulateTree(const CString& directory, HTREEITEM parentItem);

	void FindFile(CString rootPath, HTREEITEM rootItem);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void OnTvnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
};
