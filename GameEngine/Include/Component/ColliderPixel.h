#pragma once
#include "Collider.h"
class CColliderPixel :
    public CCollider
{
	friend class CGameObject;

protected:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& com);
	virtual ~CColliderPixel();

protected:
	PixelInfo	m_Info;

public:
	PixelInfo GetInfo()	const
	{
		return m_Info;
	}

public:
	void SetTexture(const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const TCHAR* FullPath);
	void SetPixelInfoColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a);
	void SetPixelCollisionType(PixelCollision_Type Type);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual CColliderPixel* Clone();

public:
	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionMouse(const Vector2& MousePos);
};

