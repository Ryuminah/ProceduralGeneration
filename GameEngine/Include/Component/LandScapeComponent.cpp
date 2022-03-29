
#include "LandScapeComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"
#include "../Resource/Material.h"
#include "../PathManager.h"
#include "../Resource/Texture.h"
#include "../Navigation3D.h"
#include "../NavigationManager.h"

CLandScapeComponent::CLandScapeComponent()	:
	m_CBuffer(nullptr),
	m_DetailLevel(1.f),
	m_SplatCount(0)
{
	m_PrimitiveType = PrimitiveComponent_Type::Primitive3D;
	m_3DType = RT3D_TERRAIN;

	SetTransformState(Transform_State::None);
}

CLandScapeComponent::CLandScapeComponent(const CLandScapeComponent& com) :
	CPrimitiveComponent(com)
{
	m_SplatCount = com.m_SplatCount;
	m_DetailLevel = com.m_DetailLevel;
	m_pMesh = com.m_pMesh;

	m_CBuffer = com.m_CBuffer->Clone();
}

CLandScapeComponent::~CLandScapeComponent()
{
	SAFE_DELETE(m_CBuffer);

	CNavigation3D* Navigation = (CNavigation3D*)CNavigationManager::GetInst()->GetNavigation();

	Navigation->DeleteLandScape(this);
}

CMesh* CLandScapeComponent::GetMesh() const
{
	return (CMesh*)m_pMesh.Get();
}

void CLandScapeComponent::SetMesh(CMesh* pMesh)
{
	m_pMesh = (CStaticMesh*)pMesh;

	Vector3 Min = m_pMesh->GetMin();
	Vector3 Max = m_pMesh->GetMax();

	m_pTransform->SetMeshSize(Max - Min);

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* pMaterialSlots =
		m_pMesh->GetMaterialSlots();

	auto    iter = pMaterialSlots->begin();
	auto    iterEnd = pMaterialSlots->end();

	for (; iter != iterEnd; ++iter)
	{
		CMaterial* pClone = (*iter)->Clone();

		pClone->SetScene(m_pScene);
		m_vecMaterialSlot.push_back(pClone);

		SAFE_RELEASE(pClone);
	}
}

void CLandScapeComponent::SetMesh(const std::string& Name)
{
	m_pMesh = (CStaticMesh*)m_pScene->GetResource()->FindMesh(Name);

	Vector3 Min = m_pMesh->GetMin();
	Vector3 Max = m_pMesh->GetMax();

	m_pTransform->SetMeshSize(Max - Min);

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* pMaterialSlots =
		m_pMesh->GetMaterialSlots();

	auto    iter = pMaterialSlots->begin();
	auto    iterEnd = pMaterialSlots->end();

	for (; iter != iterEnd; ++iter)
	{
		CMaterial* pClone = (*iter)->Clone();

		pClone->SetScene(m_pScene);
		m_vecMaterialSlot.push_back(pClone);

		SAFE_RELEASE(pClone);
	}
}

void CLandScapeComponent::SetMaterial(int SlotIndex, CMaterial* pMaterial)
{
	CPrimitiveComponent::SetMaterial(SlotIndex, pMaterial);
}

void CLandScapeComponent::SetMaterial(int SlotIndex, const std::string& Name)
{
	CPrimitiveComponent::SetMaterial(SlotIndex, Name);
}

void CLandScapeComponent::AddMaterial(CMaterial* pMaterial)
{
	CPrimitiveComponent::AddMaterial(pMaterial);
}

void CLandScapeComponent::AddMaterial(const std::string& Name)
{
	CPrimitiveComponent::AddMaterial(Name);
}

