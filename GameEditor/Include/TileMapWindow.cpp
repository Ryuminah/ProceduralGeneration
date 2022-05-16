#include "pch.h"
#include "TileMapWindow.h"
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
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "GameObject.h"
#include "Component/TileMapComponent.h"
#include "PathManager.h"
#include "IMGUIImage.h"

CTileMapWindow::CTileMapWindow()	:
	m_CreateTile(false),
	m_ImageFrameX(0),
	m_ImageFrameY(0),
	m_ImageFrameMaxX(1),
	m_ImageFrameMaxY(1)
{
}

CTileMapWindow::~CTileMapWindow()
{
}

void CTileMapWindow::SetPosition(const Vector3& Pos)
{
	m_InputPosX->SetFloat(Pos.x);
	m_InputPosY->SetFloat(Pos.y);
	m_InputPosZ->SetFloat(Pos.z);
}

bool CTileMapWindow::Init()
{
	CreateInputPos();
	CreateInputRotation();
	CreateInputScale();

	CreateTileMapInfo();

	m_TileShape = Tile_Shape::Rect;

	CScene* Scene = CSceneManager::GetInst()->GetScene();

	Scene->GetResource()->CreateMaterial("MainMap");
	Scene->GetResource()->AddMaterialTexture("MainMap", "MainMap",
		TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));
	Scene->GetResource()->SetMaterialTransparency("MainMap", true);
	Scene->GetResource()->SetMaterialShader("MainMap", "TileMapShader");

	Scene->GetResource()->CreateMaterial("MainMapRect");
	Scene->GetResource()->AddMaterialTexture("MainMapRect", "MainMapRect",
		TEXT("Floors.png"));
	Scene->GetResource()->SetMaterialTransparency("MainMapRect", true);
	Scene->GetResource()->SetMaterialShader("MainMapRect", "TileMapShader");


	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CTileMapWindow::InputPosX()
{
}

void CTileMapWindow::InputPosY()
{
}

void CTileMapWindow::InputPosZ()
{
}

void CTileMapWindow::InputRotX()
{
}

void CTileMapWindow::InputRotY()
{
}

void CTileMapWindow::InputRotZ()
{
}

void CTileMapWindow::InputScaleX()
{
}

void CTileMapWindow::InputScaleY()
{
}

void CTileMapWindow::InputScaleZ()
{
}

void CTileMapWindow::InputImageFrameX()
{
	m_ImageFrameX = m_InputImageFrameX->GetValueInt();

	if (m_ImageFrameX >= m_ImageFrameMaxX)
		m_ImageFrameX = m_ImageFrameMaxX - 1;

	else if (m_ImageFrameX < 0)
		m_ImageFrameX = 0;

	if (m_TileMap)
		m_TileMap->UpdateInfo();

	m_TileImage->SetStartUV(m_ImageFrameX / (float)m_ImageFrameMaxX,
		m_ImageFrameY / (float)m_ImageFrameMaxY);
	m_TileImage->SetEndUV((m_ImageFrameX + 1) / (float)m_ImageFrameMaxX,
		(m_ImageFrameY + 1) / (float)m_ImageFrameMaxY);
}

void CTileMapWindow::InputImageFrameY()
{
	m_ImageFrameY = m_InputImageFrameY->GetValueInt();

	if (m_ImageFrameY >= m_ImageFrameMaxY)
		m_ImageFrameY = m_ImageFrameMaxY - 1;

	else if (m_ImageFrameY < 0)
		m_ImageFrameY = 0;

	if (m_TileMap)
		m_TileMap->UpdateInfo();

	m_TileImage->SetStartUV(m_ImageFrameX / (float)m_ImageFrameMaxX,
		m_ImageFrameY / (float)m_ImageFrameMaxY);
	m_TileImage->SetEndUV((m_ImageFrameX + 1) / (float)m_ImageFrameMaxX,
		(m_ImageFrameY + 1) / (float)m_ImageFrameMaxY);
}

