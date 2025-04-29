#include "stdafx.h"
#include "TransformsDialogue.h"
#include "MFCMain.h"
#include "Game.h"
#include "TransformCommand.h"


IMPLEMENT_DYNAMIC(TransformsDialogue, CDialogEx)

TransformsDialogue::TransformsDialogue(CWnd* pParent)
	: CDialogEx(IDD_TRANSFORMS_DIALOGUE, pParent)
{

}

TransformsDialogue::~TransformsDialogue()
{

}

void TransformsDialogue::SetMain(MFCMain* main)
{
	Main = main;
}

void TransformsDialogue::SetGame(Game* game)
{
	m_game = game;
}

void TransformsDialogue::SetSceneGraph(std::vector<SceneObject>* sceneGraph)
{
	m_sceneGraph = sceneGraph;
}

void TransformsDialogue::UpdateFromSelectedObject()
{
	if (!m_sceneGraph || !m_currentSelection || *m_currentSelection < 0) return;

	const auto& obj = m_sceneGraph->at(*m_currentSelection);
	float posX, posY, posZ;
	posX = obj.posX;
	posY = obj.posY;
	posZ = obj.posZ;
	float rotX, rotY, rotZ;
	rotX = obj.rotX;
	rotY = obj.rotY;
	rotZ = obj.rotZ;
	float scaleX, scaleY, scaleZ;
	scaleX = obj.scaX;
	scaleY = obj.scaY;
	scaleZ = obj.scaZ;

	std::wstring meshName(obj.model_path.begin(), obj.model_path.end());
	m_meshPath.SetWindowTextW(meshName.c_str());

	std::wstring textName(obj.tex_diffuse_path.begin(), obj.tex_diffuse_path.end());
	m_textPath.SetWindowTextW(textName.c_str());

	m_currentID.SetWindowTextW(std::to_wstring(*m_currentSelection).c_str());
	m_chunkID.SetWindowTextW(std::to_wstring(obj.chunk_ID).c_str());
	std::wstring wideName(obj.name.begin(), obj.name.end());
	m_objectName.SetWindowTextW(wideName.c_str());

	m_posX.SetWindowTextW(std::to_wstring(posX).c_str());
	m_posY.SetWindowTextW(std::to_wstring(posY).c_str());
	m_posZ.SetWindowTextW(std::to_wstring(posZ).c_str());

	// clamp between -360 and 360
	m_rotX.SetWindowTextW(std::to_wstring(rotX).c_str());
	m_rotY.SetWindowTextW(std::to_wstring(rotY).c_str());
	m_rotZ.SetWindowTextW(std::to_wstring(rotZ).c_str());

	m_scaleX.SetWindowTextW(std::to_wstring(scaleX).c_str());
	m_scaleY.SetWindowTextW(std::to_wstring(scaleY).c_str());
	m_scaleZ.SetWindowTextW(std::to_wstring(scaleZ).c_str());
}

void TransformsDialogue::End()
{
	Main->OnDialogueBoxDestroyed();

	DestroyWindow();	//destroy the window properly
}

void TransformsDialogue::OnClose()
{
	if (Main != nullptr)
	{
		Main->OnDialogueBoxDestroyed();  
	}

	DestroyWindow();

	CDialogEx::OnClose();
}

void TransformsDialogue::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	if (menu.LoadMenu(IDR_CONTEXT_MENU))  // Load the predefined menu
	{
		CMenu* pPopup = menu.GetSubMenu(0);  // Get the submenu (index 0)
		if (pPopup)
		{
			UINT nID = pWnd->GetDlgCtrlID();

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
}

BOOL TransformsDialogue::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (GetKeyState(VK_CONTROL) & 0x8000))
	{
		switch (pMsg->wParam)
		{
		case 'Z': OnUndo(); return TRUE;
		case 'Y': OnRedo(); return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void TransformsDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// model and texture paths
	DDX_Control(pDX, IDC_MESH_PATH, m_meshPath);
	DDX_Control(pDX, IDC_TEX_PATH, m_textPath);
	
	// Transforms

	DDX_Control(pDX, IDC_EDIT_POS_X, m_posX);
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_posY);
	DDX_Control(pDX, IDC_EDIT_POS_Z, m_posZ);

	DDX_Control(pDX, IDC_EDIT_ROT_X, m_rotX);
	DDX_Control(pDX, IDC_EDIT_ROT_Y, m_rotY);
	DDX_Control(pDX, IDC_EDIT_ROT_Z, m_rotZ);

	DDX_Control(pDX, IDC_EDIT_SCALE_X, m_scaleX);
	DDX_Control(pDX, IDC_EDIT_SCALE_Y, m_scaleY);
	DDX_Control(pDX, IDC_EDIT_SCALE_Z, m_scaleZ);

	// ID and name info
	DDX_Control(pDX, IDC_CURRENT, m_currentID);
	DDX_Control(pDX, IDC_CHUNKID, m_chunkID);
	DDX_Control(pDX, IDC_OBJECT_NAME, m_objectName);

	// buttons to add or remove
	DDX_Control(pDX, IDC_BTN_PLUS, m_plusButton);
	DDX_Control(pDX, IDC_BTN_MINUS, m_minusButton);


}

