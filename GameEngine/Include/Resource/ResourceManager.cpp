
#include "ResourceManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"
#include "AnimationMesh.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Texture.h"
#include "../Device.h"
#include "AnimationSequence2D.h"
#include "ParticleSystem.h"
#include "RenderTarget.h"
#include "Sound.h"
#include "../PathManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()	:
	m_System(nullptr),
	m_MasterGroup(nullptr),
	m_WriteFactory(nullptr),
	m_FontFamilyName{},
	m_FontFamilyNameMultibyte{}
{
}

CResourceManager::~CResourceManager()
{
	{
		auto    iter = m_mapMesh.begin();
		auto    iterEnd = m_mapMesh.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapMaterial.begin();
		auto    iterEnd = m_mapMaterial.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapTexture.begin();
		auto    iterEnd = m_mapTexture.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapSampler.begin();
		auto    iterEnd = m_mapSampler.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapAnimationSequence2D.begin();
		auto    iterEnd = m_mapAnimationSequence2D.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapParticleSystem.begin();
		auto    iterEnd = m_mapParticleSystem.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapSound.begin();
		auto    iterEnd = m_mapSound.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapSoundGroup.begin();
		auto    iterEnd = m_mapSoundGroup.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->release();
		}
	}

	{
		auto    iter = m_mapFont.begin();
		auto    iterEnd = m_mapFont.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapFontColor.begin();
		auto    iterEnd = m_mapFontColor.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto    iter = m_mapFontFile.begin();
		auto    iterEnd = m_mapFontFile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	if (m_System)
	{
		m_System->close();
		m_System->release();
	}

	SAFE_RELEASE(m_WriteFactory);

	CShaderManager::DestroyInst();
}

