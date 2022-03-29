
#include "pch.h"
#include "EditorScene.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Component/Camera.h"
#include "../TileMapWindow.h"
#include "IMGUIManager.h"
#include "Component/TileMapComponent.h"

CEditorScene::CEditorScene()
{
}

CEditorScene::~CEditorScene()
{
}

bool CEditorScene::Init()
{
    CInput::GetInst()->CreateKey("MoveUp", 'W');
    CInput::GetInst()->CreateKey("MoveDown", 'S');
    CInput::GetInst()->CreateKey("MoveLeft", 'A');
    CInput::GetInst()->CreateKey("MoveRight", 'D');
    CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);

    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveUp", KT_Push, this, &CEditorScene::MoveUp);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveDown", KT_Push, this, &CEditorScene::MoveDown);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveLeft", KT_Push, this, &CEditorScene::MoveLeft);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MoveRight", KT_Push, this, &CEditorScene::MoveRight);
    CInput::GetInst()->AddKeyCallback<CEditorScene>("MouseLButton", KT_Push, this, &CEditorScene::MouseLButton);

    m_TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");

	return true;
}

void CEditorScene::Update(float DeltaTime)
{
    CSceneMode::Update(DeltaTime);
}

void CEditorScene::PostUpdate(float DeltaTime)
{
    CSceneMode::PostUpdate(DeltaTime);
}

void CEditorScene::MoveUp(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CEditorScene::MoveDown(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_Y) * -300.f * DeltaTime);
}

void CEditorScene::MoveLeft(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_X) * -300.f * DeltaTime);
}

void CEditorScene::MoveRight(float DeltaTime)
{
    CCamera* Camera = m_pScene->GetCameraManager()->GetCurrentCamera();

    Camera->AddRelativePos(Camera->GetAxis(AXIS_X) * 300.f * DeltaTime);
}

void CEditorScene::MouseLButton(float DeltaTime)
{
    if (!m_TileMapWindow->IsTileMap())
        return;

    Tile_Modify_Type    ModifyType = m_TileMapWindow->GetTileModifyType();

    switch (ModifyType)
    {
    case Tile_Modify_Type::Type:
        EditTileType();
        break;
    case Tile_Modify_Type::Image:
        EditTileImage();
        break;
    }
}

void CEditorScene::EditTileType()
{
    Tile_Type   Type = m_TileMapWindow->GetTileType();

    Vector2 MousePos = CInput::GetInst()->GetMouse2DWorldPos();

    CTileMapComponent* TileMap = m_TileMapWindow->GetTileMap();

    // 마우스 위치를 이용해서 현재 마우스가 위치한 곳의 타일을 얻어온다.
    CTile* Tile = TileMap->GetTile(MousePos);

    if (Tile)
        Tile->SetTileType(Type);
}

void CEditorScene::EditTileImage()
{
    int ImageFrameX = m_TileMapWindow->GetImageFrameX();
    int ImageFrameY = m_TileMapWindow->GetImageFrameY();

    Vector2 MousePos = CInput::GetInst()->GetMouse2DWorldPos();

    CTileMapComponent* TileMap = m_TileMapWindow->GetTileMap();

    if (ImageFrameX == -1 || ImageFrameY == -1)
    {
        TileMap->TileRemoveRender(Vector3(MousePos.x, MousePos.y, 0.f));

        CTile* Tile = TileMap->GetTile(MousePos);

        if (Tile)
            Tile->SetTileType(Tile_Type::Wall);
    }

    else
    {
        TileMap->SetTileFrame(MousePos, ImageFrameX, ImageFrameY);
    }
}
