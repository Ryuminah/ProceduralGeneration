#include "pch.h"
#include "ObjectWindow.h"
#include "TestWindow.h"
#include "IMGUIButton.h"
#include "IMGUIConsole.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/Camera.h"
#include "Component/SpringArm.h"
#include "Component/SpringArm2D.h"
#include "DetailWindow.h"
#include "IMGUIManager.h"

CObjectWindow::CObjectWindow()	:
	m_CreateObjectCount(0),
	m_SelectObjectIndex(-1),
	m_SelectComponentIndex(-1),
	m_CreateComponentIndex(-1)
{
}

CObjectWindow::~CObjectWindow()
{
}

bool CObjectWindow::Init()
{
	m_ObjListBox = AddWidget<CIMGUIListBox>("ListBox", 300.f, 100.f);

	m_ObjListBox->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::ListCallback);

	m_ComponentListBox = AddWidget<CIMGUIListBox>("ComponentListBox", 300.f, 100.f);

	m_ComponentListBox->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::ComponentListCallback);
	

	CIMGUIButton* CreateObjButton = AddWidget<CIMGUIButton>("오브젝트생성");

	CreateObjButton->SetFont("DefaultFont");

	CreateObjButton->SetClickCallback<CObjectWindow>(this, &CObjectWindow::CreateObjectButtonClick);

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUIComboBox* ComponentCombo = AddWidget<CIMGUIComboBox>("ComponentCombo", 300.f, 100.f);

	ComponentCombo->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::ComponentComboCallback);

	ComponentCombo->AddItem("Scene");
	ComponentCombo->AddItem("Sprite");
	ComponentCombo->AddItem("Circle");
	ComponentCombo->AddItem("Box2D");
	ComponentCombo->AddItem("Circle");
	ComponentCombo->AddItem("Pixel");
	ComponentCombo->AddItem("Camera");
	ComponentCombo->AddItem("SpringArm");
	ComponentCombo->AddItem("SpringArm2D");

	CIMGUIButton* SaveBtn = AddWidget<CIMGUIButton>("저장", 100.f, 40.f);
	SaveBtn->SetFont("DefaultFont");
	SaveBtn->SetClickCallback<CObjectWindow>(this, &CObjectWindow::SaveButton);

	CIMGUISameLine* Line2 = AddWidget<CIMGUISameLine>("");

	CIMGUIButton* LoadBtn = AddWidget<CIMGUIButton>("불러오기", 100.f, 40.f);
	LoadBtn->SetFont("DefaultFont");
	LoadBtn->SetClickCallback<CObjectWindow>(this, &CObjectWindow::LoadButton);

	return true;
}

void CObjectWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}


void CObjectWindow::ListCallback(int SelectIndex, const char* Item)
{
	m_SelectObjectIndex = SelectIndex;

	std::string SelectName = Item;

	CScene* Scene = CSceneManager::GetInst()->GetScene();

	m_SelectObject = Scene->FindObject(SelectName);

	m_SelectComponent = nullptr;
	m_SelectComponentIndex = -1;

	// 가지고 있는 컴포넌트의 이름을 얻어온다.
	std::vector<std::string>	vecName;

	m_SelectObject->GetComponentName(vecName);

	m_ComponentListBox->Clear();

	size_t	Size = vecName.size();

	for (size_t i = 0; i < vecName.size(); ++i)
	{
		m_ComponentListBox->AddItem(vecName[i].c_str());
	}
}

void CObjectWindow::ComponentListCallback(int SelectIndex, const char* Item)
{
	m_SelectComponentIndex = SelectIndex;

	m_SelectComponent = m_SelectObject->FindSceneComponent(Item);

	Vector3	Pos = m_SelectComponent->GetWorldPos();

	CDetailWindow* DetailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");

	if (DetailWindow)
		DetailWindow->SetPosition(Pos);
}

