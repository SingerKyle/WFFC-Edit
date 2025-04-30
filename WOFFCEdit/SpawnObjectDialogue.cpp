#include "SpawnObjectDialogue.h"

IMPLEMENT_DYNAMIC(SpawnObjectDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(SpawnObjectDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &SpawnObjectDialogue::OnBnClickedOk)
END_MESSAGE_MAP()

SpawnObjectDialogue::SpawnObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPAWNOBJECT, pParent),
	m_posX(0.0f), m_posY(0.0f), m_posZ(0.0f),
	m_rotX(0.0f), m_rotY(0.0f), m_rotZ(0.0f),
	m_scaX(1.0f), m_scaY(1.0f), m_scaZ(1.0f)
{
}

void SpawnObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_POS_X, m_editposX);
	DDX_Control(pDX, IDC_POS_Y, m_editposY);
	DDX_Control(pDX, IDC_POS_Z, m_editposZ);
								  
	DDX_Control(pDX, IDC_ROT_X, m_editrotX);
	DDX_Control(pDX, IDC_ROT_Y, m_editrotY);
	DDX_Control(pDX, IDC_ROT_Z, m_editrotZ);
								  
	DDX_Control(pDX, IDC_SCA_X, m_editscaX);
	DDX_Control(pDX, IDC_SCA_Y, m_editscaY);
	DDX_Control(pDX, IDC_SCA_Z, m_editscaZ);
}

BOOL SpawnObjectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set default values (as strings) in the edit boxes
	m_editposX.SetWindowTextW(L"0.0");
	m_editposY.SetWindowTextW(L"0.0");
	m_editposZ.SetWindowTextW(L"0.0");

	m_editrotX.SetWindowTextW(L"0.0");
	m_editrotY.SetWindowTextW(L"0.0");
	m_editrotZ.SetWindowTextW(L"0.0");

	m_editscaX.SetWindowTextW(L"1.0");
	m_editscaY.SetWindowTextW(L"1.0");
	m_editscaZ.SetWindowTextW(L"1.0");

	return TRUE;
}

float GetFloatFromEdit(CEdit& editCtrl)
{
	CString text;
	editCtrl.GetWindowText(text);
	return static_cast<float>(_ttof(text));  
}

void SpawnObjectDialogue::OnBnClickedOk()
{
	m_posX = GetFloatFromEdit(m_editposX);
	m_posY = GetFloatFromEdit(m_editposY);
	m_posZ = GetFloatFromEdit(m_editposZ);

	m_rotX = GetFloatFromEdit(m_editrotX);
	m_rotY = GetFloatFromEdit(m_editrotY);
	m_rotZ = GetFloatFromEdit(m_editrotZ);

	m_scaX = GetFloatFromEdit(m_editscaX);
	m_scaY = GetFloatFromEdit(m_editscaY);
	m_scaZ = GetFloatFromEdit(m_editscaZ);

	CDialogEx::OnOK();  // closes the dialog
}