bool CResourceManager::Init()
{
	if (!CShaderManager::GetInst()->Init())
		return false;

	// fmod 초기화
	FMOD_RESULT	FMODResult = FMOD::System_Create(&m_System);

	if (FMODResult != FMOD_OK)
		return false;

	FMODResult = m_System->init(128, FMOD_INIT_NORMAL, nullptr);

	if (FMODResult != FMOD_OK)
		return false;

	// MasterGroup을 얻어온다.
	FMODResult = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (FMODResult != FMOD_OK)
		return false;

	m_mapSoundGroup.insert(std::make_pair("Master", m_MasterGroup));

	// DirectWrite 팩토리를 생성한다.
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_WriteFactory), (IUnknown**)&m_WriteFactory)))
		return false;


	// Default Color Rectangle
	VertexColor ColorRect[4] =
	{
		VertexColor(0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f),
		VertexColor(1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
		VertexColor(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f),
		VertexColor(1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f)
	};

	int IdxRect[6] = { 0, 1, 3, 0, 3, 2 };

	if (!CreateMesh(Mesh_Type::Sprite, "ColorRect", ColorRect, 4, sizeof(VertexColor), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IdxRect, 6, sizeof(int), D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT))
		return false;

	// Default Texture Rectangle
	VertexUV TextureRect[4] =
	{
		VertexUV(0.f, 1.f, 0.f, 0.f, 0.f),
		VertexUV(1.f, 1.f, 0.f, 1.f, 0.f),
		VertexUV(0.f, 0.f, 0.f, 0.f, 1.f),
		VertexUV(1.f, 0.f, 0.f, 1.f, 1.f)
	};

	if (!CreateMesh(Mesh_Type::Sprite, "TextureRect", TextureRect, 4, sizeof(VertexUV), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IdxRect, 6, sizeof(int), D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT))
		return false;

	Vector3	Box2DPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	if (!CreateMesh(Mesh_Type::Sprite, "ColliderBox2D", Box2DPos, 5,
		sizeof(Vector3), D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP))
		return false;

	// 충돌체 2D 원
	Vector3	CirclePos[31];

	CirclePos[0] = Vector3(1.f, 0.f, 0.f);

	for (int i = 1; i < 31; ++i)
	{
		CirclePos[i].x = cosf(DegreeToRadian(12.f * i));
		CirclePos[i].y = sinf(DegreeToRadian(12.f * i));
	}

	if (!CreateMesh(Mesh_Type::Sprite, "ColliderCircle", CirclePos, 31,
		sizeof(Vector3), D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP))
		return false;

	Vector3	Point;

	if (!CreateMesh(Mesh_Type::Sprite, "ParticlePoint", &Point, 1,
		sizeof(Vector3), D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST))
		return false;


	if (!CreateMaterial("DefaultMaterial"))
		return false;

	SetMaterialBaseColor("DefaultMaterial", 1.f, 1.f, 1.f, 1.f);
	SetMaterialTransparency("DefaultMaterial", true);

	CMesh* pMesh = FindMesh("ColorRect");

	pMesh->AddMaterialSlot("DefaultMaterial");

	pMesh = FindMesh("TextureRect");

	pMesh->AddMaterialSlot("DefaultMaterial");

	CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT);
	CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	CreateSampler("Anisotropic", D3D11_FILTER_ANISOTROPIC, 2);

	ID3D11SamplerState* pSampler = FindSampler("Point");
	CONTEXT->VSSetSamplers(0, 1, &pSampler);
	CONTEXT->PSSetSamplers(0, 1, &pSampler);
	CONTEXT->DSSetSamplers(0, 1, &pSampler);
	CONTEXT->HSSetSamplers(0, 1, &pSampler);
	CONTEXT->GSSetSamplers(0, 1, &pSampler);
	CONTEXT->CSSetSamplers(0, 1, &pSampler);

	pSampler = FindSampler("Linear");
	CONTEXT->VSSetSamplers(1, 1, &pSampler);
	CONTEXT->PSSetSamplers(1, 1, &pSampler);
	CONTEXT->DSSetSamplers(1, 1, &pSampler);
	CONTEXT->HSSetSamplers(1, 1, &pSampler);
	CONTEXT->GSSetSamplers(1, 1, &pSampler);
	CONTEXT->CSSetSamplers(1, 1, &pSampler);

	pSampler = FindSampler("Anisotropic");
	CONTEXT->VSSetSamplers(2, 1, &pSampler);
	CONTEXT->PSSetSamplers(2, 1, &pSampler);
	CONTEXT->DSSetSamplers(2, 1, &pSampler);
	CONTEXT->HSSetSamplers(2, 1, &pSampler);
	CONTEXT->GSSetSamplers(2, 1, &pSampler);
	CONTEXT->CSSetSamplers(2, 1, &pSampler);

	if (!LoadTexture("RandomNoise", TEXT("noise_01.png")))
		return false;

	CTexture* RandomNoiseTex = FindTexture("RandomNoise");

	if (RandomNoiseTex)
		RandomNoiseTex->SetShader(100, TST_ALL);

	Resolution	RS = CDevice::GetInst()->GetResolution();

	CreateTarget("DiffuseTarget", RS.Width, RS.Height, 
		DXGI_FORMAT_R8G8B8A8_UNORM);

	CreateTarget("DistortionTarget", RS.Width, RS.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM);

	if (!LoadTexture("DefaultBurn", TEXT("DefaultBurn.png")))
		return false;

	CTexture* DefaultBurnTex = FindTexture("DefaultBurn");

	if (DefaultBurnTex)
		DefaultBurnTex->SetShader(101, TST_PIXEL);

	if (!LoadTexture("DefaultDistortionNoise", TEXT("noise_01.png")))
		return false;

	CTexture* DefaultDisetortionNoiseTex = FindTexture("DefaultDistortionNoise");

	if (DefaultDisetortionNoiseTex)
		DefaultDisetortionNoiseTex->SetShader(105, TST_PIXEL);

	if (!LoadTexture("DefaultDistortionNormal", TEXT("DefaultDistortionNormal.png")))
		return false;

	CTexture* DefaultDisetortionNormalTex = FindTexture("DefaultDistortionNormal");

	if (DefaultDisetortionNormalTex)
		DefaultDisetortionNormalTex->SetShader(102, TST_PIXEL);

	if (!LoadTexture("DefaultDistortionFilter", TEXT("DefaultDistortionFilter.png")))
		return false;

	CTexture* DefaultDisetortionFilterTex = FindTexture("DefaultDistortionFilter");

	if (DefaultDisetortionFilterTex)
		DefaultDisetortionFilterTex->SetShader(103, TST_PIXEL);


	CreateSoundChannelGroup("BGM");
	CreateSoundChannelGroup("Effect");
	CreateSoundChannelGroup("UI");



	CreateFontFile("Default", TEXT("NotoSansKR-Regular.otf"));

	const TCHAR* FontFaceName = GetFontFaceName("Default");

	CreateTextFormat("Default", FontFaceName, 600, 20.f, TEXT("ko"));


	return true;
}

