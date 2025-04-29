#include "FileSelect.h"

IMPLEMENT_DYNAMIC(FileSelect, CDialogEx)

BEGIN_MESSAGE_MAP(FileSelect, CDialogEx)
	ON_BN_CLICKED(IDOK, &FileSelect::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FileSelect::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &FileSelect::OnTvnSelChanged)
END_MESSAGE_MAP()

FileSelect::FileSelect(CWnd* pParent) 
	: CDialogEx(IDD_SELECT_FILE, pParent)
{

}

FileSelect::~FileSelect()
{

}

void FileSelect::SetObjectData(CEdit* targetEdit, const CString& allowedExtension)
{
	m_TargetEdit = targetEdit;
	m_AllowedExtension = allowedExtension;

	if (!m_AllowedExtension.IsEmpty() && m_AllowedExtension[0] != '.')
		m_AllowedExtension.Insert(0, '.');
}

BOOL FileSelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RootItem = m_FileTree.InsertItem(L"Database");
	AfxBeginThread(&FileSelect::EnumerateFilesThread, this);

	return TRUE;
}

void FileSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_FileTree);
}

CString FileSelect::GetExecutableDirectory()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	CString fullPath(buffer);

	int lastSlash = fullPath.ReverseFind(L'\\');
	if (lastSlash != -1)
		fullPath = fullPath.Left(lastSlash + 1);

	return fullPath;
}

CString FileSelect::GetParentDirectory()
{
	CString exeDir = GetExecutableDirectory();

	int debugPos = exeDir.MakeLower().Find(L"debug");
	if (debugPos != -1)
		exeDir = exeDir.Left(debugPos);

	return exeDir + L"database\\";
}

UINT FileSelect::EnumerateFilesThread(LPVOID pParam)
{
	FileSelect* pThis = reinterpret_cast<FileSelect*>(pParam);

	if (pThis)
	{
		CString rootPath = pThis->GetParentDirectory();
		pThis->PopulateTree(rootPath, pThis->m_RootItem);
	}

	return 0;
}

void FileSelect::PopulateTree(const CString& directory, HTREEITEM parentItem)
{
	CFileFind finder;
	BOOL working = finder.FindFile(directory + L"*.*");

	while (working)
	{
		working = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		CString fileName = finder.GetFileName();
		CString fullPath = finder.GetFilePath();

		if (finder.IsDirectory())
		{
			HTREEITEM newItem = m_FileTree.InsertItem(fileName, parentItem);
			PopulateTree(fullPath + L"\\", newItem);
		}
		else
		{
			if (fileName.Right(m_AllowedExtension.GetLength()).CompareNoCase(m_AllowedExtension) == 0)
			{
				m_FileTree.InsertItem(fileName, parentItem);
			}
		}
	}
}

void FileSelect::OnBnClickedOk()
{
	if (m_filePath.IsEmpty() || m_TargetEdit == nullptr)
	{
		MessageBox(L"Please select a valid file.", L"Warning", MB_ICONWARNING);
		return;
	}

	m_TargetEdit->SetWindowText(m_filePath);
	CDialogEx::OnOK();
}

void FileSelect::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}

void FileSelect::OnTvnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM selectedItem = m_FileTree.GetSelectedItem();

	if (selectedItem == nullptr || selectedItem == m_RootItem)
	{
		m_filePath.Empty();
		*pResult = 0;
		return;
	}

	// build up path from root folder
	CString fullPath;
	while (selectedItem && selectedItem != m_RootItem)
	{
		CString itemText = m_FileTree.GetItemText(selectedItem);
		fullPath = itemText + L'\\' + fullPath;
		selectedItem = m_FileTree.GetParentItem(selectedItem);
	}

	CString databaseDir = GetParentDirectory();
	fullPath = databaseDir + fullPath;
	fullPath.TrimRight(L'\\');

	m_filePath = fullPath;

	*pResult = 0;
}
