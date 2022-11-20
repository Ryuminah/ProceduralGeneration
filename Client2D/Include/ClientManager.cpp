#include "ClientManager.h"
#include "MapGenerator\MapGeneratorManager.h"
#include "Scene/SceneManager.h"
#include "SceneMode/MainScene.h"
#include "Input.h"
#include "CollisionManager.h"
#include "IMGUIManager.h"
#include "Engine.h"
#include "UI/MouseWidget.h"
#include "Resource/ResourceManager.h"
#include "MapGenerator/MapGeneratorManager.h"
#include "GenerateWindow.h"
#include "MapGenerator/GenerateOptionManager.h"

#include "SceneMode/StartScene.h"

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
   CMapGeneratorManager::DestroyInst();
   CGenerateOptionManager::DestroyInst();
}

bool CClientManager::Init()
{
    // Debug창을 띄우는 코드
	//CEngine::GetInst()->OnDebugLog();
	CreateKey();

	if (!CMapGeneratorManager::GetInst()->Init())
		return false;

    CIMGUIManager::GetInst()->AddWindow<CGenerateWindow>("GenerateWindow");

	CSceneManager::GetInst()->SetSceneMode<CMainScene>();

	return true;
}

void CClientManager::CreateMouse()
{
    // 마우스 애니메이션 시퀀스 생성
    CResourceManager::GetInst()->CreateAnimationSequence2D("MouseDefault");

    std::vector<const TCHAR*> vecFileName;

    vecFileName.push_back(TEXT("Mouse/Default/0.png"));
    vecFileName.push_back(TEXT("Mouse/Default/1.png"));
    vecFileName.push_back(TEXT("Mouse/Default/2.png"));
    vecFileName.push_back(TEXT("Mouse/Default/3.png"));
    vecFileName.push_back(TEXT("Mouse/Default/4.png"));
    vecFileName.push_back(TEXT("Mouse/Default/5.png"));
    vecFileName.push_back(TEXT("Mouse/Default/6.png"));
    vecFileName.push_back(TEXT("Mouse/Default/7.png"));
    vecFileName.push_back(TEXT("Mouse/Default/8.png"));
    vecFileName.push_back(TEXT("Mouse/Default/9.png"));
    vecFileName.push_back(TEXT("Mouse/Default/10.png"));
    vecFileName.push_back(TEXT("Mouse/Default/11.png"));
    vecFileName.push_back(TEXT("Mouse/Default/12.png"));

    CResourceManager::GetInst()->SetAnimationSequence2DTexture("MouseDefault",
        "MouseDefault", vecFileName);

    CResourceManager::GetInst()->AddAnimationSequence2DFrame("MouseDefault",
        Vector2(0.f, 0.f), Vector2(32.f, 31.f), 13);

    CMouseWidget* Mouse = CEngine::GetInst()->CreateMouseWidget<CMouseWidget>("Mouse");

    Mouse->SetSize(32.f, 31.f);

    Mouse->CreateAnimation2D<CAnimation2D>();

    CAnimation2D* MouseAnim = Mouse->GetAnimation2D();

    MouseAnim->AddAnimationSequence2D("MouseDefault");
}

void CClientManager::CreateKey()
{
    // 키 등록
    CInput::GetInst()->CreateKey("MoveUp", 'W');
    CInput::GetInst()->CreateKey("MoveDown", 'S');
    CInput::GetInst()->CreateKey("RotationZ", 'A');
    CInput::GetInst()->CreateKey("RotationZInv", 'D');
    CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
    CInput::GetInst()->CreateKey("MouseRButton", VK_RBUTTON);
    CInput::GetInst()->CreateKey("Space", VK_SPACE);
    CInput::GetInst()->CreateKey("Enter", VK_RETURN);
    CInput::GetInst()->CreateKey("1", '1');
    CInput::GetInst()->CreateKey("2", '2');
    CInput::GetInst()->CreateKey("3", '3');
    CInput::GetInst()->CreateKey("4", '4');
    CInput::GetInst()->CreateKey("5", '5');
    CInput::GetInst()->CreateKey("6", '6');
    CInput::GetInst()->CreateKey("7", '7');


}

void CClientManager::CreateCollision()
{
    CCollisionManager::GetInst()->CreateChannel("PlayerAttack", Collision_Interaction::Block);
    CCollisionManager::GetInst()->CreateChannel("MonsterAttack", Collision_Interaction::Block);

    CCollisionManager::GetInst()->CreateProfile("PlayerAttack", Collision_Channel::Custom1);
    CCollisionManager::GetInst()->CreateProfile("MonsterAttack", Collision_Channel::Custom2);

    CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::Custom1,
        Collision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetProfileChannelState("PlayerAttack", Collision_Channel::Custom1,
        Collision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetProfileChannelState("MonsterAttack", Collision_Channel::Custom1,
        Collision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetProfileChannelState("Monster", Collision_Channel::Custom2,
        Collision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetProfileChannelState("PlayerAttack", Collision_Channel::Custom2,
        Collision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetProfileChannelState("MonsterAttack", Collision_Channel::Custom2,
        Collision_Interaction::Ignore);

}