void CResourceManager::Update()
{
	m_System->update();
}

bool CResourceManager::CreateMesh(Mesh_Type Type, const std::string& Name, void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY Primitive, void* pIndices, int IdxCount, int IdxSize, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	CMesh* pMesh = FindMesh(Name);

	if (pMesh)
		return true;

	switch (Type)
	{
	case Mesh_Type::Sprite:
		pMesh = new CSpriteMesh;
		break;
	case Mesh_Type::StaticMesh:
		pMesh = new CStaticMesh;
		break;
	case Mesh_Type::AnimationMesh:
		pMesh = new CAnimationMesh;
		break;
	}

	if (!pMesh->CreateMesh(pVertices, VtxCount, VtxSize, VtxUsage, Primitive, pIndices, IdxCount, IdxSize, IdxUsage,
		Fmt))
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, pMesh));

	return true;
}

void CResourceManager::ReleaseMesh(const std::string& Name)
{
	auto    iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return;

	if (iter->second->GetRefCount() == 1)
	{
		iter->second->Release();
		m_mapMesh.erase(iter);
	}
}

CMesh* CResourceManager::FindMesh(const std::string& Name)
{
	auto    iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateMaterial(const std::string& Name)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (pMaterial)
		return true;

	pMaterial = new CMaterial;

	if (!pMaterial->Init())
	{
		SAFE_RELEASE(pMaterial);
		return false;
	}

	m_mapMaterial.insert(std::make_pair(Name, pMaterial));

	return true;
}

CMaterial* CResourceManager::CreateMaterial()
{
	CMaterial* Material = new CMaterial;

	Material->Init();

	return Material;
}

bool CResourceManager::SetMaterialTransparency(const std::string& Name, bool bTransparency)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetTransparency(bTransparency);

	return true;
}

bool CResourceManager::SetMaterialOpacity(const std::string& Name,
	float Opacity)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetOpacity(Opacity);

	return true;
}

bool CResourceManager::SetMaterialBaseColor(const std::string& Name, const Vector4& Color)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetBaseColor(Color);

	return true;
}

bool CResourceManager::SetMaterialBaseColor(const std::string& Name, float r, float g, float b, float a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetBaseColor(r, g, b, a);

	return true;
}

bool CResourceManager::SetMaterialBaseColor(const std::string& Name, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetBaseColor(r, g, b, a);

	return true;
}

bool CResourceManager::SetMaterialEmissiveColor(const std::string& Name, const Vector4& Color)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetEmissiveColor(Color);

	return true;
}

bool CResourceManager::SetMaterialEmissiveColor(const std::string& Name, float r, float g, float b, float a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetEmissiveColor(r, g, b, a);

	return true;
}

bool CResourceManager::SetMaterialEmissiveColor(const std::string& Name, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	CMaterial* pMaterial = FindMaterial(Name);

	if (!pMaterial)
		return false;

	pMaterial->SetEmissiveColor(r, g, b, a);

	return true;
}

bool CResourceManager::SetMaterialShader(const std::string& MaterialName, const std::string& ShaderName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetShader(ShaderName);

	return true;
}

bool CResourceManager::AddMaterialTexture(const std::string& MaterialName, 
	const std::string& TextureName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	CTexture* Texture = FindTexture(TextureName);

	if (!Texture)
		return false;

	pMaterial->AddTexture(TextureName, Texture);

	return true;
}

