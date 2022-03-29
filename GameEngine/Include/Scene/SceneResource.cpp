
#include "SceneResource.h"
#include "../Resource/ResourceManager.h"

CSceneResource::CSceneResource()    :
	m_pScene(nullptr)
{
}

CSceneResource::~CSceneResource()
{
	{
		auto    iter = m_mapMesh.begin();
		auto    iterEnd = m_mapMesh.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseMesh(iter->first);
		}
	}

	{
		auto    iter = m_mapMaterial.begin();
		auto    iterEnd = m_mapMaterial.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseMaterial(iter->first);
		}
	}

	{
		auto    iter = m_mapTexture.begin();
		auto    iterEnd = m_mapTexture.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseTexture(iter->first);
		}
	}

	{
		auto    iter = m_mapAnimationSequence2D.begin();
		auto    iterEnd = m_mapAnimationSequence2D.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseAnimationSequence2D(iter->first);
		}
	}

	{
		auto    iter = m_mapParticle.begin();
		auto    iterEnd = m_mapParticle.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseParticle(iter->first);
		}
	}

	{
		auto    iter = m_mapSound.begin();
		auto    iterEnd = m_mapSound.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseSound(iter->first);
		}
	}

	{
		auto    iter = m_mapAnimationSequence.begin();
		auto    iterEnd = m_mapAnimationSequence.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Release();
			CResourceManager::GetInst()->ReleaseAnimationSequence(iter->first);
		}
	}
}