void CTileMapWindow::InputImageFrameMaxX()
{
	m_ImageFrameMaxX = m_InputImageFrameMaxX->GetValueInt();

	if (m_TileMap)
	{
		m_TileMap->SetFrameMax(m_ImageFrameMaxX, m_ImageFrameMaxY);
		m_TileMap->UpdateInfo();
	}

	m_TileImage->SetStartUV(m_ImageFrameX / (float)m_ImageFrameMaxX,
		m_ImageFrameY / (float)m_ImageFrameMaxY);
	m_TileImage->SetEndUV((m_ImageFrameX + 1) / (float)m_ImageFrameMaxX,
		(m_ImageFrameY + 1) / (float)m_ImageFrameMaxY);
}

void CTileMapWindow::InputImageFrameMaxY()
{
	m_ImageFrameMaxY = m_InputImageFrameMaxY->GetValueInt();

	if (m_TileMap)
	{
		m_TileMap->SetFrameMax(m_ImageFrameMaxX, m_ImageFrameMaxY);
		m_TileMap->UpdateInfo();
	}

	m_TileImage->SetStartUV(m_ImageFrameX / (float)m_ImageFrameMaxX,
		m_ImageFrameY / (float)m_ImageFrameMaxY);
	m_TileImage->SetEndUV((m_ImageFrameX + 1) / (float)m_ImageFrameMaxX,
		(m_ImageFrameY + 1) / (float)m_ImageFrameMaxY);
}

void CTileMapWindow::InputTileCountX()
{
}

void CTileMapWindow::InputTileCountY()
{
}

void CTileMapWindow::InputTileSizeX()
{
}

void CTileMapWindow::InputTileSizeY()
{
}

void CTileMapWindow::TileShapeComboCallback(int SelectIndex, 
	const char* Item)
{
	m_TileShape = (Tile_Shape)SelectIndex;
}

void CTileMapWindow::TileModifyTypeComboCallback(int SelectIndex, 
	const char* Item)
{
	m_TileModifyType = (Tile_Modify_Type)SelectIndex;

	m_TileEditCombo->DeleteAllItem();

	switch ((Tile_Modify_Type)SelectIndex)
	{
	case Tile_Modify_Type::Type:
		m_TileEditCombo->AddItem("None");
		m_TileEditCombo->AddItem("Wall");
		break;
	case Tile_Modify_Type::Image:
		break;
	}
}

void CTileMapWindow::TileEditComboCallback(int SelectIndex, const char* Item)
{
	switch (m_TileModifyType)
	{
	case Tile_Modify_Type::Type:
		m_TileType = (Tile_Type)SelectIndex;
		break;
	case Tile_Modify_Type::Image:
		break;
	}
}

void CTileMapWindow::CreateInputPos()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("Position");
	Text->SetFont("DefaultFont");
	Text->SetText("Position");

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	Label->SetText("X");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosX = AddWidget<CIMGUITextInput>("##PosX", 50.f, 20.f);
	m_InputPosX->SetNumberFloat(true);
	m_InputPosX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputPosX);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
	Label->SetText("Y");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosY = AddWidget<CIMGUITextInput>("##PosY", 50.f, 20.f);
	m_InputPosY->SetNumberFloat(true);
	m_InputPosY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputPosY);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
	Label->SetText("Z");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputPosZ = AddWidget<CIMGUITextInput>("##PosZ", 50.f, 20.f);
	m_InputPosZ->SetNumberFloat(true);
	m_InputPosZ->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputPosZ);

	m_InputPosX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputPosY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputPosZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
}

void CTileMapWindow::CreateInputScale()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("Scale");
	Text->SetFont("DefaultFont");
	Text->SetText("Scale");

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	Label->SetText("X");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputScaleX = AddWidget<CIMGUITextInput>("##ScaleX", 50.f, 20.f);
	m_InputScaleX->SetNumberFloat(true);
	m_InputScaleX->SetFloat(1.f);
	m_InputScaleX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputScaleX);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
	Label->SetText("Y");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputScaleY = AddWidget<CIMGUITextInput>("##ScaleY", 50.f, 20.f);
	m_InputScaleY->SetNumberFloat(true);
	m_InputScaleY->SetFloat(1.f);
	m_InputScaleY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputScaleY);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
	Label->SetText("Z");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputScaleZ = AddWidget<CIMGUITextInput>("##ScaleZ", 50.f, 20.f);
	m_InputScaleZ->SetNumberFloat(true);
	m_InputScaleZ->SetFloat(1.f);
	m_InputScaleZ->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputScaleZ);

	m_InputScaleX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputScaleY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputScaleZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
}