void CLandScapeComponent::CreateLandScape(const std::string& Name, int CountX, int CountZ,
	float CellWidth, float CellHeight, const TCHAR* HeightMap, 
	const std::string& PathName)
{
	m_CountX = CountX;
	m_CountZ = CountZ;
	m_CellWidth = CellWidth;
	m_CellHeight = CellHeight;

	std::vector<float>	vecY;

	if (HeightMap)
	{
		TCHAR	FullPath[MAX_PATH] = {};

		const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

		if (Path)
			lstrcpy(FullPath, Path->pPath);
		lstrcat(FullPath, HeightMap);

		ScratchImage	Img;

		// 경로에서 확장자를 얻어온다.
		char    strExt[_MAX_EXT] = {};

		char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

		strcpy_s(FullPathMultibyte, pFullPath);

#endif

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
			nullptr, 0, strExt, _MAX_EXT);

		// 확장자를 대문자로 바꿔준다.
		_strupr_s(strExt);

		if (strcmp(strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, nullptr,
				Img)))
			{
				return;
			}
		}

		else if (strcmp(strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(FullPath, nullptr, Img)))
			{
				return;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, nullptr, Img)))
			{
				return;
			}
		}

		const Image* PixelInfo = Img.GetImages();

		for (size_t i = 0; i < PixelInfo->height; ++i)
		{
			for (size_t j = 0; j < PixelInfo->width; ++j)
			{
				int	PixelIndex = (int)i * (int)PixelInfo->width * 4 + (int)j * 4;

				float	Y = PixelInfo->pixels[PixelIndex] / 60.f;

				vecY.push_back(Y);
			}
		}
	}

	else
		vecY.resize(CountX * CountZ);

	// 정점정보를 만들어준다.
	for (int i = 0; i < m_CountZ; ++i)
	{
		for (int j = 0; j < m_CountX; ++j)
		{
			Vertex3D	vtx = {};
			vtx.Pos = Vector3((float)j, vecY[i * m_CountX + j], (float)(m_CountZ - i - 1));
			vtx.Normal = Vector3(0.f, 0.f, 0.f);
			vtx.UV = Vector2(j / (float)(m_CountX - 1), i / (float)(m_CountZ - 1));
			vtx.Tangent = Vector3(1.f, 0.f, 0.f);
			vtx.Binormal = Vector3(0.f, 0.f, -1.f);

			m_vecVtx.push_back(vtx);
		}
	}

	// 인덱스 정보를 얻어온다.
	Vector3	CellPos[3];
	Vector3	Edge1, Edge2;
	unsigned int	TriIndex = 0;

	m_vecFaceNormal.resize((size_t)((m_CountX - 1) * (m_CountZ - 1) * 2));

	for (unsigned int i = 0; i < m_CountZ - 1; ++i)
	{
		for (unsigned int j = 0; j < m_CountX - 1; ++j)
		{
			unsigned int	Index = i * m_CountX + j;

			// 우 상단 삼각형
			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + 1);
			m_vecIndex.push_back(Index + m_CountX + 1);

			CellPos[0] = m_vecVtx[Index].Pos;
			CellPos[1] = m_vecVtx[Index + 1].Pos;
			CellPos[2] = m_vecVtx[Index + m_CountX + 1].Pos;

			Edge1 = CellPos[1] - CellPos[0];
			Edge2 = CellPos[2] - CellPos[0];

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();
			++TriIndex;

			// 좌 하단 삼각형
			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + m_CountX + 1);
			m_vecIndex.push_back(Index + m_CountX);

			CellPos[0] = m_vecVtx[Index].Pos;
			CellPos[1] = m_vecVtx[Index + m_CountX + 1].Pos;
			CellPos[2] = m_vecVtx[Index + m_CountX].Pos;

			Edge1 = CellPos[1] - CellPos[0];
			Edge2 = CellPos[2] - CellPos[0];

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();
			++TriIndex;
		}
	}

	ComputeNormal();

	ComputeTangent();

	// 메쉬를 만들어준다.
	m_pScene->GetResource()->CreateMesh(Mesh_Type::StaticMesh, Name,
		&m_vecVtx[0], (int)m_vecVtx.size(), sizeof(Vertex3D),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&m_vecIndex[0], (int)m_vecIndex.size(),
		sizeof(unsigned int), D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT);

	m_pMesh = (CStaticMesh*)m_pScene->GetResource()->FindMesh(Name);
}

void CLandScapeComponent::ComputeNormal()
{
	size_t	Size = m_vecFaceNormal.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Index0 = m_vecIndex[i * 3];
		unsigned int	Index1 = m_vecIndex[i * 3 + 1];
		unsigned int	Index2 = m_vecIndex[i * 3 + 2];

		m_vecVtx[Index0].Normal += m_vecFaceNormal[i];
		m_vecVtx[Index1].Normal += m_vecFaceNormal[i];
		m_vecVtx[Index2].Normal += m_vecFaceNormal[i];
	}

	Size = m_vecVtx.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecVtx[i].Normal.Normalize();
	}
}

