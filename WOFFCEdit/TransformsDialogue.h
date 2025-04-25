#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "CommandManager.h"

class MFCMain;
class Game;

enum class Axis { X, Y, Z };
enum class Operation { Add, Subtract };
enum class SelectedButton { None, Plus, Minus, X, Y, Z };
enum class Transform { Position, Rotation, Scale };

class TransformsDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TransformsDialogue)
public:
	TransformsDialogue(CWnd* pParent = nullptr);
	virtual ~TransformsDialogue();

	void SetMain(MFCMain* main);
	void SetGame(Game* game);

	inline void SetSelection(int selection) { m_selectedIndex = selection;}
	void SetSceneGraph(std::vector<SceneObject>* sceneGraph);

	inline void SetCommandManager(CommandManager* commandManager) { m_commandManager = commandManager; }

	void UpdateFromSelectedObject();

	void InitialiseWithSelection();

	void OnUndo();

	void OnRedo();
	afx_msg void End();		//kill the dialogue
	void OnClose();

	void OnContextMenu(CWnd* pWnd, CPoint point);

	BOOL PreTranslateMessage(MSG* pMsg);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORMS_DIALOGUE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	MFCMain* Main;

	CommandManager* m_commandManager;

private:
	std::vector<SceneObject>* m_sceneGraph = nullptr;
	int m_selectedIndex = -1;
	int* m_currentSelection = &m_selectedIndex; 
	Game* m_game = nullptr;  // Pointer to the Game object

	// Position fields
	CEdit m_posX, m_posY, m_posZ;

	// Rotation fields
	CEdit m_rotX, m_rotY, m_rotZ;

	// Scale fields
	CEdit m_scaleX, m_scaleY, m_scaleZ;

	CEdit m_currentID;

	CButton m_plusButton, m_minusButton;

	Axis m_selectedAxis = Axis::X;
	Operation m_operation = Operation::Add;
	SelectedButton m_selectedButton = SelectedButton::None;
	Transform m_transforms = Transform::Position;

	// Event handlers
	afx_msg void OnChangeTransform();
	afx_msg void OnIncrement(UINT nID); // All increment buttons share one handler
	void ApplyChangesToObject();

	void UpdateEditFields();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAxisSelectedX();
	afx_msg void OnAxisSelectedY();
	afx_msg void OnAxisSelectedZ();
	afx_msg void OnOperationPlus();
	afx_msg void OnOperationMinus();
	afx_msg void OnTransformAmountClicked(UINT nID);
};

