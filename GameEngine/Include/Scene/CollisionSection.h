#pragma once

#include "../GameEngine.h"

class CCollisionSection
{
	friend class CSceneCollision;

private:
	CCollisionSection();
	~CCollisionSection();

private:
	std::vector<class CCollider*>	m_vecCollider;
	Vector3			m_SectionSize;
	Vector3			m_SectionTotalSize;
	Vector3			m_Min;
	Vector3			m_Max;
	int				m_IndexX;
	int				m_IndexY;
	int				m_IndexZ;
	int				m_Index;

public:
	bool Init(int IndexX, int IndexY, int IndexZ, int Index,
		const Vector3& SectionSize, const Vector3& SectionTotalSize);
	void SetSectionMin(const Vector3& Min);
	void SetSectionMax(const Vector3& Max);
	void AddCollider(class CCollider* Collider);
	void Clear();
	void Collision(float DeltaTime);
	class CCollider* CollisionMouse(bool Is2D, float DeltaTime);

private:
	static int SortY(const void* Src, const void* Dest);
	static int SortZ(const void* Src, const void* Dest);
};