void CTileMapWindow::CreateInputRotation()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("Rotation", 70.f);
	Text->SetFont("DefaultFont");
	Text->SetText("Rotation");

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	Label->SetText("X");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputRotX = AddWidget<CIMGUITextInput>("##RotX", 50.f, 20.f);
	m_InputRotX->SetNumberFloat(true);
	m_InputRotX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputRotX);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
	Label->SetText("Y");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputRotY = AddWidget<CIMGUITextInput>("##RotY", 50.f, 20.f);
	m_InputRotY->SetNumberFloat(true);
	m_InputRotY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputRotY);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 20.f);
	Label->SetText("Z");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputRotZ = AddWidget<CIMGUITextInput>("##RotZ", 50.f, 20.f);
	m_InputRotZ->SetNumberFloat(true);
	m_InputRotZ->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputRotZ);

	m_InputRotX->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputRotY->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
	m_InputRotZ->AddFlag(ImGuiInputTextFlags_EnterReturnsTrue);
}

void CTileMapWindow::CreateTileMapInfo()
{
	CIMGUIText* Text = AddWidget<CIMGUIText>("TileMap");
	Text->SetFont("DefaultFont");
	Text->SetText("TileShape");

	CIMGUISameLine* SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_TileShapeCombo = AddWidget<CIMGUIComboBox>("##TileShape", 300.f, 100.f);

	m_TileShapeCombo->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::TileShapeComboCallback);

	m_TileShapeCombo->AddItem("Rect");
	m_TileShapeCombo->AddItem("Rhombus");

	Text = AddWidget<CIMGUIText>("TileMap");
	Text->SetFont("DefaultFont");
	Text->SetText("TileCount");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	Label->SetText("X");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputTileCountX = AddWidget<CIMGUITextInput>("##TileCountX", 100.f, 20.f);
	m_InputTileCountX->SetNumberInt(true);
	m_InputTileCountX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputTileCountX);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
	Label->SetText("Y");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputTileCountY = AddWidget<CIMGUITextInput>("##TileCountY", 100.f, 20.f);
	m_InputTileCountY->SetNumberInt(true);
	m_InputTileCountY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputTileCountY);


	// Tile Size
	Text = AddWidget<CIMGUIText>("TileMapSize");
	Text->SetFont("DefaultFont");
	Text->SetText("TileSize");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 20.f);
	Label->SetText("X");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputTileSizeX = AddWidget<CIMGUITextInput>("##TileSizeX", 100.f, 20.f);
	m_InputTileSizeX->SetNumberFloat(true);
	m_InputTileSizeX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputTileSizeX);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 20.f);
	Label->SetText("Y");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputTileSizeY = AddWidget<CIMGUITextInput>("##TileSizeY", 100.f, 20.f);
	m_InputTileSizeY->SetNumberFloat(true);
	m_InputTileSizeY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputTileSizeY);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("타일생성", 80.f, 20.f);

	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::CreateTileButton);

	Text = AddWidget<CIMGUIText>("TileMapModifyType");
	Text->SetFont("DefaultFont");
	Text->SetText("ModifyType");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_TileModifyTypeCombo = AddWidget<CIMGUIComboBox>("##TileModifyType", 100.f, 100.f);

	m_TileModifyTypeCombo->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::TileModifyTypeComboCallback);

	m_TileModifyTypeCombo->AddItem("Type");
	m_TileModifyTypeCombo->AddItem("Image");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Text = AddWidget<CIMGUIText>("TileMapEdit");
	Text->SetFont("DefaultFont");
	Text->SetText("TileTypeEdit");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_TileEditCombo = AddWidget<CIMGUIComboBox>("##TileTypeEdit", 100.f, 100.f);

	m_TileEditCombo->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::TileEditComboCallback);

	Text = AddWidget<CIMGUIText>("TileMap");
	Text->SetFont("DefaultFont");
	Text->SetText("ImageFrame");

	Label = AddWidget<CIMGUILabel>("FrameX", 70.f, 20.f);
	Label->SetText("FrameX");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputImageFrameX = AddWidget<CIMGUITextInput>("##ImageFrameX", 100.f, 20.f);
	m_InputImageFrameX->SetNumberInt(true);
	m_InputImageFrameX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputImageFrameX);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("FrameY", 70.f, 20.f);
	Label->SetText("FrameY");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputImageFrameY = AddWidget<CIMGUITextInput>("##ImageFrameY", 100.f, 20.f);
	m_InputImageFrameY->SetNumberInt(true);
	m_InputImageFrameY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputImageFrameY);

	Label = AddWidget<CIMGUILabel>("FrameMaxX", 70.f, 20.f);
	Label->SetText("FrameMaxX");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputImageFrameMaxX = AddWidget<CIMGUITextInput>("##ImageFrameMaxX", 100.f, 20.f);
	m_InputImageFrameMaxX->SetNumberInt(true);
	m_InputImageFrameMaxX->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputImageFrameMaxX);
	m_InputImageFrameMaxX->SetInt(1);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Label = AddWidget<CIMGUILabel>("FrameMaxY", 70.f, 20.f);
	Label->SetText("FrameMaxY");
	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_InputImageFrameMaxY = AddWidget<CIMGUITextInput>("##ImageFrameMaxY", 100.f, 20.f);
	m_InputImageFrameMaxY->SetNumberInt(true);
	m_InputImageFrameMaxY->SetInputCallback<CTileMapWindow>(this, &CTileMapWindow::InputImageFrameMaxY);
	m_InputImageFrameMaxY->SetInt(1);

	Label = AddWidget<CIMGUILabel>("TileImage", 70.f, 20.f);
	Label->SetText("TileImage");

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	m_TileImage = AddWidget<CIMGUIImage>("##TileImage", 100.f, 100.f);
	m_TileImage->SetStartUV(0.f, 0.f);
	m_TileImage->SetEndUV(1.f, 1.f);
	m_TileImage->SetTexture("DefaultImageSlot", TEXT("DefaultSlot.png"));

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Button = AddWidget<CIMGUIButton>("타일이미지 불러오기", 150.f, 100.f);

	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::LoadTileImage);

	Button = AddWidget<CIMGUIButton>("저장", 80.f, 40.f);

	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SaveTileMap);

	SameLine = AddWidget<CIMGUISameLine>("SameLine");

	Button = AddWidget<CIMGUIButton>("불러오기", 80.f, 40.f);

	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::LoadTileMap);
}

