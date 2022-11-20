#include "GenerateWindow.h"
#include "IMGUIButton.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"


CGenerateWindow::CGenerateWindow()
{
}

CGenerateWindow::~CGenerateWindow()
{
}

bool CGenerateWindow::Init()
{
    //CIMGUIButton* Button = AddWidget<CIMGUIButton>("Button", 50.f, 30.f);

    //Button->SetClickCallback<CGenerateWindow>(this, &CGenerateWindow::TestButtonClick);

    m_TileTypeComboBox = AddWidget<CIMGUIComboBox>("WorldList", 300.f, 100.f);
    
    m_TileTypeComboBox->SetSelectCallback<CGenerateWindow>(this, &CGenerateWindow::ComboBoxCallback);

    return true;
}

void CGenerateWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);

}

void CGenerateWindow::ComboBoxCallback(int SelectIndex, const char* Item)
{
	// 각 타일의 상태를 변경할 수 있는 Editing 기능까지 제작하기.
	m_SelectTileTypeIndex = SelectIndex;
	std::string SelectName = Item;

	//// 
	//CScene* Scene = CSceneManager::GetInst()->GetScene();

	//m_SelectObject = Scene->FindObject(SelectName);

	//m_SelectComponent = nullptr;
	//m_SelectComponentIndex = -1;

	//// 가지고 있는 컴포넌트의 이름을 얻어온다.
	//std::vector<std::string>	vecName;

	//m_SelectObject->GetComponentName(vecName);

	//m_ComponentListBox->Clear();

	//size_t	Size = vecName.size();

	//for (size_t i = 0; i < vecName.size(); ++i)
	//{
	//	m_ComponentListBox->AddItem(vecName[i].c_str());
	//}
}

