
#include "TestWindow.h"
#include "IMGUIButton.h"

CTestWindow::CTestWindow()
{
}

CTestWindow::~CTestWindow()
{
}

bool CTestWindow::Init()
{
    CIMGUIButton* Button = AddWidget<CIMGUIButton>("Button1", 100.f, 100.f);

    Button->SetClickCallback(this, &CTestWindow::TestButtonClick);

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