BOOL TransformsDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_FileSelectDialogue = new FileSelect();

	UpdateFromSelectedObject();
	return TRUE;
}

void TransformsDialogue::OnChangeTransform()
{
	ApplyChangesToObject();
}

void TransformsDialogue::OnIncrement(UINT nID)
{
	
}

void TransformsDialogue::ApplyChangesToObject()
{
	if (!m_sceneGraph || !m_currentSelection || *m_currentSelection < 0)
		return;

	SceneObject& obj = m_sceneGraph->at(*m_currentSelection);

	CString value;

	m_posX.GetWindowTextW(value); obj.posX = std::stof(std::wstring(value));
	m_posY.GetWindowTextW(value); obj.posY = std::stof(std::wstring(value));
	m_posZ.GetWindowTextW(value); obj.posZ = std::stof(std::wstring(value));

	m_rotX.GetWindowTextW(value); obj.rotX = std::stof(std::wstring(value));
	m_rotY.GetWindowTextW(value); obj.rotY = std::stof(std::wstring(value));
	m_rotZ.GetWindowTextW(value); obj.rotZ = std::stof(std::wstring(value));

	m_scaleX.GetWindowTextW(value); obj.scaX = std::stof(std::wstring(value));
	m_scaleY.GetWindowTextW(value); obj.scaY = std::stof(std::wstring(value));
	m_scaleZ.GetWindowTextW(value); obj.scaZ = std::stof(std::wstring(value));

	if (m_currentSelection != nullptr && m_game != nullptr)
	{
		m_game->BuildDisplayList(m_sceneGraph);
	}
}

void TransformsDialogue::InitialiseWithSelection()
{
	if (m_sceneGraph && !m_sceneGraph->empty())
	{
		//m_indexStorage = static_cast<int>(m_sceneGraph->size() - 1);
		//m_currentSelection = &m_indexStorage;
		UpdateFromSelectedObject();
	}
}

void TransformsDialogue::OnUndo()
{
	if (m_commandManager)
	{
		m_commandManager->Undo();
		UpdateFromSelectedObject(); 
		if (m_game != nullptr)
		{
			m_game->BuildDisplayList(m_sceneGraph);
		}
	}
}

void TransformsDialogue::OnRedo()
{
	if (m_commandManager)
	{
		m_commandManager->Redo();
		UpdateFromSelectedObject();  // Refresh the UI with the redone state

		if (m_game != nullptr)
		{
			m_game->BuildDisplayList(m_sceneGraph);
		}

	}
}

void TransformsDialogue::UpdateEditFields()
{
	UpdateFromSelectedObject();
}

BEGIN_MESSAGE_MAP(TransformsDialogue, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDOK, &TransformsDialogue::End)
	ON_BN_CLICKED(IDC_BTN_PLUS, &TransformsDialogue::OnOperationPlus)
	ON_BN_CLICKED(IDC_BTN_MINUS, &TransformsDialogue::OnOperationMinus)
	ON_BN_CLICKED(IDC_BTN_AXIS_X, &TransformsDialogue::OnAxisSelectedX)
	ON_BN_CLICKED(IDC_BTN_AXIS_Y, &TransformsDialogue::OnAxisSelectedY)
	ON_BN_CLICKED(IDC_BTN_AXIS_Z, &TransformsDialogue::OnAxisSelectedZ)
	ON_COMMAND_RANGE(IDC_BTN_POS_1, IDC_BTN_SCALE_100, &TransformsDialogue::OnTransformAmountClicked)
	ON_COMMAND(ID_UNDO, &TransformsDialogue::OnUndo)
	ON_COMMAND(ID_REDO, &TransformsDialogue::OnRedo)
	//ON_COMMAND(ID_RESET_TRANSFORMS, &TransformsDialogue::OnResetTransforms)
	ON_EN_CHANGE(IDC_EDIT_POS_X, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_POS_Z, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_ROT_X, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_ROT_Y, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_ROT_Z, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_SCALE_X, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_SCALE_Y, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_EDIT_SCALE_Z, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_OBJECT_NAME, &TransformsDialogue::OnEditChanged)
	ON_EN_CHANGE(IDC_CHUNKID, &TransformsDialogue::OnEditChanged)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MESH_SELECT, &TransformsDialogue::OnBnClickedMeshSelect)
	ON_BN_CLICKED(IDC_TEX_SELECT, &TransformsDialogue::OnBnClickedTexSelect)