void CTileMapWindow::CreateTileButton()
{
	m_CreateTile = true;

	// 타일맵을 생성한다.
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	Vector3	Pos, Rot, Scale;

	Vector2	TileSize;

	int	TileCountX = 0, TileCountY = 0;

	Pos.x = m_InputPosX->GetValueFloat();
	Pos.y = m_InputPosY->GetValueFloat();
	Pos.z = m_InputPosZ->GetValueFloat();

	Rot.x = m_InputRotX->GetValueFloat();
	Rot.y = m_InputRotY->GetValueFloat();
	Rot.z = m_InputRotZ->GetValueFloat();

	Scale.x = m_InputScaleX->GetValueFloat();
	Scale.y = m_InputScaleY->GetValueFloat();
	Scale.z = m_InputScaleZ->GetValueFloat();

	TileSize.x = m_InputTileSizeX->GetValueFloat();
	TileSize.y = m_InputTileSizeY->GetValueFloat();

	TileCountX = m_InputTileCountX->GetValueInt();
	TileCountY = m_InputTileCountY->GetValueInt();

	CGameObject* MainMap = Scene->SpawnObject<CGameObject>("MainMap");

	CTileMapComponent* TileMap = MainMap->CreateSceneComponent<CTileMapComponent>("TileMap");

	MainMap->SetRootComponent(TileMap);

	TileMap->SetWorldPos(Pos);
	TileMap->SetWorldRotation(Rot);
	TileMap->SetWorldScale(Scale);

	// TileShape를 얻어온다.
	TileMap->CreateTile<CTile>(m_TileShape, TileCountX, TileCountY, TileSize);

	TileMap->SetMaterial(0, "MainMap");

	TileMap->SetFrameMax(m_ImageFrameMaxX, m_ImageFrameMaxY);
	TileMap->SetTileDefaultFrame(m_ImageFrameX, m_ImageFrameY);

	if (m_TileTexture)
	{
		TileMap->GetMaterial(0)->SetTexture("MainMap", m_TileTexture);
		TileMap->UpdateInfo();
	}

	/*TileMap->SetMaterial(0, "MainMapRect");
	TileMap->SetFrameMax(1, 5);
	TileMap->SetTileDefaultFrame(0, 0);*/

	m_TileMap = TileMap;
}

