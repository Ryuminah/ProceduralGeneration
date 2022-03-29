
#include "PixelCollisionTest.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Resource/Material.h"

CPixelCollisionTest::CPixelCollisionTest()
{
}

CPixelCollisionTest::CPixelCollisionTest(const CPixelCollisionTest& obj) :
	CGameObject(obj)
{
	m_Body = (CColliderPixel*)FindSceneComponent("Body");
	m_Sprite = (CSpriteComponent*)FindSceneComponent("Sprite");
}

CPixelCollisionTest::~CPixelCollisionTest()
{
}

void CPixelCollisionTest::Start()
{
	CGameObject::Start();
}

bool CPixelCollisionTest::Init()
{
	CGameObject::Init();

	m_Body = CreateSceneComponent<CColliderPixel>("Body");
	m_Sprite = CreateSceneComponent<CSpriteComponent>("Sprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(200.f, 200.f, 1.f);
	m_Sprite->SetRelativePos(100.f, 500.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	//m_Body->SetRadius(sqrtf(50.f * 50.f));
	m_Body->SetTexture(TEXT("PixelCollision.png"));
	m_Body->SetPixelCollisionType(PixelCollision_Type::Color_Ignore);
	m_Body->SetPixelInfoColor(255, 0, 255, 0);
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->AddChild(m_Body);

	CSharedPtr<CMaterial>   SpriteMtrl = m_Sprite->GetMaterial(0);

	//SpriteMtrl->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	SpriteMtrl->AddTexture("PixelCollisionTex", TEXT("PixelCollision.png"));

	return true;
}

void CPixelCollisionTest::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPixelCollisionTest::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CPixelCollisionTest::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CPixelCollisionTest::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CPixelCollisionTest* CPixelCollisionTest::Clone()
{
	return new CPixelCollisionTest(*this);
}
