
#include "Material.h"
#include "ShaderManager.h"
#include "MaterialConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "ResourceManager.h"
#include "../Render/RenderState.h"
#include "../Render/RenderStateManager.h"
#include "../PathManager.h"

CMaterial::CMaterial()  :
	m_bTransparency(false),
	m_PaperBurnEnable(false),
	m_DistortionEnable(false),
	m_pCBuffer(nullptr),
	m_pScene(nullptr),
	m_BaseColor(1.f, 1.f, 1.f, 1.f),
	m_EmissiveColor(0.f, 0.f, 0.f, 0.f),
	m_RenderState{},
	m_Opacity(1.f)
{
}

CMaterial::CMaterial(const CMaterial& material)
{
	*this = material;

	m_RefCount = 1;

	m_pCBuffer = material.m_pCBuffer->Clone();

	m_vecTexture.clear();

	size_t  Size = material.m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		MaterialTextureInfo* pInfo = new MaterialTextureInfo;

		m_vecTexture.push_back(pInfo);

		pInfo->Name = material.m_vecTexture[i]->Name;
		pInfo->Link = material.m_vecTexture[i]->Link;
		pInfo->pTexture = material.m_vecTexture[i]->pTexture;
		pInfo->Register = material.m_vecTexture[i]->Register;
		pInfo->ShaderType = material.m_vecTexture[i]->ShaderType;
	}

	m_SetMaterialList.clear();
	m_ResetMaterialList.clear();
}

CMaterial::~CMaterial()
{
	{
		auto	iter = m_SetMaterialList.begin();
		auto	iterEnd = m_SetMaterialList.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
	}

	{
		auto	iter = m_ResetMaterialList.begin();
		auto	iterEnd = m_ResetMaterialList.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
	}

	size_t  Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTexture[i]);
	}

	SAFE_DELETE(m_pCBuffer);
}

void CMaterial::SetTransparency(bool bTransparency)
{
	m_bTransparency = bTransparency;

	if (!m_bTransparency)
		m_RenderState[RST_Blend] = nullptr;

	else
		m_RenderState[RST_Blend] = CRenderStateManager::GetInst()->FindRenderState("AlphaBlend");
}

void CMaterial::SetBaseColor(const Vector4& Color)
{
	m_BaseColor = Color;

	m_pCBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);

	m_pCBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColor(unsigned char r, unsigned char g, 
	unsigned char b, unsigned char a)
{
	m_BaseColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_pCBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetEmissiveColor(const Vector4& Color)
{
	m_EmissiveColor = Color;

	m_pCBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetEmissiveColor(float r, float g, float b, float a)
{
	m_EmissiveColor = Vector4(r, g, b, a);

	m_pCBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetEmissiveColor(unsigned char r, unsigned char g, 
	unsigned char b, unsigned char a)
{
	m_EmissiveColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_pCBuffer->SetEmissiveColor(m_EmissiveColor);
}

bool CMaterial::AddTexture(const std::string& Name, CTexture* Texture)
{
	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = Texture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::AddTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTexture(Name, FileName, PathName);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = pTexture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::AddTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureFullPath(Name, FullPath);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = pTexture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::AddTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTexture(Name, vecFileName, PathName);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = pTexture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::AddTextureFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureFullPath(Name, vecFullPath);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = pTexture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::AddTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureArray(Name, vecFileName, PathName);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = pTexture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::AddTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	MaterialTextureInfo* pInfo = new MaterialTextureInfo;

	pInfo->Name = Name;
	pInfo->pTexture = pTexture;

	m_vecTexture.push_back(pInfo);

	return true;
}

bool CMaterial::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTexture(Name, FileName, PathName);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	pInfo->pTexture = pTexture;

	return true;
}

bool CMaterial::SetTexture(const std::string& Name, CTexture* Texture)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	pInfo->pTexture = Texture;

	return true;
}

bool CMaterial::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureFullPath(Name, FullPath);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	pInfo->pTexture = pTexture;

	return true;
}

bool CMaterial::SetTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTexture(Name, vecFileName, PathName);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	pInfo->pTexture = pTexture;

	return true;
}

bool CMaterial::SetTextureFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureFullPath(Name, vecFullPath);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	pInfo->pTexture = pTexture;

	return true;
}