bool CResourceManager::AddMaterialTexture(const std::string& MaterialName,
	const std::string& TextureName, const TCHAR* FileName, 
	const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTexture(TextureName, FileName, PathName);

	return true;
}

bool CResourceManager::AddMaterialTextureFullPath(const std::string& MaterialName,
	const std::string& TextureName, const TCHAR* FullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureFullPath(TextureName, FullPath);

	return true;
}

bool CResourceManager::AddMaterialTexture(const std::string& MaterialName,
	const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTexture(TextureName, vecFileName, PathName);

	return true;
}

bool CResourceManager::AddMaterialTextureFullPath(
	const std::string& MaterialName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureFullPath(TextureName, vecFullPath);

	return true;
}

bool CResourceManager::AddMaterialTextureArray(const std::string& MaterialName, 
	const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureArray(TextureName, vecFileName, PathName);

	return true;
}

bool CResourceManager::AddMaterialTextureArrayFullPath(
	const std::string& MaterialName, const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->AddTextureArrayFullPath(TextureName, vecFullPath);

	return true;
}

bool CResourceManager::SetMaterialTexture(const std::string& MaterialName, 
	const std::string& TextureName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	CTexture* Texture = FindTexture(TextureName);

	if (!Texture)
		return false;

	pMaterial->SetTexture(TextureName, Texture);

	return false;
}

bool CResourceManager::SetMaterialTexture(const std::string& MaterialName,
	const std::string& TextureName, const TCHAR* FileName, 
	const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTexture(TextureName, FileName, PathName);

	return true;
}

bool CResourceManager::SetMaterialTextureFullPath(const std::string& MaterialName,
	const std::string& TextureName, const TCHAR* FullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureFullPath(TextureName, FullPath);

	return true;
}

bool CResourceManager::SetMaterialTexture(const std::string& MaterialName,
	const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTexture(TextureName, vecFileName, PathName);

	return true;
}

bool CResourceManager::SetMaterialTextureFullPath(
	const std::string& MaterialName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureFullPath(TextureName, vecFullPath);

	return true;
}

bool CResourceManager::SetMaterialTextureArray(const std::string& MaterialName,
	const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureArray(TextureName, vecFileName, PathName);

	return true;
}

bool CResourceManager::SetMaterialTextureArrayFullPath(
	const std::string& MaterialName, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureArrayFullPath(TextureName, vecFullPath);

	return true;
}

bool CResourceManager::SetMaterialTextureLink(const std::string& MaterialName, 
	const std::string& TextureName, Texture_Link Link)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureLink(TextureName, Link);

	return true;
}

bool CResourceManager::SetMaterialTextureRegister(const std::string& MaterialName,
	const std::string& TextureName, int Register)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureRegister(TextureName, Register);

	return true;
}

bool CResourceManager::SetMaterialTextureShaderType(
	const std::string& MaterialName, const std::string& TextureName,
	int ShaderType)
{
	CMaterial* pMaterial = FindMaterial(MaterialName);

	if (!pMaterial)
		return false;

	pMaterial->SetTextureShaderType(TextureName, ShaderType);

	return true;
}

void CResourceManager::ReleaseMaterial(const std::string& Name)
{
	auto    iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
		return;

	if (iter->second->GetRefCount() == 1)
	{
		iter->second->Release();
		m_mapMaterial.erase(iter);
	}
}

