#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "ObjectEditDialogue.h"
#include "EditorSettingsDialogue.h"


class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

	void OnDialogueBoxDestroyed();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	ObjectEditDialogue m_ToolEditObjectDialogue;
	EditorSettingsDialogue m_ToolEditorSettings;

	int m_width;		
	int m_height;
	bool WindowOpen;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg	void ToolBarButton1();
	afx_msg void AngryButtonFunction();
	afx_msg void Undo();
	afx_msg void Redo();
	afx_msg void EditObjectTransform();
	afx_msg void EditorSettings();

	void OnDialogueBoxCreated();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