bool CSceneResource::Init()
{
	CMesh* pMesh = CResourceManager::GetInst()->FindMesh("ColorRect");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("ColorRect", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("TextureRect");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("TextureRect", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("ColliderBox2D");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("ColliderBox2D", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("ColliderCircle");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("ColliderCircle", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("SpherePos");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("SpherePos", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("CubeLine");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("CubeLine", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("Cube");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("Cube", pMesh));
	}

	pMesh = CResourceManager::GetInst()->FindMesh("BillboardMesh");

	if (pMesh)
	{
		pMesh->m_pScene = m_pScene;
		pMesh->AddRef();
		m_mapMesh.insert(std::make_pair("BillboardMesh", pMesh));
	}

	CMaterial* pMaterial = CResourceManager::GetInst()->FindMaterial("DefaultMaterial");

	if (pMaterial)
	{
		pMaterial->m_pScene = m_pScene;
		pMaterial->AddRef();
		m_mapMaterial.insert(std::make_pair("DefaultMaterial", pMaterial));
	}

	pMaterial = CResourceManager::GetInst()->FindMaterial("DefaultSky");

	if (pMaterial)
	{
		pMaterial->m_pScene = m_pScene;
		pMaterial->AddRef();
		m_mapMaterial.insert(std::make_pair("DefaultSky", pMaterial));
	}
	
	pMaterial = CResourceManager::GetInst()->FindMaterial("DefaultDecal");

	if (pMaterial)
	{
		pMaterial->m_pScene = m_pScene;
		pMaterial->AddRef();
		m_mapMaterial.insert(std::make_pair("DefaultDecal", pMaterial));
	}
	
	pMaterial = CResourceManager::GetInst()->FindMaterial("DefaultDecalDebug");

	if (pMaterial)
	{
		pMaterial->m_pScene = m_pScene;
		pMaterial->AddRef();
		m_mapMaterial.insert(std::make_pair("DefaultDecalDebug", pMaterial));
	}

	return true;
}

bool CSceneResource::CreateMesh(Mesh_Type Type, const std::string& Name,
	void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY Primitive, void* pIndices, int IdxCount, int IdxSize, 
	D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	CMesh* pMesh = FindMesh(Name);

	if (pMesh)
		return true;

	if (!CResourceManager::GetInst()->CreateMesh(Type, Name, pVertices, VtxCount,
		VtxSize, VtxUsage, Primitive, pIndices, IdxCount, IdxSize,
		IdxUsage, Fmt))
		return false;

	pMesh = CResourceManager::GetInst()->FindMesh(Name);

	pMesh->m_pScene = m_pScene;
	pMesh->AddRef();

	m_mapMesh.insert(std::make_pair(Name, pMesh));

	return true;
}

bool CSceneResource::LoadMesh(Mesh_Type Type, const std::string& Name, const TCHAR* Filename, 
	const std::string& PathName)
{
	CMesh* pMesh = FindMesh(Name);

	if (pMesh)
		return true;

	if (!CResourceManager::GetInst()->LoadMesh(m_pScene, Type, Name, Filename, PathName))
		return false;

	pMesh = CResourceManager::GetInst()->FindMesh(Name);

	pMesh->m_pScene = m_pScene;
	pMesh->AddRef();

	m_mapMesh.insert(std::make_pair(Name, pMesh));

	return true;
}

bool CSceneResource::LoadMeshFullPath(Mesh_Type Type, const std::string& Name, const TCHAR* FullPath)
{
	CMesh* pMesh = FindMesh(Name);

	if (pMesh)
		return true;

	if (!CResourceManager::GetInst()->LoadMeshFullPath(m_pScene, Type, Name, FullPath))
		return false;

	pMesh = CResourceManager::GetInst()->FindMesh(Name);

	pMesh->m_pScene = m_pScene;
	pMesh->AddRef();

	m_mapMesh.insert(std::make_pair(Name, pMesh));

	return true;
}

bool CSceneResource::LoadMeshMultibyte(Mesh_Type Type, const std::string& Name, const char* Filename, const std::string& PathName)
{
	CMesh* pMesh = FindMesh(Name);

	if (pMesh)
		return true;

	if (!CResourceManager::GetInst()->LoadMeshMultibyte(m_pScene, Type, Name, Filename, PathName))
		return false;

	pMesh = CResourceManager::GetInst()->FindMesh(Name);

	pMesh->m_pScene = m_pScene;
	pMesh->AddRef();

	m_mapMesh.insert(std::make_pair(Name, pMesh));

	return true;
}

bool CSceneResource::LoadMeshFullPathMultibyte(Mesh_Type Type, const std::string& Name, const char* FullPath)
{
	CMesh* pMesh = FindMesh(Name);

	if (pMesh)
		return true;

	if (!CResourceManager::GetInst()->LoadMeshFullPathMultibyte(m_pScene, Type, Name, FullPath))
		return false;

	pMesh = CResourceManager::GetInst()->FindMesh(Name);

	pMesh->m_pScene = m_pScene;
	pMesh->AddRef();

	m_mapMesh.insert(std::make_pair(Name, pMesh));

	return true;
}

bool CSceneResource::SetAnimationMeshSkeleton(const std::string& MeshName, const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return CResourceManager::GetInst()->SetAnimationMeshSkeleton(m_pScene, MeshName, Name, FileName, PathName);
}

bool CSceneResource::SetAnimationMeshSkeleton(const std::string& MeshName, CSkeleton* Skeleton)
{
	return CResourceManager::GetInst()->SetAnimationMeshSkeleton(m_pScene, MeshName, Skeleton);
}

CMesh* CSceneResource::FindMesh(const std::string& Name)
{
	auto    iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::CreateMaterial(const std::string& Name)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (pMaterial)
		return true;

	if (!CResourceManager::GetInst()->CreateMaterial(Name))
		return false;

	pMaterial = CResourceManager::GetInst()->FindMaterial(Name);

	pMaterial->m_pScene = m_pScene;
	pMaterial->AddRef();

	m_mapMaterial.insert(std::make_pair(Name, pMaterial));

	return true;
}

bool CSceneResource::SetMaterialTransparency(const std::string& Name, 
	bool bTransparency)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetTransparency(bTransparency);

	return true;
}

bool CSceneResource::SetMaterialOpacity(const std::string& Name, 
	float Opacity)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetOpacity(Opacity);

	return true;
}

bool CSceneResource::SetMaterialBaseColor(const std::string& Name, const Vector4& Color)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetBaseColor(Color);

	return true;
}

bool CSceneResource::SetMaterialBaseColor(const std::string& Name, float r, float g, float b, float a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetBaseColor(r, g, b, a);

	return true;
}

bool CSceneResource::SetMaterialBaseColor(const std::string& Name, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetBaseColor(r, g, b, a);

	return true;
}

bool CSceneResource::SetMaterialEmissiveColor(const std::string& Name, const Vector4& Color)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetEmissiveColor(Color);

	return true;
}