void CLandScapeComponent::ComputeTangent()
{
	// 탄젠트 벡터 구함.
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = m_vecIndex[i * 3];
		int	idx1 = m_vecIndex[i * 3 + 1];
		int	idx2 = m_vecIndex[i * 3 + 2];

		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = m_vecVtx[idx1].Pos.x - m_vecVtx[idx0].Pos.x;
		fVtx1[1] = m_vecVtx[idx1].Pos.y - m_vecVtx[idx0].Pos.y;
		fVtx1[2] = m_vecVtx[idx1].Pos.z - m_vecVtx[idx0].Pos.z;

		fVtx2[0] = m_vecVtx[idx2].Pos.x - m_vecVtx[idx0].Pos.x;
		fVtx2[1] = m_vecVtx[idx2].Pos.y - m_vecVtx[idx0].Pos.y;
		fVtx2[2] = m_vecVtx[idx2].Pos.z - m_vecVtx[idx0].Pos.z;

		float	ftu[2], ftv[2];
		ftu[0] = m_vecVtx[idx1].UV.x - m_vecVtx[idx0].UV.x;
		ftv[0] = m_vecVtx[idx1].UV.y - m_vecVtx[idx0].UV.y;

		ftu[1] = m_vecVtx[idx2].UV.x - m_vecVtx[idx0].UV.x;
		ftv[1] = m_vecVtx[idx2].UV.y - m_vecVtx[idx0].UV.y;

		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vector3	Tangent;
		Tangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		Tangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		Tangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		Tangent.Normalize();

		m_vecVtx[idx0].Tangent = Tangent;
		m_vecVtx[idx1].Tangent = Tangent;
		m_vecVtx[idx2].Tangent = Tangent;

		m_vecVtx[idx0].Binormal = m_vecVtx[idx0].Normal.Cross(Tangent);
		m_vecVtx[idx1].Binormal = m_vecVtx[idx1].Normal.Cross(Tangent);
		m_vecVtx[idx2].Binormal = m_vecVtx[idx2].Normal.Cross(Tangent);

		m_vecVtx[idx0].Binormal.Normalize();
		m_vecVtx[idx1].Binormal.Normalize();
		m_vecVtx[idx2].Binormal.Normalize();
	}
}

void CLandScapeComponent::SetDetailLevel(float Level)
{
	m_DetailLevel = Level;
	m_CBuffer->SetDetailLevel(Level);
}

void CLandScapeComponent::Start()
{
	CPrimitiveComponent::Start();

	CNavigation3D* Navigation = (CNavigation3D*)CNavigationManager::GetInst()->GetNavigation();

	Navigation->SetLandScape(this);
}

bool CLandScapeComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	m_CBuffer = new CLandScapeCBuffer;

	m_CBuffer->Init();

	return true;
}

void CLandScapeComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);
}

void CLandScapeComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CLandScapeComponent::Collision(float DeltaTime)
{
	CPrimitiveComponent::Collision(DeltaTime);
}

void CLandScapeComponent::Render(float DeltaTime)
{
	CPrimitiveComponent::Render(DeltaTime);

	m_CBuffer->SetSplatCount(m_SplatCount);

	m_CBuffer->UpdateCBuffer();

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetMaterial();
	}
}

CLandScapeComponent* CLandScapeComponent::Clone()
{
	return new CLandScapeComponent(*this);
}

void CLandScapeComponent::RenderShadow(float DeltaTime)
{
	CPrimitiveComponent::RenderShadow(DeltaTime);

	//m_CBuffer->SetSplatCount(m_SplatCount);

	//m_CBuffer->UpdateCBuffer();

	for (int i = 0; i < (int)m_vecMaterialSlot.size(); ++i)
	{
		m_vecMaterialSlot[i]->SetShadowMaterial();

		m_pMesh->Render(i);

		m_vecMaterialSlot[i]->ResetShadowMaterial();
	}
}
