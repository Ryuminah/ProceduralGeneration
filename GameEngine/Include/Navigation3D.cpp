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
	// ���� �������� �߿��� ���� ��ġ�� �ش��ϴ� ������ �Ǵ��Ѵ�.
	CLandScapeComponent* LandScape = m_LandScapeList.front();

	// ��ġ�� �̿��ؼ� � �簢�� �ȿ� �ִ����� �Ǵ��Ѵ�.
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

	// � �ﰢ�� �ȿ� �ִ����� �Ǵ��Ѵ�.
	float   RatioX = ConvertPos.x - (int)ConvertPos.x;
	float   RatioZ = ConvertPos.z - (int)ConvertPos.z;

	// �� ��� �ﰢ���� ���
	if (RatioX >= RatioZ)
		return Y[0] + (Y[1] - Y[0]) * RatioX + (Y[3] - Y[1]) * RatioZ;

	// �� �ϴ� �ﰢ���� ���
	return Y[0] + (Y[3] - Y[2]) * RatioX + (Y[2] - Y[0]) * RatioZ;
}

bool CNavigation3D::FindPath(const Vector3& Start, const Vector3& Goal,
	std::vector<Vector3>& vecPath)
{
	return false;
}