bool CSceneResource::SetMaterialEmissiveColor(const std::string& Name, float r, float g, float b, float a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetEmissiveColor(r, g, b, a);

	return true;
}

bool CSceneResource::SetMaterialEmissiveColor(const std::string& Name, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetEmissiveColor(r, g, b, a);

	return true;
}

bool CSceneResource::SetMaterialShader(const std::string& MaterialName, const std::string& ShaderName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetShader(ShaderName);

	return true;
}

bool CSceneResource::AddMaterialTexture(const std::string& MaterialName,
	const std::string& TextureName, const TCHAR* FileName,
	const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTexture(TextureName, FileName, PathName);

	return true;
}

bool CSceneResource::AddMaterialTextureFullPath(const std::string& MaterialName,
	const std::string& TextureName, const TCHAR* FullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureFullPath(TextureName, FullPath);

	return true;
}

bool CSceneResource::AddMaterialTexture(const std::string& MaterialName,
	const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTexture(TextureName, vecFileName, PathName);

	return true;
}

bool CSceneResource::AddMaterialTextureFullPath(const std::string& MaterialName, 
	const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureFullPath(TextureName, vecFullPath);

	return true;
}

bool CSceneResource::AddMaterialTextureArray(const std::string& MaterialName,
	const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureArray(TextureName, vecFileName, PathName);

	return true;
}

bool CSceneResource::AddMaterialTextureArrayFullPath(
	const std::string& MaterialName, const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureArrayFullPath(TextureName, vecFullPath);

	return true;
}

bool CSceneResource::SetMaterialTexture(const std::string& MaterialName, 
	const std::string& FindName, const std::string& TextureName, const TCHAR* FileName,
	const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTexture(FindName, TextureName, FileName, PathName);

	return true;
}

bool CSceneResource::SetMaterialTextureFullPath(const std::string& MaterialName, 
	const std::string& FindName, const std::string& TextureName, const TCHAR* FullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureFullPath(FindName, TextureName, FullPath);

	return true;
}

bool CSceneResource::SetMaterialTexture(const std::string& MaterialName, 
	const std::string& FindName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTexture(FindName, TextureName, vecFileName, PathName);

	return true;
}

bool CSceneResource::SetMaterialTextureFullPath(const std::string& MaterialName, 
	const std::string& FindName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureFullPath(FindName, TextureName, vecFullPath);

	return true;
}

bool CSceneResource::SetMaterialTextureArray(const std::string& MaterialName, 
	const std::string& FindName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureArray(FindName, TextureName, vecFileName, PathName);

	return true;
}

bool CSceneResource::SetMaterialTextureArrayFullPath(
	const std::string& FindName, const std::string& MaterialName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureArrayFullPath(FindName, TextureName, vecFullPath);

	return true;
}

bool CSceneResource::SetMaterialTextureLink(const std::string& MaterialName,
	const std::string& TextureName, Texture_Link Link)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureLink(TextureName, Link);

	return true;
}

bool CSceneResource::SetMaterialTextureRegister(const std::string& MaterialName, 
	const std::string& TextureName, int Register)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureRegister(TextureName, Register);

	return true;
}

bool CSceneResource::SetMaterialTextureShaderType(const std::string& MaterialName,
	const std::string& TextureName, int ShaderType)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureShaderType(TextureName, ShaderType);

	return true;
}

CMaterial* CSceneResource::FindMaterial(const std::string& Name)
{
	auto    iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CSceneResource::LoadTexture(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CSceneResource::LoadTextureArray(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CSceneResource::LoadTextureArrayFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CSceneResource::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	if (!CResourceManager::GetInst()->CreateTarget(Name, Width, Height, PixelFormat))
		return false;

	pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->m_pScene = m_pScene;
	pTexture->AddRef();

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto    iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& SequenceName,
	float PlayTime, float PlayRate)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(SequenceName, PlayTime,
		PlayRate))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	pSequence->m_pScene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence2D.insert(std::make_pair(SequenceName, pSequence));

	return true;
}