END_MESSAGE_MAP()

void TransformsDialogue::OnOperationPlus()
{
	m_operation = Operation::Add;

	// Set the "Plus" button as selected
	m_plusButton.ModifyStyle(0x0, BS_FLAT);
	// Force the button to repaint
	m_plusButton.Invalidate();  
	// Ensure it updates immediately
	m_plusButton.UpdateWindow();  

	// Set the "Minus" button back to the default
	m_minusButton.ModifyStyle(BS_FLAT, BS_PUSHBUTTON);
	m_minusButton.Invalidate();
	m_minusButton.UpdateWindow();  
}

void TransformsDialogue::OnOperationMinus()
{
	m_operation = Operation::Subtract;

	// Set the "Minus" button as selected
	m_minusButton.ModifyStyle(0x0, BS_FLAT);
	// Force the button to repaint
	m_minusButton.Invalidate(); 
	// Ensure it updates immediately
	m_minusButton.UpdateWindow();

	// Set the Plus button back to the default
	m_plusButton.ModifyStyle(BS_FLAT, BS_PUSHBUTTON);
	
	m_plusButton.Invalidate();  
	
	m_plusButton.UpdateWindow();  
}

void TransformsDialogue::OnAxisSelectedX()
{
	m_selectedAxis = Axis::X;
}

void TransformsDialogue::OnAxisSelectedY()
{
	m_selectedAxis = Axis::Y;
}

void TransformsDialogue::OnAxisSelectedZ()
{
	m_selectedAxis = Axis::Z;
}

void TransformsDialogue::OnTransformAmountClicked(UINT nID)
{
	if (!m_sceneGraph || !m_currentSelection || *m_currentSelection < 0)
		return;

	float amount = 0.0f;
	switch (nID)
	{
	case IDC_BTN_POS_1:
		m_transforms = Transform::Position;
		amount = 1.0f;
		break;
	case IDC_BTN_ROT_1:
		m_transforms = Transform::Rotation;
		amount = 1.0f;
		break;
	case IDC_BTN_SCALE_1:
		m_transforms = Transform::Scale;
		amount = 1.0f; 
		break;
	case IDC_BTN_POS_10:  
		m_transforms = Transform::Position;
		amount = 10.0f;
		break;
	case IDC_BTN_ROT_10:
		m_transforms = Transform::Rotation;
		amount = 10.0f;
		break;
	case IDC_BTN_SCALE_10:
		m_transforms = Transform::Scale;
		amount = 10.0f;
		break;
	case IDC_BTN_POS_50:
		m_transforms = Transform::Position;
		amount = 50.0f;
		break;
	case IDC_BTN_ROT_50:
		m_transforms = Transform::Rotation;
		amount = 50.0f;
		break;
	case IDC_BTN_SCALE_50:
		m_transforms = Transform::Scale;
		amount = 50.0f;
		break;
	case IDC_BTN_POS_100:
		Transform::Position;
		amount = 100.0f;
		break;
	case IDC_BTN_ROT_100:
		Transform::Rotation;
		amount = 100.0f;
		break;
	case IDC_BTN_SCALE_100:
		Transform::Scale;
		amount = 100.0f;
		break;
	}

	if (m_operation == Operation::Subtract)
		amount = -amount;

	SceneObject& obj = m_sceneGraph->at(*m_currentSelection);

	TransformData oldData = {
		obj.posX, obj.posY, obj.posZ,
		obj.rotX, obj.rotY, obj.rotZ,
		obj.scaX, obj.scaY, obj.scaZ
	};

	switch (m_selectedAxis)
	{
	case Axis::X:
		switch (m_transforms)
		{
		case Transform::Position:
			obj.posX += amount;
			break;
		case Transform::Rotation:
			obj.rotX += amount;
			break;
		case Transform::Scale:
			obj.scaX += amount;
			break;
		}
		break;
	case Axis::Y:
		switch (m_transforms)
		{
		case Transform::Position:
			obj.posY += amount;
			break;
		case Transform::Rotation:
			obj.rotY += amount;
			break;
		case Transform::Scale:
			obj.scaY += amount;
			break;
		}
		break;
	case Axis::Z: 
		switch (m_transforms)
		{
		case Transform::Position:
			obj.posZ += amount;
			break;
		case Transform::Rotation:
			obj.rotZ += amount;
			break;
		case Transform::Scale:
			obj.scaZ += amount;
			break;
		}
		break;
	}

	TransformData newData = {
		obj.posX, obj.posY, obj.posZ,
		obj.rotX, obj.rotY, obj.rotZ,
		obj.scaX, obj.scaY, obj.scaZ
	};

	if (m_commandManager)
	{
		m_commandManager->ExecuteCommand(new TransformCommand(&obj, oldData, newData));
	}

	UpdateFromSelectedObject(); // Refresh the UI
	OnChangeTransform();
}

