
#include "ImageObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material.h"
#include "Engine.h"
#include "../Animation2D/Anim_Image.h"


CImageObject::CImageObject()
{
}

CImageObject::CImageObject(const CImageObject& obj) :
	CGameObject(obj)
{
	m_Image = (CSpriteComponent*)FindSceneComponent("Image");
	m_ImageAnim = m_Image->GetAnimation2D();
}

CImageObject::~CImageObject()
{
}

void CImageObject::Start()
{
	CGameObject::Start();
}

bool CImageObject::Init()
{
	CGameObject::Init();

	//m_pScene->GetResource()->CreateMaterial("Background");
	//m_pScene->GetResource()->AddMaterialTexture("Background", "Background",
	//	TEXT("RandomMap/Background.png"));

	m_Image = CreateSceneComponent<CSpriteComponent>("Image");
	SetRootComponent(m_Image);
	m_Image->SetPivot(0.f, 0.f, 0.f);
	m_Image->SetRelativePos(0.f, 0.f, 0.9f);
	m_Image->SetRelativeScale(7200.f, 5000, 1.f);
	m_Image->SetRender2DType(Render_Type_2D::RT2D_MAP);

	m_Image->CreateAnimation2D<CAnim_Image>();
	m_ImageAnim = m_Image->GetAnimation2D();
	m_ImageAnim->ChangeAnimation("Image_BackgroundWhite");


	//m_ImageAnim->CreateAnimation2D<CImageObject>();

	return true;
}

void CImageObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	
}

void CImageObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CImageObject::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CImageObject::Render(float DeltaTime)
{
	CGameObject::Render(DeltaTime);
}

CImageObject* CImageObject::Clone()
{
	return new CImageObject(*this);
}



