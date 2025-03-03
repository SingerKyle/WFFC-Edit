#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include "vector"

class ObjectEditDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectEditDialogue)

public:
	ObjectEditDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph);   // modal // takes in out scenegraph in the constructor
	ObjectEditDialogue(CWnd* pParent = NULL);
	virtual ~ObjectEditDialogue();
	void GrabCurrentSelectedObject(std::vector<SceneObject>* SceneGraph);

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTRANSFORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Select();	//Item has been selected

	std::vector<SceneObject> * m_sceneGraph;
	int m_currentSelection;


	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient access of the listbox
	CComboBox m_comboBox;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg void OnCbnSelchangeObjectSelect();

	CEdit m_PositionX;
	CEdit m_PositionY;
	CEdit m_PositionZ;
};


INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);

