#pragma once

#include "GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/Camera.h"
#include "Component/SpringArm2D.h"
#include "Component/ColliderBox2D.h"
#include "Component/WidgetComponent.h"

class CMapCamera :
	public CGameObject
{
	friend class CScene;

protected:
	CMapCamera();
	CMapCamera(const CMapCamera& obj);
	virtual ~CMapCamera();

protected:
	CSharedPtr<CSpringArm2D> m_Arm;
	CSharedPtr<CCamera> m_Camera;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CMapCamera* Clone();
	virtual void Animation2DNotify(const std::string& Name);

public:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void RotationZ(float DeltaTime);
	void RotationZInv(float DeltaTime);


public:
	void AnimationFrameEnd(const std::string& Name);
};