void CObjectWindow::ComponentComboCallback(int SelectIndex, const char* Item)
{
	if (!m_SelectObject || !m_SelectComponent)
		return;

	m_CreateComponentIndex = SelectIndex;

	// 이름을 지정하기 위한 Popup 창을 열어준다.
	m_NameInput = AddPopupWidget<CIMGUITextInput>("NameInput");

	CIMGUIButton* NameButton = AddPopupWidget<CIMGUIButton>("NameButton");

	NameButton->SetClickCallback<CObjectWindow>(this, &CObjectWindow::InputNamePopupButton);

	SetPopupTitle("Component Name");
	EnableModalPopup();
}

void CObjectWindow::CreateObjectButtonClick()
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	char	ObjName[256] = {};

	sprintf_s(ObjName, "GameObject_%d", m_CreateObjectCount);

	++m_CreateObjectCount;

	CGameObject* Obj = Scene->SpawnObject<CGameObject>(ObjName);

	m_ObjListBox->AddItem(ObjName);
}

void CObjectWindow::InputNamePopupButton()
{
	if (!m_SelectObject)
		return;

	// 이름을 얻어온다.
	const char* Name = m_NameInput->GetTextMultibyte();

	CSceneComponent* NewComponent = nullptr;

	switch ((Component_Class_Type)m_CreateComponentIndex)
	{
	case Component_Class_Type::Scene:
		NewComponent = m_SelectObject->CreateSceneComponent<CSceneComponent>(Name);
		break;
	case Component_Class_Type::Sprite:
		NewComponent = m_SelectObject->CreateSceneComponent<CSpriteComponent>(Name);
		break;
	case Component_Class_Type::Box2D:
		NewComponent = m_SelectObject->CreateSceneComponent<CColliderBox2D>(Name);
		break;
	case Component_Class_Type::Circle:
		NewComponent = m_SelectObject->CreateSceneComponent<CColliderCircle>(Name);
		break;
	case Component_Class_Type::Pixel:
		NewComponent = m_SelectObject->CreateSceneComponent<CColliderPixel>(Name);
		break;
	case Component_Class_Type::Camera:
		NewComponent = m_SelectObject->CreateSceneComponent<CCamera>(Name);
		break;
	case Component_Class_Type::SpringArm:
		NewComponent = m_SelectObject->CreateSceneComponent<CSpringArm>(Name);
		break;
	case Component_Class_Type::SpringArm2D:
		NewComponent = m_SelectObject->CreateSceneComponent<CSpringArm2D>(Name);
		break;
	}

	m_ComponentListBox->AddItem(Name);

	m_SelectComponent->AddChild(NewComponent);

	NewComponent->SetRelativePos(10.f, 20.f, 30.f);

	ClosePopup();
}

void CObjectWindow::SetPosX(float x)
{
	if (!m_SelectComponent)
		return;

	Vector3	Pos = m_SelectComponent->GetWorldPos();
	Pos.x = x;

	m_SelectComponent->SetWorldPos(Pos);
}

void CObjectWindow::SetPosY(float y)
{
	if (!m_SelectComponent)
		return;

	Vector3	Pos = m_SelectComponent->GetWorldPos();
	Pos.y = y;

	m_SelectComponent->SetWorldPos(Pos);
}

void CObjectWindow::SetPosZ(float z)
{
	if (!m_SelectComponent)
		return;

	Vector3	Pos = m_SelectComponent->GetWorldPos();
	Pos.z = z;

	m_SelectComponent->SetWorldPos(Pos);
}

void CObjectWindow::SaveButton()
{
	static TCHAR Filter[] = TEXT("텍스트 파일(*.txt)|*.txt|모든파일(*.*)|*.*||");
	CFileDialog	dlg(FALSE, TEXT("*.txt"), TEXT(""), OFN_OVERWRITEPROMPT,
		Filter);

	if (dlg.DoModal() == IDOK)
	{
		CString	Path = dlg.GetPathName();

		AfxMessageBox(Path);
	}
}

void CObjectWindow::LoadButton()
{
	static TCHAR Filter[] = TEXT("텍스트 파일(*.txt)|*.txt|모든파일(*.*)|*.*||");
	CFileDialog	dlg(TRUE, TEXT("*.txt"), TEXT(""), OFN_HIDEREADONLY,
		Filter);

	if (dlg.DoModal() == IDOK)
	{
		CString	Path = dlg.GetPathName();

		AfxMessageBox(Path);
	}
}
