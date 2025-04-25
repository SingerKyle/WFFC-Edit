#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "ToolMain.h"

struct FEditorSettings
{
	float moveSpeed = 150.0f;
	float rotationSpeed = 10.0f;
	bool invertCamera = false;

	bool LoadFromFile(const std::wstring& filename);
	bool SaveToFile(const std::wstring& filename);

	void UpdateCamValues(float moveSpeed,
	float rotationSpeed, bool invertCamera);
};

// EditorSettings dialog
class EditorSettingsDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(EditorSettingsDialogue)

public:
	EditorSettingsDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EditorSettingsDialogue();
	void SetObjectData(ToolMain* toolMain);

	void SetLoadedValues();


	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EditorSettingsDialogue };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

	ToolMain* m_Tool;
	void SetInitialText(CEdit& box, float value);

public:
	CEdit m_moveSpeed;
	CEdit m_camRotRate;
	CButton m_camInvertControls;
	CEdit m_resolutionX;
	CEdit m_resolutionY;

	FEditorSettings camSettings;

	virtual BOOL OnInitDialog() override;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
