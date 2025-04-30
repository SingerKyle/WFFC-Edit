#include "MFCMain.h"
#include "resource.h"
#include <fstream> 
#include <string> 


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_COMMAND(ID_EDIT_OBJECTINSPECTOR, &MFCMain::Inspector)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
	ON_COMMAND(ID_SETTINGS_EDITOR_SETTINGS, &MFCMain::EditorSettings)
	ON_COMMAND(ID_EDIT_CREATEOBJECT, &MFCMain::SpawnObject)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1920, 1080),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	m_commandManager = new CommandManager();

	WindowOpen = false;

	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	m_frame->m_DirXView.m_toolMain = &m_ToolSystem;
	m_ToolSystem.SetCommandManager(m_commandManager);

	LoadEditorSettings(L"CameraSettings.txt");



	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{
			std::vector<int> IDs = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: ";

			if (IDs.empty())
			{
				statusString += L"None";
			}
			else
			{
				for (size_t i = 0; i < IDs.size(); i++)
				{
					// Add the current ID to the string
					statusString += std::to_wstring(IDs[i]);

					// Add a comma if this isn't the last ID
					if (i < IDs.size() - 1)
					{
						statusString += L", ";
					}


				}
			}
			m_ToolSystem.Tick(&msg);

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);
		}

		
	}

	return (int)msg.wParam;
}

void MFCMain::OnDialogueBoxDestroyed()
{
	WindowOpen = false;
	m_ToolSystem.OnWindowStatusChanged(WindowOpen);
}

void MFCMain::LoadEditorSettings(const std::wstring& filename)
{
	std::wifstream file(filename);
	if (!file.is_open()) return;

	FEditorSettings LoadedCamSettings;

	file >> LoadedCamSettings.moveSpeed;
	file >> LoadedCamSettings.rotationSpeed;
	file >> LoadedCamSettings.invertCamera;

	m_ToolSystem.UpdateCamValues(LoadedCamSettings.moveSpeed, LoadedCamSettings.rotationSpeed, LoadedCamSettings.invertCamera);

	return;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetTool(&m_ToolSystem);
	m_ToolSelectDialogue.SetMain(this);
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
	
	OnDialogueBoxCreated();
}

void MFCMain::ToolBarButton1()
{
	m_ToolSystem.onActionSave();
}

void MFCMain::Inspector()
{
	if (!m_ToolSystem.m_selectedObjects.empty())
	{
		// Get the last selected object from the list
		int lastSelectedIndex = m_ToolSystem.m_selectedObjects.back();

		// Pass the index to the TransformsDialogue
		m_ToolTransformsDialogue.Create(IDD_TRANSFORMS_DIALOGUE);
		m_ToolTransformsDialogue.SetSceneGraph(&m_ToolSystem.m_sceneGraph);
		m_ToolTransformsDialogue.SetMain(this);
		m_ToolTransformsDialogue.SetGame(&m_ToolSystem.GetGame());
		m_ToolTransformsDialogue.SetSelection(lastSelectedIndex);
		m_ToolTransformsDialogue.SetCommandManager(m_commandManager);
		m_ToolTransformsDialogue.ShowWindow(SW_SHOW);

		m_ToolTransformsDialogue.InitialiseWithSelection();
		OnDialogueBoxCreated();
	}
}

void MFCMain::EditorSettings()
{
	m_ToolEditorSettings.Create(IDD_EDITOR_SETTINGS);	//Start up modeless
	m_ToolEditorSettings.ShowWindow(SW_SHOW);	//show modeless
	m_ToolEditorSettings.SetObjectData(&m_ToolSystem);
	m_ToolEditorSettings.SetMain(this);

	OnDialogueBoxCreated();
}

void MFCMain::SpawnObject()
{
	m_SpawnObjectDialogue.Create(IDD_SPAWNOBJECT);
	m_SpawnObjectDialogue.ShowWindow(SW_SHOW);
}

void MFCMain::OnDialogueBoxCreated()
{
	WindowOpen = true;
	m_ToolSystem.OnWindowStatusChanged(WindowOpen);
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
