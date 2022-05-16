#pragma once
#include "GameObject.h"
#include "Component/SpriteComponent.h"

class CImageObject :
    public CGameObject
{
	friend class CScene;

protected:
	CImageObject();
	CImageObject(const CImageObject& obj);
	virtual ~CImageObject();

protected:
	CSharedPtr<CSpriteComponent> m_Image;
	class CAnimation2D* m_ImageAnim;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CImageObject* Clone();
};