bool CMaterial::SetTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureArray(Name, vecFileName, PathName);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	pInfo->pTexture = pTexture;

	return true;
}

bool CMaterial::SetTextureArrayFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	CTexture* pTexture = nullptr;

	if (m_pScene)
	{
		m_pScene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath);

		pTexture = m_pScene->GetResource()->FindTexture(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath);

		pTexture = CResourceManager::GetInst()->FindTexture(Name);
	}

	pInfo->pTexture = pTexture;

	return true;
}

bool CMaterial::SetTextureLink(const std::string& Name, Texture_Link Link)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	pInfo->Link = Link;

	if (Link == Texture_Link::BaseTexture)
		pInfo->Register = 0;

	else if (Link == Texture_Link::EmissiveTexture)
		pInfo->Register = 1;

	return true;
}

bool CMaterial::SetTextureRegister(const std::string& Name, int Register)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	pInfo->Register = Register;

	if (Register == 0)
		pInfo->Link = Texture_Link::BaseTexture;

	else if (Register == 1)
		pInfo->Link = Texture_Link::EmissiveTexture;

	else
		pInfo->Link = Texture_Link::CustomTexture;

	return true;
}

bool CMaterial::SetTextureShaderType(const std::string& Name, int ShaderType)
{
	MaterialTextureInfo* pInfo = nullptr;

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecTexture[i]->Name == Name)
		{
			pInfo = m_vecTexture[i];
			break;
		}
	}

	if (!pInfo)
		return false;

	pInfo->ShaderType = ShaderType;

	return true;
}

void CMaterial::PaperBurnEnable(bool Enable)
{
	m_PaperBurnEnable = Enable;
	m_pCBuffer->PaperBurnEnable(Enable);
}

void CMaterial::DistortionEnable(bool Enable)
{
	m_DistortionEnable = Enable;
	m_pCBuffer->DistortionEnable(Enable);
}

bool CMaterial::Init()
{
	m_pCBuffer = new CMaterialConstantBuffer;

	m_pCBuffer->Init();

	SetShader("Standard2DTextureShader");

	return true;
}

void CMaterial::SetShader(const std::string& Name)
{
	m_pShader = CShaderManager::GetInst()->FindShader(Name);
}

void CMaterial::SetMaterial()
{
	m_pCBuffer->SetBaseColor(m_BaseColor);
	m_pCBuffer->SetEmissiveColor(m_EmissiveColor);
	m_pCBuffer->SetOpacity(m_Opacity);
	m_pCBuffer->UpdateCBuffer();

	for (int i = 0; i < RST_End; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->SetState();
	}

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTexture[i]->pTexture->SetShader(m_vecTexture[i]->Register,
			m_vecTexture[i]->ShaderType);
	}

	auto	iter = m_SetMaterialList.begin();
	auto	iterEnd = m_SetMaterialList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Func();
	}

	m_pShader->SetShader();
}

void CMaterial::ResetMaterial()
{
	for (int i = 0; i < RST_End; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->ResetState();
	}

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTexture[i]->pTexture->ResetShader(m_vecTexture[i]->Register,
			m_vecTexture[i]->ShaderType);
	}

	auto	iter = m_ResetMaterialList.begin();
	auto	iterEnd = m_ResetMaterialList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Func();
	}
}

void CMaterial::SetMaterialEmptyShader()
{
	m_pCBuffer->SetBaseColor(m_BaseColor);
	m_pCBuffer->SetEmissiveColor(m_EmissiveColor);
	m_pCBuffer->SetOpacity(m_Opacity);
	m_pCBuffer->UpdateCBuffer();

	for (int i = 0; i < RST_End; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->SetState();
	}

	size_t	Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTexture[i]->pTexture->SetShader(m_vecTexture[i]->Register,
			m_vecTexture[i]->ShaderType);
	}

	auto	iter = m_SetMaterialList.begin();
	auto	iterEnd = m_SetMaterialList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Func();
	}

	m_pShader->SetShader();
}