CMaterial* CResourceManager::FindMaterial(const std::string& Name)
{
	auto    iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CResourceManager::LoadTexture(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFullPath(Name, vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}
	
	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CResourceManager::LoadTextureArray(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CTexture;

	if (!pTexture->LoadTextureArray(Name, vecFileName, PathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CResourceManager::LoadTextureArrayFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CTexture;

	if (!pTexture->LoadTextureArrayFullPath(Name, vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

bool CResourceManager::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat)
{
	CRenderTarget* pTexture = (CRenderTarget*)FindTexture(Name);

	if (pTexture)
		return true;

	pTexture = new CRenderTarget;

	if (!pTexture->CreateTarget(Name, Width, Height, PixelFormat))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, pTexture));

	return true;
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	auto    iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return;

	if (iter->second->GetRefCount() == 1)
	{
		iter->second->Release();
		m_mapTexture.erase(iter);
	}
}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	auto    iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateSampler(const std::string& Name, D3D11_FILTER Filter, 
	UINT MaxAnisotropy, D3D11_TEXTURE_ADDRESS_MODE AddrU, 
	D3D11_TEXTURE_ADDRESS_MODE AddrV, D3D11_TEXTURE_ADDRESS_MODE AddrW)
{
	D3D11_SAMPLER_DESC	Desc = {};

	Desc.Filter = Filter;
	Desc.AddressU = AddrU;
	Desc.AddressV = AddrV;
	Desc.AddressW = AddrW;
	Desc.MaxAnisotropy = MaxAnisotropy;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;
	
	for (int i = 0; i < 4; ++i)
	{
		Desc.BorderColor[i] = 1.f;
	}

	ID3D11SamplerState* pState = nullptr;

	if (FAILED(DEVICE->CreateSamplerState(&Desc, &pState)))
		return false;

	m_mapSampler.insert(std::make_pair(Name, pState));

	return true;
}

ID3D11SamplerState* CResourceManager::FindSampler(const std::string& Name)
{
	auto    iter = m_mapSampler.find(Name);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& SequenceName,
	float PlayTime, float PlayRate)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (pSequence)
		return true;

	pSequence = new CAnimationSequence2D;

	pSequence->SetName(SequenceName);
	pSequence->SetPlayTime(PlayTime);
	pSequence->SetPlayRate(PlayRate);

	m_mapAnimationSequence2D.insert(std::make_pair(SequenceName, pSequence));

	return true;
}

bool CResourceManager::SetAnimationSequence2DTexture(const std::string& SequenceName, 
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

bool CResourceManager::SetAnimationSequence2DTexture(const std::string& SequenceName,
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

bool CResourceManager::SetAnimationSequence2DTextureFullPath(
	const std::string& SequenceName, const std::string& TextureName, 
	const TCHAR* FullPath)
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

bool CResourceManager::SetAnimationSequence2DTexture(const std::string& SequenceName,
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

bool CResourceManager::SetAnimationSequence2DTextureFullPath(
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

bool CResourceManager::SetAnimationSequence2DTextureArray(const std::string& SequenceName,
	const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
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

bool CResourceManager::SetAnimationSequence2DTextureArrayFullPath(
	const std::string& SequenceName, const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFullPath)
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

bool CResourceManager::AddAnimationSequence2DFrame(const std::string& SequenceName, 
	const Vector2& Start, const Vector2& End)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;

	pSequence->AddFrame(Start, End);

	return true;
}

bool CResourceManager::AddAnimationSequence2DFrame(const std::string& SequenceName,
	const Vector2& Start, const Vector2& End, int Count)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;

	pSequence->AddFrame(Start, End, Count);

	return true;
}

bool CResourceManager::AddAnimationSequence2DNotify(const std::string& SequenceName,
	const std::string& NotifyName, int Frame)
{
	CAnimationSequence2D* pSequence = FindAnimationSequence2D(SequenceName);

	if (!pSequence)
		return false;

	pSequence->AddNotify(NotifyName, Frame);

	return true;
}

void CResourceManager::ReleaseAnimationSequence2D(const std::string& Name)
{
	auto    iter = m_mapAnimationSequence2D.find(Name);

	if (iter == m_mapAnimationSequence2D.end())
		return;

	if (iter->second->GetRefCount() == 1)
	{
		iter->second->Release();
		m_mapAnimationSequence2D.erase(iter);
	}
}

CAnimationSequence2D* CResourceManager::FindAnimationSequence2D(const std::string& Name)
{
	auto    iter = m_mapAnimationSequence2D.find(Name);

	if (iter == m_mapAnimationSequence2D.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateParticle(const std::string& Name)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (Particle)
		return true;

	Particle = new CParticleSystem;

	Particle->SetName(Name);

	if (!Particle->Init())
	{
		SAFE_RELEASE(Particle);
		return false;
	}

	m_mapParticleSystem.insert(std::make_pair(Name, Particle));

	return true;
}

void CResourceManager::SetParticleMaterial(const std::string& Name, CMaterial* Material)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMaterial(Material);
}

void CResourceManager::SetParticleMaxParticleCount(const std::string& Name,
	unsigned int Count)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMaxParticleCount(Count);
}

void CResourceManager::SetParticleStartColor(const std::string& Name, 
	const Vector4& Color)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartColor(Color);
}

void CResourceManager::SetParticleStartColor(const std::string& Name,
	float r, float g, float b, float a)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartColor(r, g, b, a);
}

void CResourceManager::SetParticleEndColor(const std::string& Name,
	const Vector4& Color)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndColor(Color);
}

void CResourceManager::SetParticleEndColor(const std::string& Name, float r, float g, float b, float a)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndColor(r, g, b, a);
}

void CResourceManager::SetParticleStartScale(const std::string& Name,
	const Vector3& Scale)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartScale(Scale);
}

