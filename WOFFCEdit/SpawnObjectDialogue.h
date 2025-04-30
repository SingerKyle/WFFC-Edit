#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"


class SpawnObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SpawnObjectDialogue)

public:
	SpawnObjectDialogue(CWnd* pParent = nullptr);

	// object data
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	float m_scaX, m_scaY, m_scaZ;
	CString m_selectedType;

	// dialogue values
	CEdit m_editposX, m_editposY, m_editposZ;
	CEdit m_editrotX, m_editrotY, m_editrotZ;
	CEdit m_editscaX, m_editscaY, m_editscaZ;
	CTreeCtrl m_objectTree;


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPAWNOBJECT };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
private:

};