void CMaterial::ResetMaterialEmptyShader()
{
	for (int i = 0; i < RST_End; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->ResetState();
	}

	auto	iter = m_ResetMaterialList.begin();
	auto	iterEnd = m_ResetMaterialList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Func();
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::Save(const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->pPath);

	lstrcat(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CMaterial::Save(FILE* pFile)
{
	int	Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), 1, Length, pFile);

	const std::string& ShaderName = m_pShader->GetName();
	Length = (int)ShaderName.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(ShaderName.c_str(), 1, Length, pFile);

	fwrite(&m_BaseColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_EmissiveColor, sizeof(Vector4), 1, pFile);

	fwrite(&m_bTransparency, sizeof(bool), 1, pFile);
	fwrite(&m_PaperBurnEnable, sizeof(bool), 1, pFile);
	fwrite(&m_DistortionEnable, sizeof(bool), 1, pFile);
	fwrite(&m_Opacity, sizeof(float), 1, pFile);

	for (int i = 0; i < RST_End; ++i)
	{
		bool	Enable = false;

		if (m_RenderState[i])
		{
			Enable = true;
			fwrite(&Enable, sizeof(bool), 1, pFile);
			const std::string& StateName = m_RenderState[i]->GetName();
			Length = (int)StateName.length();

			fwrite(&Length, sizeof(int), 1, pFile);
			fwrite(StateName.c_str(), 1, Length, pFile);
		}

		else
		{
			fwrite(&Enable, sizeof(bool), 1, pFile);
		}
	}

	int	TextureCount = (int)m_vecTexture.size();
	fwrite(&TextureCount, sizeof(int), 1, pFile);

	for (int i = 0; i < TextureCount; ++i)
	{
		Length = (int)m_vecTexture[i]->Name.length();

		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_vecTexture[i]->Name.c_str(), 1, Length, pFile);
		fwrite(&m_vecTexture[i]->Link, sizeof(Texture_Link), 1, pFile);
		fwrite(&m_vecTexture[i]->Register, sizeof(int), 1, pFile);
		fwrite(&m_vecTexture[i]->ShaderType, sizeof(int), 1, pFile);
		m_vecTexture[i]->pTexture->Save(pFile);
	}
}

void CMaterial::SaveFullPath(const TCHAR* FullPath)
{
	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, FullPath);

#endif // UNICODE

	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPathMultibyte, "wb");

	if (!pFile)
		return;

	Save(pFile);

	fclose(pFile);
}