void CResourceManager::SetParticleStartScale(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetStartScale(x, y, z);
}

void CResourceManager::SetParticleEndScale(const std::string& Name, const Vector3& Scale)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndScale(Scale);
}

void CResourceManager::SetParticleEndScale(const std::string& Name, float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetEndScale(x, y, z);
}

void CResourceManager::SetParticleLifeTimeMin(const std::string& Name,
	float Min)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetLifeTimeMin(Min);
}

void CResourceManager::SetParticleLifeTimeMax(const std::string& Name, float Max)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetLifeTimeMax(Max);
}

void CResourceManager::SetParticleRange(const std::string& Name, 
	const Vector3& Range)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetRange(Range);
}

void CResourceManager::SetParticleRange(const std::string& Name, 
	float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetRange(x, y, z);
}

void CResourceManager::SetParticleMinSpeed(const std::string& Name, float Min)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMinSpeed(Min);
}

void CResourceManager::SetParticleMaxSpeed(const std::string& Name, float Max)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMaxSpeed(Max);
}

void CResourceManager::SetParticleMoveEnable(const std::string& Name, 
	bool Move)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveEnable(Move);
}

void CResourceManager::SetParticle2D(const std::string& Name, 
	bool b2D)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetParticle2D(b2D);
}

void CResourceManager::SetParticleMoveDir(const std::string& Name,
	const Vector3& Dir)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveDir(Dir);
}

void CResourceManager::SetParticleMoveDir(const std::string& Name,
	float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveDir(x, y, z);
}

void CResourceManager::SetParticleMoveAngle(const std::string& Name, 
	const Vector3& Angle)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveAngle(Angle);
}

void CResourceManager::SetParticleMoveAngle(const std::string& Name, 
	float x, float y, float z)
{
	CParticleSystem* Particle = FindParticle(Name);

	if (!Particle)
		return;

	Particle->SetMoveAngle(x, y, z);
}

void CResourceManager::ReleaseParticle(const std::string& Name)
{
	auto    iter = m_mapParticleSystem.find(Name);

	if (iter == m_mapParticleSystem.end())
		return;

	if (iter->second->GetRefCount() == 1)
	{
		iter->second->Release();
		m_mapParticleSystem.erase(iter);
	}
}

