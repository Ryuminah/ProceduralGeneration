
#include "SceneCollision.h"
#include "CameraManager.h"
#include "Scene.h"
#include "../Render/RenderManager.h"
#include "CollisionSection.h"
#include "../Input.h"

CSceneCollision::CSceneCollision()	:
	m_Section2D(nullptr),
	m_Section3D(nullptr),
	m_Move(false)
{
}

CSceneCollision::~CSceneCollision()
{
	if (m_Section2D)
	{
		size_t	Size = m_Section2D->vecSection.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_Section2D->vecSection[i]);
		}

		SAFE_DELETE(m_Section2D);
	}

	if (m_Section3D)
	{
		size_t	Size = m_Section3D->vecSection.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_Section3D->vecSection[i]);
		}

		SAFE_DELETE(m_Section3D);
	}
}

void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_ColliderList.push_back(Collider);
}

void CSceneCollision::AddWidget(CWidget* Widget)
{
	m_vecWidget.push_back(Widget);
}

void CSceneCollision::CheckCollisionSection()
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	Vector3	SectionMin = m_Center - m_SectionTotalSize / 2.f;

	for (; iter != iterEnd; ++iter)
	{
		// ���� �浹ü�� �浹 ������ �����ش�.
		CCollider* Collider = *iter;

		Vector3	Min = Collider->GetMin();
		Vector3	Max = Collider->GetMax();

		Min -= SectionMin;
		Max -= SectionMin;

		CollisionSectionInfo* SectionInfo = m_Section2D;

		if (Collider->GetColliderSpace() == Collider_Space::Collider3D)
			SectionInfo = m_Section3D;

		// �浹ü�� Min, Max ���� �̿��ؼ� �Ҽӵ� �浹������ �ε����� ���Ѵ�.
		int	IndexMinX, IndexMinY, IndexMinZ;
		int	IndexMaxX, IndexMaxY, IndexMaxZ;

		IndexMinX = (int)(Min.x / SectionInfo->SectionSize.x);
		IndexMinY = (int)(Min.y / SectionInfo->SectionSize.y);
		IndexMinZ = (int)(Min.z / SectionInfo->SectionSize.z);

		IndexMaxX = (int)(Max.x / SectionInfo->SectionSize.x);
		IndexMaxY = (int)(Max.y / SectionInfo->SectionSize.y);
		IndexMaxZ = (int)(Max.z / SectionInfo->SectionSize.z);

		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;
		IndexMinZ = IndexMinZ < 0 ? 0 : IndexMinZ;

		IndexMaxX = IndexMaxX >= SectionInfo->CountX ? SectionInfo->CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= SectionInfo->CountY ? SectionInfo->CountY - 1 : IndexMaxY;
		IndexMaxZ = IndexMaxZ >= SectionInfo->CountZ ? SectionInfo->CountZ - 1 : IndexMaxZ;

		for (int z = IndexMinZ; z <= IndexMaxZ; ++z)
		{
			for (int y = IndexMinY; y <= IndexMaxY; ++y)
			{
				for (int x = IndexMinX; x <= IndexMaxX; ++x)
				{
					int	Index = z * (SectionInfo->CountX * SectionInfo->CountY) +
						y * SectionInfo->CountX + x;

					SectionInfo->vecSection[Index]->AddCollider(Collider);
				}
			}
		}
	}
}

void CSceneCollision::CreateSection2D()
{
	m_Section2D = new CollisionSectionInfo;

	m_Section2D->CountX = 10;
	m_Section2D->CountY = 10;
	m_Section2D->CountZ = 1;
	m_Section2D->SectionSize = Vector3(1000.f, 1000.f, 1.f);
	m_Section2D->SectionTotalSize = Vector3(10000.f, 10000.f, 1.f);

	m_SectionTotalSize = m_Section2D->SectionTotalSize;

	m_Section2D->Min = Vector3(-5000.f, -5000.f, 0.f);
	m_Section2D->Max = Vector3(5000.f, 5000.f, 0.f);

	for (int i = 0; i < m_Section2D->CountY; ++i)
	{
		for (int j = 0; j < m_Section2D->CountX; ++j)
		{
			CCollisionSection* Section = new CCollisionSection;

			m_Section2D->vecSection.push_back(Section);

			Section->m_SectionSize = m_Section2D->SectionSize;
			Section->m_SectionTotalSize = m_Section2D->SectionTotalSize;
			Section->m_Min = m_Section2D->Min + m_Section2D->SectionSize * Vector3((float)j, (float)i, 0.f);
			Section->m_Max = Section->m_Min + m_Section2D->SectionSize;
			Section->m_IndexX = j;
			Section->m_IndexY = i;
			Section->m_IndexZ = 0;
			Section->m_Index = i * 10 + j;
		}
	}
}

