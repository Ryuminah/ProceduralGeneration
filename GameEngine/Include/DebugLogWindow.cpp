#include "DebugLogWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"

CDebugLogWindow::CDebugLogWindow()
{
}

CDebugLogWindow::~CDebugLogWindow()
{
}

bool CDebugLogWindow::Init()
{
	CIMGUIButton* ClearButton = AddWidget<CIMGUIButton>("Clear");

	ClearButton->SetClickCallback<CDebugLogWindow>(this, &CDebugLogWindow::ClearButton);

	m_Console = AddWidget<CIMGUIConsole>("Console");

	m_Console->SetMaxHistory(300);

	return true;
}

void CDebugLogWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CDebugLogWindow::ClearButton()
{
	m_Console->Clear();
}

void CDebugLogWindow::AddLog(const char* Text)
{
	m_Console->AddText(Text);
}