CParticleSystem* CResourceManager::FindParticle(const std::string& Name)
{
	auto    iter = m_mapParticleSystem.find(Name);

	if (iter == m_mapParticleSystem.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::LoadSound(const std::string& GroupName, bool Loop,
	const std::string& Name, const char* FileName, const std::string& PathName)
{

	CSound* Sound = FindSound(Name);

	if (Sound)
		return true;

	Sound = new CSound;

	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	if (!Sound->LoadSound(m_System, Group, Loop, Name, FileName, PathName))
	{
		SAFE_RELEASE(Sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CResourceManager::CreateSoundChannelGroup(const std::string& Name)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(Name);

	if (Group)
		return false;

	if (m_System->createChannelGroup(Name.c_str(), &Group) != FMOD_OK)
		return false;

	// 생성된 그룹은 마스터 그룹의 자식 그룹으로 지정한다.
	m_MasterGroup->addGroup(Group);

	m_mapSoundGroup.insert(std::make_pair(Name, Group));

	return true;
}

bool CResourceManager::SetVolume(int Volume)
{
	m_MasterGroup->setVolume(Volume / 100.f);

	return true;
}

bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	Group->setVolume(Volume / 100.f);

	return true;
}

bool CResourceManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}

bool CResourceManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

bool CResourceManager::SoundPause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Pause();

	return true;
}

bool CResourceManager::SoundResume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Resume();

	return true;
}

void CResourceManager::ReleaseSound(const std::string& Name)
{
	auto    iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return;

	if (iter->second->GetRefCount() == 1)
	{
		iter->second->Release();
		m_mapSound.erase(iter);
	}
}

FMOD::ChannelGroup* CResourceManager::FindSoundChannelGroup(const std::string& Name)
{
	auto    iter = m_mapSoundGroup.find(Name);

	if (iter == m_mapSoundGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CResourceManager::FindSound(const std::string& Name)
{
	auto    iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateFontFile(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	IDWriteFontCollection1* FontCollection = FindFontFile(Name);

	if (FontCollection)
		return true;

	TCHAR	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->pPath);
	lstrcat(FullPath, FileName);

	IDWriteFontFile* FontFile = nullptr;

	if (FAILED(m_WriteFactory->CreateFontFileReference(FullPath, nullptr, 
		&FontFile)))
		return false;

	IDWriteFontSetBuilder1* Builder = nullptr;

	if (FAILED(m_WriteFactory->CreateFontSetBuilder(&Builder)))
		return false;

	Builder->AddFontFile(FontFile);

	IDWriteFontSet* FontSet = nullptr;

	if (FAILED(Builder->CreateFontSet(&FontSet)))
		return false;

	if (FAILED(m_WriteFactory->CreateFontCollectionFromFontSet(FontSet,
		&FontCollection)))
		return false;

	m_mapFontFile.insert(std::make_pair(Name, FontCollection));

	SAFE_RELEASE(FontSet);
	SAFE_RELEASE(Builder);
	SAFE_RELEASE(FontFile);

	return true;
}

bool CResourceManager::CreateTextFormat(const std::string& Name,
	const TCHAR* FontName, int Weight, 
	float Size, const TCHAR* LocalName, int Stretch)
{
	IDWriteTextFormat* Font = FindFont(Name);

	if (Font)
		return true;

	if (FAILED(m_WriteFactory->CreateTextFormat(FontName, nullptr,
		(DWRITE_FONT_WEIGHT)Weight, DWRITE_FONT_STYLE_NORMAL,
		(DWRITE_FONT_STRETCH)Stretch, Size, LocalName, &Font)))
		return false;

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

const TCHAR* CResourceManager::GetFontFaceName(const std::string& Name)
{
	IDWriteFontCollection1* Collection = FindFontFile(Name);

	if (!Collection)
		return nullptr;

	IDWriteFontFamily* Family = nullptr;

	if (FAILED(Collection->GetFontFamily(0, &Family)))
		return nullptr;

	IDWriteLocalizedStrings* LocalizedName = nullptr;

	if (FAILED(Family->GetFamilyNames(&LocalizedName)))
		return nullptr;

	memset(m_FontFamilyName, 0, sizeof(TCHAR) * 256);

	if (FAILED(LocalizedName->GetString(0, m_FontFamilyName, 256)))
		return nullptr;

	return m_FontFamilyName;
}

const char* CResourceManager::GetFontFaceNameMultibyte(const std::string& Name)
{
	if (!GetFontFaceName(Name))
		return nullptr;

	memset(m_FontFamilyNameMultibyte, 0, 256);

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, m_FontFamilyName, 
		-1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, m_FontFamilyName, -1,
		m_FontFamilyNameMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(m_FontFamilyNameMultibyte, m_FontFamilyName);

#endif // UNICODE

	return m_FontFamilyNameMultibyte;
}

bool CResourceManager::CreateFontColor(float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(r, g, b, a);

	if (Brush)
		return true;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a), &Brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), Brush));

	return true;
}