bool CSceneResource::SetAnimationSequence2DTexture(const std::string& SequenceName, 
	const std::string& TextureName)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::SetAnimationSequence2DTexture(const std::string& SequenceName,
	const std::string& TextureName, const TCHAR* pFileName, const std::string& PathName)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	if (!LoadTexture(TextureName, pFileName, PathName))
		return false;

	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::SetAnimationSequence2DTextureFullPath(const std::string& SequenceName,
	const std::string& TextureName, const TCHAR* FullPath)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	if (!LoadTextureFullPath(TextureName, FullPath))
		return false;

	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::SetAnimationSequence2DTexture(const std::string& SequenceName, 
	const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	if (!LoadTexture(TextureName, vecFileName, PathName))
		return false;

	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::SetAnimationSequence2DTextureFullPath(
	const std::string& SequenceName, const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	if (!LoadTextureFullPath(TextureName, vecFullPath))
		return false;

	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::SetAnimationSequence2DTextureArray(const std::string& SequenceName, const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	if (!LoadTextureArray(TextureName, vecFileName, PathName))
		return false;

	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::SetAnimationSequence2DTextureArrayFullPath(const std::string& SequenceName, const std::string& TextureName, const std::vector<const TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;


	if (!LoadTextureArrayFullPath(TextureName, vecFullPath))
		return false;

	CTexture* pTexture = FindTexture(TextureName);

	if (!pTexture)
		return false;

	pSequence->SetTexture(pTexture);

	return true;
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& SequenceName, 
	const Vector2& Start, const Vector2& End)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;

	pSequence->AddFrame(Start, End);

	return true;
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& SequenceName, const Vector2& Start, const Vector2& End, int Count)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;

	pSequence->AddFrame(Start, End, Count);

	return true;
}

bool CSceneResource::AddAnimationSequence2DNotify(const std::string& SequenceName,
	const std::string& NotifyName, int Frame)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;

	pSequence->AddNotify(NotifyName, Frame);

	return true;
}

CAnimationSequence2D* CSceneResource::FindAnimationSequence2D(const std::string& Name)
{
	auto    iter = m_mapAnimationSequence2D.find(Name);

	if (iter == m_mapAnimationSequence2D.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::CreateParticle(const std::string& Name)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (Particle)
		return true;

	if (!CResourceManager::GetInst()->CreateParticle(Name))
		return false;

	Particle = CResourceManager::GetInst()->FindParticle(Name);

	Particle->SetScene(m_pScene);
	Particle->AddRef();

	m_mapParticle.insert(std::make_pair(Name, Particle));

	return true;
}

void CSceneResource::SetParticleMaterial(const std::string& Name, CMaterial* Material)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMaterial(Material);
}

void CSceneResource::SetParticleMaxParticleCount(const std::string& Name, 
	unsigned int Count)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMaxParticleCount(Count);
}

void CSceneResource::SetParticleStartColor(const std::string& Name, 
	const Vector4& Color)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartColor(Color);
}

void CSceneResource::SetParticleStartColor(const std::string& Name, float r, float g, float b, float a)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartColor(r, g, b, a);
}

void CSceneResource::SetParticleEndColor(const std::string& Name, const Vector4& Color)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndColor(Color);
}

void CSceneResource::SetParticleEndColor(const std::string& Name, float r, float g, float b, float a)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndColor(r, g, b, a);
}

void CSceneResource::SetParticleStartScale(const std::string& Name, const Vector3& Scale)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartScale(Scale);
}

void CSceneResource::SetParticleStartScale(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartScale(x, y, z);
}

void CSceneResource::SetParticleEndScale(const std::string& Name, const Vector3& Scale)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndScale(Scale);
}

void CSceneResource::SetParticleEndScale(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndScale(x, y, z);
}

void CSceneResource::SetParticleLifeTimeMin(const std::string& Name, float Min)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetLifeTimeMin(Min);
}

void CSceneResource::SetParticleLifeTimeMax(const std::string& Name, float Max)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetLifeTimeMax(Max);
}

void CSceneResource::SetParticleRange(const std::string& Name, const Vector3& Range)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetRange(Range);
}

