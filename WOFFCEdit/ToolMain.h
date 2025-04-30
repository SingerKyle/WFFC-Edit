#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>
#include "Camera.h"
#include <chrono>
#include "TransformCommand.h"
#include "CommandManager.h"
#include "CutCommand.h"

using namespace std::chrono;
using namespace std::literals;


class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	std::vector<int>		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

	//void	UpdateCamValues(float moveSpeed, float camRotRate, bool invertControls);

	inline std::shared_ptr<Camera> GetCamera() { return m_d3dRenderer.GetCamera(); }
	inline std::shared_ptr<Gizmo> GetGizmo() { return m_d3dRenderer.GetGizmo(); }

	void OnWindowStatusChanged(bool IsWindowOpen);

	void    OnResizeWindow(int width, int height);

	// Used to update camera variables from dialogue box
	void					UpdateCamValues(float moveSpeed, float camRotRate, bool invertControls);

	void    OnWindowPositionChanged(WINDOWPOS newPos);

	Game& GetGame();

	void Undo();

	void Redo();

	void Cut();

	void Copy();

	void Paste();

	void Delete();

	inline void SetCommandManager(CommandManager* commandManager) { m_commandManager = commandManager; }

	void SetSelectedFromMenu(int m_selectedObject);

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int>			m_selectedObjects;  //ID of current Selection
	int							m_selectedObject = -1;
	POINT						m_cursorPos{ 0, 0 };
	RECT						m_screenDimensions;

	TransformData				m_oldData;
	TransformData				m_newData;
	SceneObject					m_copiedObject;
	CommandManager*				m_commandManager;
	CutCommand*					m_cutCommand;

private:	//methods
	void	onContentAdded();

	void OnGizmoMove(SceneObject* Object, const Vector3& position, const Vector3& rotation, const Vector3& scale);
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	bool WindowOpen;
	bool m_shouldStoreObjectPosition;
	bool m_hasCopiedData = false;
	

	std::chrono::steady_clock::time_point			m_leftMouseDownTime;
	const std::chrono::milliseconds					holdThreshold = 500ms;
	
};