bool CResourceManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(r, g, b, a);

	if (Brush)
		return true;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f), &Brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), Brush));

	return true;
}

bool CResourceManager::CreateFontColor(const Vector4& Color)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(Color);

	if (Brush)
		return true;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
		D2D1::ColorF(Color.x, Color.y, Color.z, Color.w), &Brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(Color), Brush));

	return true;
}

bool CResourceManager::CreateFontColor(unsigned int Color)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(Color);

	if (Brush)
		return true;

	float	r, g, b, a;
	b = (Color & 0x000000ff) / 255.f;
	g = ((Color >> 8) & 0x000000ff) / 255.f;
	r = ((Color >> 16) & 0x000000ff) / 255.f;
	a = ((Color >> 24) & 0x000000ff) / 255.f;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a), &Brush)))
		return false;

	m_mapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), Brush));

	return true;
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(float r, float g, float b, float a)
{
	unsigned int	Key = CreateFontColorKey(r, g, b, a);

	auto	iter = m_mapFontColor.find(Key);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	unsigned int	Key = CreateFontColorKey(r, g, b, a);

	auto	iter = m_mapFontColor.find(Key);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const Vector4& Color)
{
	unsigned int	Key = CreateFontColorKey(Color);

	auto	iter = m_mapFontColor.find(Key);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned int Color)
{
	auto	iter = m_mapFontColor.find(Color);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

unsigned int CResourceManager::CreateFontColorKey(float r, float g, float b,
	float a)
{
	unsigned int	Color = 0;
	unsigned char R, G, B, A;

	R = (unsigned char)(r * 255);
	G = (unsigned char)(g * 255);
	B = (unsigned char)(b * 255);
	A = (unsigned char)(a * 255);

	Color = A;
	Color = (Color << 8) | R;
	Color = (Color << 8) | G;
	Color = (Color << 8) | B;

	return Color;
}

unsigned int CResourceManager::CreateFontColorKey(unsigned char r, 
	unsigned char g, unsigned char b, unsigned char a)
{
	unsigned int	Color = 0;

	Color = a;
	Color = (Color << 8) | r;
	Color = (Color << 8) | g;
	Color = (Color << 8) | b;

	return Color;
}

unsigned int CResourceManager::CreateFontColorKey(const Vector4& Color)
{
	unsigned int	Result = 0;
	unsigned char R, G, B, A;

	R = (unsigned char)(Color.x * 255);
	G = (unsigned char)(Color.y * 255);
	B = (unsigned char)(Color.z * 255);
	A = (unsigned char)(Color.w * 255);

	Result = A;
	Result = (Result << 8) | R;
	Result = (Result << 8) | G;
	Result = (Result << 8) | B;

	return Result;
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* Text, 
	IDWriteTextFormat* Font, float Width, float Height)
{
	IDWriteTextLayout* Layout = nullptr;

	if (FAILED(m_WriteFactory->CreateTextLayout(Text, lstrlen(Text),
		Font, Width, Height, &Layout)))
		return nullptr;

	return Layout;
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* Text,
	const std::string& FontName, float Width, float Height)
{
	IDWriteTextFormat* Font = FindFont(FontName);

	if (!Font)
		return nullptr;

	return CreateTextLayout(Text, Font, Width, Height);
}

IDWriteFontCollection1* CResourceManager::FindFontFile(const std::string& Name)
{
	auto    iter = m_mapFontFile.find(Name);

	if (iter == m_mapFontFile.end())
		return nullptr;

	return iter->second;
}

IDWriteTextFormat* CResourceManager::FindFont(const std::string& Name)
{
	auto    iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}
