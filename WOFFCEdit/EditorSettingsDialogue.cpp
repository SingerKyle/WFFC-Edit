#include "EditorSettingsDialogue.h"
#include <fstream> 
#include <string> 
#include "MFCMain.h"

IMPLEMENT_DYNAMIC(EditorSettingsDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(EditorSettingsDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &EditorSettingsDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &EditorSettingsDialogue::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

EditorSettingsDialogue::EditorSettingsDialogue(CWnd* pParent) 
	: CDialogEx(IDD_EDITOR_SETTINGS, pParent)
{

}

EditorSettingsDialogue::~EditorSettingsDialogue()
{

}

void EditorSettingsDialogue::OnClose()
{
	if (m_main != nullptr)
	{
		m_main->OnDialogueBoxDestroyed();
	}

	DestroyWindow();

	CDialogEx::OnClose();
}

void EditorSettingsDialogue::SetMain(MFCMain* main)
{
	m_main = main;
}

void EditorSettingsDialogue::SetObjectData(ToolMain* toolMain)
{
	m_Tool = toolMain;

	SetInitialText(m_moveSpeed, m_Tool->GetCamera()->GetMoveSpeed());
	SetInitialText(m_camRotRate, m_Tool->GetCamera()->GetRotRate());
	m_camInvertControls.SetCheck(m_Tool->GetCamera()->GetIsInverted() ? BST_CHECKED : BST_UNCHECKED);
	//SetInitialText(m_resolutionX, m_Tool->GetWidth());
	//SetInitialText(m_resolutionY, m_Tool->GetHeight());
	SetLoadedValues();
}

void EditorSettingsDialogue::SetLoadedValues()
{
	camSettings.LoadFromFile(L"CameraSettings.txt");


	SetInitialText(m_moveSpeed, camSettings.moveSpeed);
	SetInitialText(m_camRotRate, camSettings.rotationSpeed);
	m_camInvertControls.SetCheck(camSettings.invertCamera ? BST_CHECKED : BST_UNCHECKED);

	// Update underlying tool as well
	m_Tool->UpdateCamValues(camSettings.moveSpeed, camSettings.rotationSpeed, camSettings.invertCamera);
}

void EditorSettingsDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_MOVESPEED, m_moveSpeed);
	DDX_Control(pDX, IDC_SETTING_ROTATESPEED, m_camRotRate);
	DDX_Control(pDX, IDC_INVERTCAM, m_camInvertControls);
	//DDX_Control(pDX, IDC_RESOLUTION_X, m_resolutionX);
	//DDX_Control(pDX, IDC_RESOLUTION_Y, m_resolutionY);
}

void EditorSettingsDialogue::SetInitialText(CEdit& box, float value)
{
	CString boxString;
	boxString.Format(_T("%f"), value);
	int dotPos = boxString.Find('.');
	if (dotPos != -1)
	{
		// Strip trailing zeros after the decimal point
		int lastNonZeroPos = boxString.GetLength() - 1;
		while (lastNonZeroPos > dotPos && boxString[lastNonZeroPos] == '0')
		{
			lastNonZeroPos--;
		}

		// If the last character is a decimal point, remove it too
		if (boxString[lastNonZeroPos] == '.')
		{
			boxString = boxString.Left(lastNonZeroPos);
		}
		else
		{
			boxString = boxString.Left(lastNonZeroPos + 1);
		}
	}
	box.SetWindowText(boxString);
}


BOOL EditorSettingsDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void EditorSettingsDialogue::OnBnClickedOk()
{
	// Get values user has entered into boxes - store in CString
	// Check they are valid
	// update camera

	CString moveSpeed, camRotRate, ResX, ResY;
	BOOL invertCamera;

	m_moveSpeed.GetWindowText(moveSpeed);
	m_camRotRate.GetWindowText(camRotRate);
	//m_resolutionX.GetWindowText(ResX);
	//m_resolutionX.GetWindowText(ResY);
	invertCamera = m_camInvertControls.GetCheck();

	if (_ttof(camRotRate) <= 0)
	{
		MessageBox(L"Camera Rotation is currently set to 0. Please change this so you can move the camera.", L"Warning: Camera Rotation", MB_OK);
	}
	else if (_ttof(moveSpeed) <= 0)
	{
		MessageBox(L"Camera Movement is currently set to 0. Please change this so you can move the camera.", L"Warning: Camera Movement", MB_OK);
	}
	/*else if (_ttoi(ResX) <= 0 && _ttoi(ResY) <= 0)
	{
		MessageBox(L"Resolution is set to 0. Please change this.", L"Warning: Resolution", MB_OK);
	}*/
	else
	{
		m_Tool->UpdateCamValues(_ttof(moveSpeed), _ttof(camRotRate), invertCamera);
		camSettings.UpdateCamValues(_ttof(moveSpeed), _ttof(camRotRate), invertCamera);
		camSettings.SaveToFile(L"CameraSettings.txt");
		//m_Tool->ResizeWindow(_ttoi(ResX), _ttoi(ResY));
		if (m_main != nullptr)
		{
			m_main->OnDialogueBoxDestroyed();
		}
		CDialogEx::OnOK();
		DestroyWindow();
	}

}

void EditorSettingsDialogue::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (m_main != nullptr)
	{
		m_main->OnDialogueBoxDestroyed();
	}
	CDialogEx::OnCancel();
	DestroyWindow();

}

bool FEditorSettings::LoadFromFile(const std::wstring& filename)
{
	std::wifstream file(filename);
	if (!file.is_open()) return false;

	file >> moveSpeed;
	file >> rotationSpeed;
	file >> invertCamera;
	return true;
}

bool FEditorSettings::SaveToFile(const std::wstring& filename)
{
	std::wofstream file(filename);
	if (!file.is_open()) return false;

	file << moveSpeed << L"\n"
		<< rotationSpeed << L"\n"
		<< invertCamera << L"\n";
	return true;
}

void FEditorSettings::UpdateCamValues(float m_moveSpeed, float m_rotationSpeed, bool m_invertCamera)
{
	moveSpeed = m_moveSpeed;
	rotationSpeed = m_rotationSpeed;
	invertCamera = m_invertCamera;
}
