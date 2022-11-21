#include "GenerateWindow.h"
#include "IMGUIButton.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUILabel.h"
#include "MapGenerator/MapGenerator.h"
#include "IMGUISameLine.h"




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
	CreateGenerateComboBox();

    return true;
}

void CGenerateWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);

}

void CGenerateWindow::CreateGenerateComboBox()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("�� �����ϱ�");
	Text->SetFont("DefaultFont");

	//CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	//Label->SetText("X");

	m_TileTypeComboBox = AddWidget<CIMGUIComboBox>("WorldList", 100.f, 100.f);
	
	m_TileTypeComboBox->AddItem("Base");
	m_TileTypeComboBox->AddItem("Land");
	m_TileTypeComboBox->AddItem("Coast");
	m_TileTypeComboBox->AddItem("Forest");
	m_TileTypeComboBox->AddItem("Lake");

	m_TileTypeComboBox->SetSelectCallback<CGenerateWindow>(this, &CGenerateWindow::ComboBoxCallback);

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");
	CIMGUIButton* Button = AddWidget<CIMGUIButton>("Generate", 100.f, 30.f);
	Button->SetClickCallback<CGenerateWindow>(this, &CGenerateWindow::Button_GenerateCallBack);

	Button = AddWidget<CIMGUIButton>("Clear", 100.f, 30.f);
	Button->SetClickCallback<CGenerateWindow>(this, &CGenerateWindow::Button_ClearCallBack);

}

void CGenerateWindow::ComboBoxCallback(int SelectIndex, const char* Item)
{
	// �� Ÿ���� ���¸� ������ �� �ִ� Editing ��ɱ��� �����ϱ�.
	m_SelectTileTypeIndex = SelectIndex;
	std::string SelectName = Item;
	
	SelectIndex -= 1;

	if (SelectName == "Base")
	{
		m_CurrentGenerator->SetCurrentTileState(TILE_STATE::BASE);
	}

	else if (SelectName == "Land")
	{
		m_CurrentGenerator->SetCurrentTileState(TILE_STATE::LAND);
	}

	else if (SelectName == "Coast")
	{
		m_CurrentGenerator->SetCurrentTileState(TILE_STATE::COAST);
	}

	else if (SelectName == "Forest")
	{
		m_CurrentGenerator->SetCurrentTileState(TILE_STATE::FOREST);
	}

	else if (SelectName == "Lake")
	{
		m_CurrentGenerator->SetCurrentTileState(TILE_STATE::LAKE);
	}
}

void CGenerateWindow::Button_GenerateCallBack()
{
	m_CurrentGenerator->GenerateWorld();
}

// ���߿� �̺κ� �ϰ��� �ְ� �����ϱ�
void CGenerateWindow::Button_ClearCallBack()
{
	m_CurrentGenerator->GenerateWorld(TILE_STATE::CLEAR);
}