void TransformsDialogue::OnEditChanged()
{
	HWND hWndCtrl = ::GetFocus(); // Get which edit box is currently focused
	if (hWndCtrl)
	{
		UINT nCtrlID = ::GetDlgCtrlID(hWndCtrl); // Now you have the control ID

		CString value;
		SceneObject& obj = m_sceneGraph->at(*m_currentSelection);

		switch (nCtrlID)
		{
		case IDC_EDIT_POS_X:
			m_posX.GetWindowTextW(value);
			obj.posX = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_POS_Y:
			m_posY.GetWindowTextW(value);
			obj.posY = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_POS_Z:
			m_posZ.GetWindowTextW(value);
			obj.posZ = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_ROT_X:
			m_rotX.GetWindowTextW(value);
			obj.rotX = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_ROT_Y:
			m_rotY.GetWindowTextW(value);
			obj.rotY = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_ROT_Z:
			m_rotZ.GetWindowTextW(value);
			obj.rotZ = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_SCALE_X:
			m_scaleX.GetWindowTextW(value);
			obj.scaX = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_SCALE_Y:
			m_scaleY.GetWindowTextW(value);
			obj.scaY = std::stof(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_EDIT_SCALE_Z:
			m_scaleZ.GetWindowTextW(value);
			obj.scaZ = std::stof(std::wstring(value));
			OnChangeTransform();
			break;

		case IDC_OBJECT_NAME:
		{
			m_objectName.GetWindowTextW(value);
			std::wstring wstr(value);
			obj.name = std::string(wstr.begin(), wstr.end());
			OnChangeTransform();
			break;
		}
		case IDC_MESH_PATH:
			m_meshPath.GetWindowTextW(value);
			obj.model_path = std::stoi(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_TEX_PATH:
			m_textPath.GetWindowTextW(value);
			obj.tex_diffuse_path = std::stoi(std::wstring(value));
			OnChangeTransform();
			break;
		case IDC_CHUNKID:
			m_chunkID.GetWindowTextW(value);
			obj.chunk_ID = std::stoi(std::wstring(value));
			OnChangeTransform();
			break;
		default:
			break;
		}

		
	}
}



void TransformsDialogue::OnBnClickedMeshSelect()
{
	if (m_FileSelectDialogue) 
	{
		if (IsWindow(m_FileSelectDialogue->GetSafeHwnd())) 
		{
			m_FileSelectDialogue->DestroyWindow();
		}

		m_FileSelectDialogue->Create(IDD_SELECT_FILE);
		m_FileSelectDialogue->ShowWindow(SW_SHOW);

		m_FileSelectDialogue->SetObjectData(&m_meshPath, L".cmo");
	}
}


void TransformsDialogue::OnBnClickedTexSelect()
{
	if (m_FileSelectDialogue) 
	{

		if (IsWindow(m_FileSelectDialogue->GetSafeHwnd()))
		{
			m_FileSelectDialogue->DestroyWindow();
		}

		m_FileSelectDialogue->Create(IDD_SELECT_FILE);
		m_FileSelectDialogue->ShowWindow(SW_SHOW);

		m_FileSelectDialogue->SetObjectData(&m_textPath, L".dds");
	}
}