void CSceneCollision::CreateSection3D()
{
	m_Section3D = new CollisionSectionInfo;

	m_Section3D->CountX = 5;
	m_Section3D->CountY = 5;
	m_Section3D->CountZ = 5;
	m_Section3D->SectionSize = Vector3(300.f, 300.f, 300.f);
	m_Section3D->SectionTotalSize = Vector3(1500.f, 1500.f, 1500.f);

	m_SectionTotalSize = m_Section3D->SectionTotalSize;

	m_Section3D->Min = Vector3(-750.f, -750.f, -750.f);
	m_Section3D->Max = Vector3(750.f, 750.f, 750.f);

	for (int k = 0; k < m_Section3D->CountZ; ++k)
	{
		for (int i = 0; i < m_Section3D->CountY; ++i)
		{
			for (int j = 0; j < m_Section3D->CountX; ++j)
			{
				CCollisionSection* Section = new CCollisionSection;

				m_Section3D->vecSection.push_back(Section);

				Section->m_SectionSize = m_Section3D->SectionSize;
				Section->m_SectionTotalSize = m_Section3D->SectionTotalSize;
				Section->m_Min = m_Section3D->Min + m_Section3D->SectionSize * Vector3((float)j, (float)i, (float)k);
				Section->m_Max = Section->m_Min + m_Section3D->SectionSize;
				Section->m_IndexX = j;
				Section->m_IndexY = i;
				Section->m_IndexZ = k;
				Section->m_Index = k * (5 * 5) + i * 5 + j;
			}
		}
	}
}

void CSceneCollision::CalculateSectionInfo()
{
	CollisionSectionInfo* SectionInfo = m_Section2D;

	if (CRenderManager::GetInst()->GetRenderSpace() == Render_Space::Render3D)
		SectionInfo = m_Section3D;

	SectionInfo->Min = SectionInfo->Center - SectionInfo->SectionTotalSize / 2.f;
	SectionInfo->Max = SectionInfo->Center + SectionInfo->SectionTotalSize / 2.f;

	for (int k = 0; k < SectionInfo->CountZ; ++k)
	{
		for (int i = 0; i < SectionInfo->CountY; ++i)
		{
			for (int j = 0; j < SectionInfo->CountX; ++j)
			{
				int	Index = k * (SectionInfo->CountX * SectionInfo->CountY) +
					i * SectionInfo->CountX + j;

				SectionInfo->vecSection[Index]->m_Min = SectionInfo->Min + SectionInfo->SectionSize * Vector3((float)j, (float)i, (float)k);
				SectionInfo->vecSection[Index]->m_Max = SectionInfo->vecSection[Index]->m_Min + SectionInfo->SectionSize;
			}
		}
	}
}

void CSceneCollision::Start()
{
	if (CRenderManager::GetInst()->GetRenderSpace() == Render_Space::Render2D)
	{
		CreateSection2D();
	}

	else
	{
		CreateSection3D();
	}
}

bool CSceneCollision::Init()
{
	return true;
}

