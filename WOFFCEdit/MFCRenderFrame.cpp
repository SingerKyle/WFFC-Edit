
#include "MFCRenderFrame.h"
#include "ToolMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildRender::CChildRender()
{
}

CChildRender::~CChildRender()
{
}


BEGIN_MESSAGE_MAP(CChildRender, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildRender::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildRender::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CChildRender::OnSize(UINT nType, int cx, int cy)
{
	if (m_toolMain)
	{
		RECT clientRect;
		GetClientRect(&clientRect);

		m_toolMain->OnResizeWindow(clientRect.right, clientRect.bottom);
	}
}

void CChildRender::OnWindowPosChanged(WINDOWPOS*)
{
	if (m_toolMain)
	{
		RECT windowRect;
		GetWindowRect(&windowRect);

		WINDOWPOS newPos;
		newPos.x = windowRect.left;
		newPos.y = windowRect.top;
		newPos.cx = windowRect.right - windowRect.left;
		newPos.cy = windowRect.bottom - windowRect.top;

		m_toolMain->OnWindowPositionChanged(newPos);
	}
}




