#include "GenerateOptionWindow.h"
#include "IMGUIButton.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUILabel.h"
#include "MapGenerator/MapGenerator.h"
#include "IMGUISameLine.h"


CGenerateOptionWindow::CGenerateOptionWindow()
{
}

CGenerateOptionWindow::~CGenerateOptionWindow()
{

}

bool CGenerateOptionWindow::Init()
{
	CreateGenerateOptionBox();
	return true;
}

void CGenerateOptionWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

}

void CGenerateOptionWindow::CreateGenerateOptionBox()
{
	CIMGUIButton* Button = AddWidget<CIMGUIButton>("CreateMap", 100.f, 30.f);
	Button->SetClickCallback<CGenerateOptionWindow>(this, &CGenerateOptionWindow::Button_CreateMapCallBack);
}

void CGenerateOptionWindow::Button_CreateMapCallBack()
{

}