void CMaterial::Load(const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->pPath);

	lstrcat(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CMaterial::Load(FILE* pFile)
{
	int	Length = 0;

	char	Name[256] = {};
	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, 1, Length, pFile);

	m_Name = Name;

	char	ShaderName[256] = {};
	fread(&Length, sizeof(int), 1, pFile);
	fread(ShaderName, 1, Length, pFile);

	m_pShader = CShaderManager::GetInst()->FindShader(ShaderName);

	fread(&m_BaseColor, sizeof(Vector4), 1, pFile);
	fread(&m_EmissiveColor, sizeof(Vector4), 1, pFile);

	m_pCBuffer->SetBaseColor(m_BaseColor);
	m_pCBuffer->SetEmissiveColor(m_EmissiveColor);

	fread(&m_bTransparency, sizeof(bool), 1, pFile);
	fread(&m_PaperBurnEnable, sizeof(bool), 1, pFile);
	fread(&m_DistortionEnable, sizeof(bool), 1, pFile);
	fread(&m_Opacity, sizeof(float), 1, pFile);

	m_pCBuffer->PaperBurnEnable(m_PaperBurnEnable);
	m_pCBuffer->DistortionEnable(m_DistortionEnable);
	m_pCBuffer->SetOpacity(m_Opacity);

	for (int i = 0; i < RST_End; ++i)
	{
		bool	Enable = false;
		fread(&Enable, sizeof(bool), 1, pFile);

		if (Enable)
		{
			char	StateName[256] = {};
			fread(&Length, sizeof(int), 1, pFile);
			fread(StateName, 1, Length, pFile);

			m_RenderState[i] = CRenderStateManager::GetInst()->FindRenderState(StateName);
		}
	}

	size_t  Size = m_vecTexture.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTexture[i]);
	}

	m_vecTexture.clear();

	int	TextureCount = 0;
	fread(&TextureCount, sizeof(int), 1, pFile);

	for (int i = 0; i < TextureCount; ++i)
	{
		MaterialTextureInfo* Info = new MaterialTextureInfo;

		m_vecTexture.push_back(Info);

		char	TextureName[256] = {};

		fread(&Length, sizeof(int), 1, pFile);
		fread(TextureName, 1, Length, pFile);

		Info->Name = Name;

		fread(&m_vecTexture[i]->Link, sizeof(Texture_Link), 1, pFile);
		fread(&m_vecTexture[i]->Register, sizeof(int), 1, pFile);
		fread(&m_vecTexture[i]->ShaderType, sizeof(int), 1, pFile);

		memset(TextureName, 0, 256);
		fread(&Length, sizeof(int), 1, pFile);
		fread(TextureName, 1, Length, pFile);

		Image_Type	ImgType;
		fread(&ImgType, sizeof(Image_Type), 1, pFile);

		int	InfoCount = 0;

		fread(&InfoCount, sizeof(int), 1, pFile);

		if (InfoCount == 1)
		{
			TCHAR	TextureFileName[MAX_PATH] = {};
			TCHAR	TextureFullPath[MAX_PATH] = {};
			char	PathName[256] = {};

			bool	Enable = false;
			fread(&Enable, sizeof(bool), 1, pFile);

			bool	FileNameEnable = Enable;

			if (Enable)
			{
				fread(&Length, sizeof(int), 1, pFile);
				fread(TextureFileName, sizeof(TCHAR), Length, pFile);
			}

			fread(&Enable, sizeof(bool), 1, pFile);

			if (Enable)
			{
				fread(&Length, sizeof(int), 1, pFile);
				fread(PathName, sizeof(char), Length, pFile);
			}

			fread(&Enable, sizeof(bool), 1, pFile);

			if (Enable)
			{
				fread(&Length, sizeof(int), 1, pFile);
				fread(TextureFullPath, sizeof(TCHAR), Length, pFile);
			}

			if (FileNameEnable)
			{
				CResourceManager::GetInst()->LoadTexture(TextureName,
					TextureFileName, PathName);
			}

			else
			{
				CResourceManager::GetInst()->LoadTextureFullPath(TextureName,
					TextureFullPath);
			}

			Info->pTexture = CResourceManager::GetInst()->FindTexture(TextureName);
		}

		else
		{
			std::vector<const TCHAR*>	vecFileName;
			std::vector<const TCHAR*>	vecFullPath;
			char	PathName[256] = {};

			for (int i = 0; i < InfoCount; ++i)
			{
				bool	Enable = false;
				fread(&Enable, sizeof(bool), 1, pFile);

				if (Enable)
				{
					TCHAR* FileName = new TCHAR[MAX_PATH];
					memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

					fread(&Length, sizeof(int), 1, pFile);
					fread(FileName, sizeof(TCHAR), Length, pFile);

					vecFileName.push_back(FileName);
				}

				fread(&Enable, sizeof(bool), 1, pFile);

				if (Enable)
				{
					fread(&Length, sizeof(int), 1, pFile);
					fread(PathName, sizeof(char), Length, pFile);
				}

				fread(&Enable, sizeof(bool), 1, pFile);

				if (Enable)
				{
					TCHAR* FullPath = new TCHAR[MAX_PATH];
					memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

					fread(&Length, sizeof(int), 1, pFile);
					fread(FullPath, sizeof(TCHAR), Length, pFile);

					vecFullPath.push_back(FullPath);
				}
			}
			


			if (!vecFileName.empty())
			{
				CResourceManager::GetInst()->LoadTexture(TextureName,
					vecFileName, PathName);
				Info->pTexture = CResourceManager::GetInst()->FindTexture(TextureName);
				for (size_t j = 0; j < vecFileName.size(); ++j)
				{
					SAFE_DELETE_ARRAY(vecFileName[j]);
				}

				vecFileName.clear();
			}

			if (!vecFullPath.empty())
			{
				if (!Info->pTexture)
				{
					CResourceManager::GetInst()->LoadTexture(TextureName,
						vecFileName, PathName);
					Info->pTexture = CResourceManager::GetInst()->FindTexture(TextureName);
				}

				for (size_t j = 0; j < vecFullPath.size(); ++j)
				{
					SAFE_DELETE_ARRAY(vecFullPath[j]);
				}

				vecFullPath.clear();
			}
		}
	}
}

void CMaterial::LoadFullPath(const TCHAR* FullPath)
{
	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, FullPath);

#endif // UNICODE

	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPathMultibyte, "rb");

	if (!pFile)
		return;

	Load(pFile);

	fclose(pFile);
}