void CSceneResource::SetParticleRange(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetRange(x, y, z);
}

void CSceneResource::SetParticleMinSpeed(const std::string& Name, float Min)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMinSpeed(Min);
}

void CSceneResource::SetParticleMaxSpeed(const std::string& Name, float Max)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMaxSpeed(Max);
}

void CSceneResource::SetParticleMoveEnable(const std::string& Name, bool Move)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveEnable(Move);
}

void CSceneResource::SetParticle2D(const std::string& Name, bool b2D)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetParticle2D(b2D);
}

void CSceneResource::SetParticleMoveDir(const std::string& Name,
	const Vector3& Dir)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveDir(Dir);
}

void CSceneResource::SetParticleMoveDir(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveDir(x, y, z);
}

void CSceneResource::SetParticleMoveAngle(const std::string& Name, const Vector3& Angle)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveAngle(Angle);
}

void CSceneResource::SetParticleMoveAngle(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveAngle(x, y, z);
}

CParticleSystem* CSceneResource::FindParticle(const std::string& Name)
{
	auto    iter = m_mapParticle.find(Name);

	if (iter == m_mapParticle.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::LoadSound(const std::string& GroupName, bool Loop,
	const std::string& Name, const char* FileName, 
	const std::string& PathName)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
		return true;

	if (!CResourceManager::GetInst()->LoadSound(GroupName, Loop, Name, FileName, PathName))
		return false;

	Sound = CResourceManager::GetInst()->FindSound(Name);

	Sound->SetScene(m_pScene);
	Sound->AddRef();

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSceneResource::SetVolume(int Volume)
{
	return CResourceManager::GetInst()->SetVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
	return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundResume(Name);
}

FMOD::ChannelGroup* CSceneResource::FindSoundChannelGroup(const std::string& Name)
{
	return CResourceManager::GetInst()->FindSoundChannelGroup(Name);
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
	auto    iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::LoadAnimationSequence(const std::string& Name, bool Loop, 
	_tagFbxAnimationClip* Clip)
{
	CAnimationSequence* pSequence = FindAnimationSequence(Name);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequence(Name, Loop, Clip))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

	pSequence->m_Scene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence.insert(std::make_pair(Name, pSequence));

	return true;
}

bool CSceneResource::LoadAnimationSequence(const std::string& Name, bool Loop,
	int StartFrame, int EndFrame, float PlayTime, 
	const std::vector<BoneKeyFrame*>& vecFrame)
{
	CAnimationSequence* pSequence = FindAnimationSequence(Name);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequence(Name, Loop, StartFrame,
		EndFrame, PlayTime, vecFrame))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

	pSequence->m_Scene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence.insert(std::make_pair(Name, pSequence));

	return true;
}

bool CSceneResource::LoadAnimationSequence(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence* pSequence = FindAnimationSequence(Name);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequence(Name, FileName, PathName))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

	pSequence->m_Scene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence.insert(std::make_pair(Name, pSequence));

	return true;
}

bool CSceneResource::LoadAnimationSequenceFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	CAnimationSequence* pSequence = FindAnimationSequence(Name);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequenceFullPath(Name, FullPath))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

	pSequence->m_Scene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence.insert(std::make_pair(Name, pSequence));

	return true;
}

bool CSceneResource::LoadAnimationSequenceMultibyte(const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	CAnimationSequence* pSequence = FindAnimationSequence(Name);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequenceMultibyte(Name, FileName, PathName))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

	pSequence->m_Scene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence.insert(std::make_pair(Name, pSequence));

	return true;
}

bool CSceneResource::LoadAnimationSequenceFullPathMultibyte(const std::string& Name,
	const char* FullPath)
{
	CAnimationSequence* pSequence = FindAnimationSequence(Name);

	if (pSequence)
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequenceFullPathMultibyte(Name, FullPath))
		return false;

	pSequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

	pSequence->m_Scene = m_pScene;
	pSequence->AddRef();

	m_mapAnimationSequence.insert(std::make_pair(Name, pSequence));

	return true;
}

CAnimationSequence* CSceneResource::FindAnimationSequence(const std::string& Name)
{
	auto    iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}
