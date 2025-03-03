
#include "stdafx.h"
#include "ObjectEditDialogue.h"

IMPLEMENT_DYNAMIC(ObjectEditDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(ObjectEditDialogue, CDialogEx)
	ON_COMMAND(IDOK, &ObjectEditDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &ObjectEditDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_OBJECT_SELECT, &ObjectEditDialogue::OnCbnSelchangeObjectSelect)
END_MESSAGE_MAP()

ObjectEditDialogue::ObjectEditDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)
	: CDialogEx(IDD_OBJECTTRANSFORM, pParent)
{
	m_sceneGraph = SceneGraph;
}

ObjectEditDialogue::ObjectEditDialogue(CWnd* pParent)
	: CDialogEx(IDD_OBJECTTRANSFORM, pParent)
{

}

ObjectEditDialogue::~ObjectEditDialogue()
{

}

void ObjectEditDialogue::GrabCurrentSelectedObject(std::vector<SceneObject>* SceneGraph)
{
	m_sceneGraph = SceneGraph;

	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (int i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.
		std::wstring ComboBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_comboBox.AddString(ComboBoxEntry.c_str());
	}
}

void ObjectEditDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_SELECT, m_comboBox);
}

void ObjectEditDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void ObjectEditDialogue::Select()
{
	int index = m_comboBox.GetCurSel();
	CString currentSelectionValue;

	//m_comboBox.GetText(index, currentSelectionValue);
	int selectedValue = _ttoi(currentSelectionValue);

	m_currentSelection = selectedValue;

	// Using selected item, update other boxes
}

BOOL ObjectEditDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ObjectEditDialogue::PostNcDestroy()
{

}

void ObjectEditDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void ObjectEditDialogue::OnCbnSelchangeObjectSelect()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_sceneGraph->size(); i++)
	{
		if (m_sceneGraph->at(i).ID == m_currentSelection)
		{
			m_PositionX.SetWindowTextW(L"3");
			m_PositionY.SetWindowTextW(L"2");
			m_PositionZ.SetWindowTextW(L"1");

			break;
		}
	}
}
