#include "pch.h"
#include "TestWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"

CTestWindow::CTestWindow()
{
}

CTestWindow::~CTestWindow()
{
}

bool CTestWindow::Init()
{
	CIMGUIButton* Button = AddWidget<CIMGUIButton>("Button1", 100.f, 100.f);

	Button->SetClickCallback<CTestWindow>(this, &CTestWindow::TestButtonClick);

	CIMGUISameLine* SameLine1 = AddWidget<CIMGUISameLine>("SameLine");
	SameLine1->SetStartX(300.f);
	SameLine1->SetSpacing(200.f);

	CIMGUIButton* Button2 = AddWidget<CIMGUIButton>("Button2", 100.f, 100.f);

	Button2->SetClickCallback(this, &CTestWindow::TestButtonClick);

	CIMGUIButton* Button3 = AddWidget<CIMGUIButton>("3번버튼", 100.f, 100.f);
	Button3->SetFont("DefaultFont");

	Button3->SetClickCallback(this, &CTestWindow::TestButtonClick);

	CIMGUILabel* Label1 = AddWidget<CIMGUILabel>("Test", 100.f, 100.f);
	Label1->SetFont("DefaultFont");

	Label1->SetText("Labe 테스트입니다.");

	CIMGUIText* Text1 = AddWidget<CIMGUIText>("Text", 100.f, 100.f);
	Text1->SetFont("DefaultFont");

	Text1->SetText("Text 테스트입니다.");

	CIMGUIListBox* ListBox1 = AddWidget<CIMGUIListBox>("ListBox", 300.f, 100.f);

	ListBox1->AddItem("AAA");
	ListBox1->AddItem("CCC");
	ListBox1->AddItem("EEE");
	ListBox1->AddItem("FFF");
	ListBox1->AddItem("BBB");
	ListBox1->AddItem("ZZZ");
	ListBox1->AddItem("HHH");

	ListBox1->SetSelectCallback<CTestWindow>(this, &CTestWindow::ListCallback);
	ListBox1->Sort(true);


	CIMGUIComboBox* ComboBox1 = AddWidget<CIMGUIComboBox>("TestComboBox");

	ComboBox1->AddItem("AAA");
	ComboBox1->AddItem("CCC");
	ComboBox1->AddItem("EEE");
	ComboBox1->AddItem("FFF");
	ComboBox1->AddItem("BBB");
	ComboBox1->AddItem("ZZZ");
	ComboBox1->AddItem("HHH");
	/*for (int i = 0; i < 100; ++i)
	{
		ComboBox1->AddItem("HHH");
	}*/

	ComboBox1->SetSelectCallback<CTestWindow>(this, &CTestWindow::ListCallback);

	CIMGUITextInput* TextInput1 = AddWidget<CIMGUITextInput>("TextInput", 100.f, 100.f);
	//TextInput1->SetFont("DefaultFont");

	TextInput1->SetMultiLine(true);
	//TextInput1->SetNumberInt(true);
	//Text1->SetText("Text 테스트입니다.");

	return true;
}

void CTestWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CTestWindow::TestButtonClick()
{
	MessageBox(0, TEXT("Test"), TEXT("Test"), MB_OK);
}

void CTestWindow::ListCallback(int SelectIndex, const char* Item)
{
	char    Text[256] = {};
	sprintf_s(Text, "Index : %d Item : %s", SelectIndex, Item);

	MessageBoxA(0, Text, "ListBox", MB_OK);
}
