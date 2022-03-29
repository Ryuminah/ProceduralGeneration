#include "Navigation3D.h"
#include "Component/LandScapeComponent.h"

CNavigation3D::CNavigation3D()
{
}

CNavigation3D::~CNavigation3D()
{
}

void CNavigation3D::SetLandScape(CLandScapeComponent* LandScape)
{
	m_LandScapeList.push_back(LandScape);
}

void CNavigation3D::DeleteLandScape(CLandScapeComponent* LandScape)
{
	auto    iter = m_LandScapeList.begin();
	auto    iterEnd = m_LandScapeList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == LandScape)
		{
			m_LandScapeList.erase(iter);
			break;
		}
	}
}

float CNavigation3D::GetHeight(const Vector3& Pos)
{
	// 여러 지형정보 중에서 현재 위치에 해당하는 지형을 판단한다.
	CLandScapeComponent* LandScape = m_LandScapeList.front();

	// 위치를 이용해서 어떤 사각형 안에 있는지를 판단한다.
	Vector3 LandScapeStartPos = LandScape->GetWorldPos();
	Vector3 LandScapeScale = LandScape->GetWorldScale();

	Vector3 ConvertPos = (Pos - LandScapeStartPos) / LandScapeScale;

	ConvertPos.z = LandScape->m_CountZ - 1 - ConvertPos.z;

	int IndexX = (int)ConvertPos.x;
	int IndexZ = (int)ConvertPos.z;
	int Index = IndexZ * LandScape->m_CountX + IndexX;

	if (IndexX < 0 || IndexX >= LandScape->m_CountX - 1)
		return 0.f;

	else if (IndexZ < 0 || IndexZ >= LandScape->m_CountZ - 1)
		return 0.f;

	float	Y[4] = {};

	Y[0] = LandScape->m_vecVtx[Index].Pos.y;
	Y[1] = LandScape->m_vecVtx[Index + 1].Pos.y;
	Y[2] = LandScape->m_vecVtx[Index + LandScape->m_CountX].Pos.y;
	Y[3] = LandScape->m_vecVtx[Index + LandScape->m_CountX + 1].Pos.y;

	// 어떤 삼각형 안에 있는지를 판단한다.
	float   RatioX = ConvertPos.x - (int)ConvertPos.x;
	float   RatioZ = ConvertPos.z - (int)ConvertPos.z;

	// 우 상단 삼각형일 경우
	if (RatioX >= RatioZ)
		return Y[0] + (Y[1] - Y[0]) * RatioX + (Y[3] - Y[1]) * RatioZ;

	// 좌 하단 삼각형일 경우
	return Y[0] + (Y[3] - Y[2]) * RatioX + (Y[2] - Y[0]) * RatioZ;
}

bool CNavigation3D::FindPath(const Vector3& Start, const Vector3& Goal,
	std::vector<Vector3>& vecPath)
{
	return false;
}
