#include "pch.h"
#include "DetailWindow.h"
#include "TestWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"
#include "ObjectWindow.h"
#include "IMGUIManager.h"

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
}

void CDetailWindow::SetPosition(const Vector3& Pos)
{
	m_InputPosX->SetFloat(Pos.x);
	m_InputPosY->SetFloat(Pos.y);
	m_InputPosZ->SetFloat(Pos.z);
}

bool CDetailWindow::Init()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("À§Ä¡");
	Text->SetFont("DefaultFont");

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	Label->SetText("X");

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosX = AddWidget<CIMGUITextInput>("##PosX", 50.f, 20.f);
	m_InputPosX->SetNumberFloat(true);
	m_InputPosX->SetInputCallback<CDetailWindow>(this, &CDetailWindow::InputPosX);

	//SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
	Label->SetText("Y");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosY = AddWidget<CIMGUITextInput>("##PosY", 50.f, 20.f);
	m_InputPosY->SetNumberFloat(true);
	m_InputPosY->SetInputCallback<CDetailWindow>(this, &CDetailWindow::InputPosY);

	//SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
	Label->SetText("Z");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosZ = AddWidget<CIMGUITextInput>("##PosZ", 50.f, 20.f);
	m_InputPosZ->SetNumberFloat(true);
	m_InputPosZ->SetInputCallback<CDetailWindow>(this, &CDetailWindow::InputPosZ);

	//SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputPosY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputPosZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);


	return true;
}

void CDetailWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CDetailWindow::InputPosX()
{
	float x = m_InputPosX->GetValueFloat();

	CObjectWindow* Window = (CObjectWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectWindow");

	Window->SetPosX(x);
}

void CDetailWindow::InputPosY()
{
	float y = m_InputPosY->GetValueFloat();

	CObjectWindow* Window = (CObjectWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectWindow");

	Window->SetPosY(y);
}

void CDetailWindow::InputPosZ()
{
	float z = m_InputPosZ->GetValueFloat();

	CObjectWindow* Window = (CObjectWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectWindow");

	Window->SetPosZ(z);
}

