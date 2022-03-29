#include "pch.h"
#include "LogWindow.h"
#include "TestWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"

CLogWindow::CLogWindow()
{
}

CLogWindow::~CLogWindow()
{
}

bool CLogWindow::Init()
{
	CIMGUIConsole* Console = AddWidget<CIMGUIConsole>("Console");

	//Console->SetClickCallback<CLogWindow>(this, &CLogWindow::TestButtonClick);
	
	srand(GetTickCount());
	rand();

	const char* TextArray[3] = { "Test", "Test1", "Test2" };
	ImVec4	Color[3] = { ImVec4(1.f, 0.f, 0.f, 1.f), ImVec4(0.f, 1.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f) };

	for (int i = 0; i < 100; ++i)
	{
		Console->AddTextImgui(TextArray[rand() % 3], Color[rand() % 3]);
	}


	return true;
}

void CLogWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}