void CSceneCollision::Collision(float DeltaTime)
{
	if (m_Move)
	{
		// Section�� Min, Max�� �ٽ� �����ش�.
		CalculateSectionInfo();
	}


	// UI ó��
	auto	iterUI = m_vecWidget.begin();
	auto	iterUIEnd = m_vecWidget.end();

	for (; iterUI != iterUIEnd;)
	{
		if (!(*iterUI)->IsActive())
		{
			if (*iterUI == m_MouseCollisionWidget)
			{
				m_MouseCollisionWidget = nullptr;
			}

			iterUI = m_vecWidget.erase(iterUI);
			iterUIEnd = m_vecWidget.end();
			continue;
		}

		++iterUI;
	}

	// �浹ü ��ü�� �ݺ��ϸ� ���ŵǾ� ��� �浹ü���� ���Ÿ� ���ְ�
	// �� �浹ü���� ��� ������ ���ϴ����� �Ǵ��Ͽ� �߰����ֵ��� �Ѵ�.
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			if (*iter == m_MouseCollisionCollider)
			{
				m_MouseCollisionCollider = nullptr;
			}

			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		++iter;
	}

	CheckCollisionSection();

	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetCurrentSectionCheck())
			continue;

		(*iter)->CurrentSectionCheck();

		// ���� �����ӿ� �浹�Ǿ��� �浹ü���� üũ�غ���.
		(*iter)->CheckPrevColliderSection();
	}

	// ���콺�� ��ü �浹
	CollisionMouse(DeltaTime);

	CollisionSectionInfo* SectionInfo = m_Section2D;

	if (CRenderManager::GetInst()->GetRenderSpace() == Render_Space::Render3D)
		SectionInfo = m_Section3D;

	size_t	Size = SectionInfo->vecSection.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SectionInfo->vecSection[i]->Collision(DeltaTime);
		SectionInfo->vecSection[i]->Clear();
	}






	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearFrame();
	}


	m_Move = false;
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	// �����Ѵ�.
	if (m_vecWidget.size() > 1)
	{
		qsort(&m_vecWidget[0], m_vecWidget.size(), (size_t)sizeof(CSharedPtr<CWidget>),
			CSceneCollision::SortZOrder);
	}

	bool	MouseCollision = false;

	Vector2	MousePos = CInput::GetInst()->GetMousePos();

	auto iterUI = m_vecWidget.begin();
	auto iterUIEnd = m_vecWidget.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		Vector2	Pos = (*iterUI)->GetPos();
		Vector2	Size = (*iterUI)->GetSize();

		if (Pos.x <= MousePos.x && MousePos.x <= Pos.x + Size.x &&
			Pos.y <= MousePos.y && MousePos.y <= Pos.y + Size.y)
		{
			MouseCollision = true;

			if (m_MouseCollisionWidget != *iterUI)
			{
				(*iterUI)->CollisionMouse(MousePos, DeltaTime);

				if (m_MouseCollisionWidget)
					m_MouseCollisionWidget->CollisionReleaseMouse(MousePos, DeltaTime);
			}

			m_MouseCollisionWidget = *iterUI;

			if (m_MouseCollisionCollider)
			{
				m_MouseCollisionCollider->CallCollisionMouseCallback(Collision_State::End, MousePos);
				m_MouseCollisionCollider = nullptr;
			}

			break;
		}
	}

	// ������� ��ü�� ���콺�� ������ �浹�� �ȵǾ��� ��쿡�� ó���Ѵ�.
	if (!MouseCollision)
	{
		if (m_MouseCollisionWidget)
		{
			m_MouseCollisionWidget->CollisionReleaseMouse(MousePos, DeltaTime);
			m_MouseCollisionWidget = nullptr;
		}

		// ���콺�� �浹 ������ ��� ������ �����ϴ����� �Ǵ��Ѵ�.
		if (CRenderManager::GetInst()->GetRenderSpace() == Render_Space::Render2D)
		{
			MousePos = CInput::GetInst()->GetMouse2DWorldPos();

			Vector2	ConvertMousePos = MousePos;

			ConvertMousePos.x -= m_Section2D->Min.x;
			ConvertMousePos.y -= m_Section2D->Min.y;

			int	IndexX, IndexY;
			IndexX = (int)(ConvertMousePos.x / m_Section2D->SectionSize.x);
			IndexY = (int)(ConvertMousePos.y / m_Section2D->SectionSize.y);

			if (0 <= IndexX && IndexX < m_Section2D->CountX &&
				0 <= IndexY && IndexY < m_Section2D->CountY)
			{
				int	Index = IndexY * m_Section2D->CountX + IndexX;

				CCollider* MouseCollisionCollider = m_Section2D->vecSection[Index]->CollisionMouse(true, DeltaTime);

				if (MouseCollisionCollider)
				{
					MouseCollision = true;

					MouseCollisionCollider->CallCollisionMouseCallback(Collision_State::Begin,
						MousePos);

					if (m_MouseCollisionCollider &&
						m_MouseCollisionCollider != MouseCollisionCollider)
					{
						m_MouseCollisionCollider->CallCollisionMouseCallback(Collision_State::End,
							MousePos);
					}

					m_MouseCollisionCollider = MouseCollisionCollider;
				}
			}
		}

		else
		{
			int	Count = m_Section3D->CountX * m_Section3D->CountY * m_Section3D->CountZ;

			for (int i = 0; i < Count; ++i)
			{
				CCollider* MouseCollisionCollider = m_Section3D->vecSection[i]->CollisionMouse(false, DeltaTime);

				if (MouseCollisionCollider)
				{
					MouseCollision = true;

					MouseCollisionCollider->CallCollisionMouseCallback(Collision_State::Begin,
						MousePos);

					if (m_MouseCollisionCollider &&
						m_MouseCollisionCollider != MouseCollisionCollider)
					{
						m_MouseCollisionCollider->CallCollisionMouseCallback(Collision_State::End,
							MousePos);
					}

					m_MouseCollisionCollider = MouseCollisionCollider;
				}
			}			
		}
	}

	// ������ �浹�� ��ü�� ���ٸ�
	if (!MouseCollision)
	{
		if (m_MouseCollisionWidget)
		{
			m_MouseCollisionWidget->CollisionReleaseMouse(MousePos, DeltaTime);
			m_MouseCollisionWidget = nullptr;
		}

		if (m_MouseCollisionCollider)
		{
			m_MouseCollisionCollider->CallCollisionMouseCallback(Collision_State::End,
				MousePos);
			m_MouseCollisionCollider = nullptr;
		}
	}
}

int CSceneCollision::SortZOrder(const void* Src, const void* Dest)
{
	CWidget* SrcUI = *((CSharedPtr<CWidget>*)Src);
	CWidget* DestUI = *((CSharedPtr<CWidget>*)Dest);

	int	SrcZ = SrcUI->GetZOrder();
	int	DestZ = DestUI->GetZOrder();

	if (SrcZ < DestZ)
		return 1;

	else if (SrcZ > DestZ)
		return -1;

	return 0;
}