void CTileMapWindow::SaveTileMap()
{
	if (!m_TileMap)
	{
		AfxMessageBox(TEXT("타일맵을 생성하세요"));
		return;
	}

	static TCHAR	Filter[] = TEXT("MapFile(*.map)|*.map;|모든파일(*.*)|*.*||");

	CFileDialog	dlg(FALSE, TEXT(".map"), TEXT(""), OFN_OVERWRITEPROMPT,
		Filter);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(MAP_PATH);

	dlg.m_pOFN->lpstrInitialDir = Info->pPath;

	if (dlg.DoModal() == IDOK)
	{
		m_TileMap->SaveFullPath(dlg.GetPathName());
	}
}

void CTileMapWindow::LoadTileMap()
{
	static TCHAR	Filter[] = TEXT("MapFile(*.map)|*.map;|모든파일(*.*)|*.*||");

	CFileDialog	dlg(TRUE, TEXT(".map"), TEXT(""), OFN_HIDEREADONLY,
		Filter);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(MAP_PATH);

	dlg.m_pOFN->lpstrInitialDir = Info->pPath;

	if (dlg.DoModal() == IDOK)
	{
		if (!m_TileMap)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();

			CGameObject* MainMap = Scene->SpawnObject<CGameObject>("MainMap");

			CTileMapComponent* TileMap = MainMap->CreateSceneComponent<CTileMapComponent>("TileMap");

			MainMap->SetRootComponent(TileMap);

			m_TileMap = TileMap;
		}

		m_TileMap->LoadFullPath(dlg.GetPathName());

		m_InputTileCountX->SetInt(m_TileMap->GetTileCountX());
		m_InputTileCountY->SetInt(m_TileMap->GetTileCountY());
		m_InputTileSizeX->SetFloat(m_TileMap->GetTileSize().x);
		m_InputTileSizeY->SetFloat(m_TileMap->GetTileSize().y);

		m_CreateTile = true;
	}
}

void CTileMapWindow::LoadTileImage()
{
	static TCHAR	Filter[] = TEXT("ImageFile(*.BMP, *.PNG, *.JPG, *.JPEG, *.TGA, *.DDS) | *.BMP;*.PNG;*.JPG;*.JPEG;*.TGA;*.DDS;*.bmp;*.png;*.jpg;*.jpeg;*.tga;*.dds |모든파일(*.*)|*.*||");

	CFileDialog	dlg(TRUE, TEXT(".png"), TEXT(""), OFN_HIDEREADONLY,
		Filter);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(TEXTURE_PATH);

	dlg.m_pOFN->lpstrInitialDir = Info->pPath;

	if (dlg.DoModal() == IDOK)
	{
		CScene* Scene = CSceneManager::GetInst()->GetScene();

		std::string	Name = CT2CA(dlg.GetFileTitle());

		Scene->GetResource()->LoadTextureFullPath(Name, 
			dlg.GetPathName().GetString());

		m_TileTexture = Scene->GetResource()->FindTexture(Name);

		m_TileImage->SetStartUV(0.f, 0.f);
		m_TileImage->SetEndUV(1.f, 1.f);
		m_TileImage->SetTexture(m_TileTexture);

		if (m_TileMap)
		{
			m_TileMap->GetMaterial(0)->SetTexture("MainMap", m_TileTexture);

			if (m_TileTexture)
			{
				m_TileMap->GetMaterial(0)->SetTexture("MainMap", m_TileTexture);
				m_TileMap->UpdateInfo();
			}
		}
	}
}


