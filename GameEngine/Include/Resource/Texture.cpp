
#include "Texture.h"
#include "../PathManager.h"
#include "../Device.h"

CTexture::CTexture()    :
	m_ImageType(Image_Type::Atlas),
	m_ArraySRV(nullptr),
	m_pScene(nullptr),
	m_IsTarget(false)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_ArraySRV);

	size_t  Size = m_vecResourceInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_RELEASE(m_vecResourceInfo[i]->SRV);
		SAFE_DELETE_ARRAY(m_vecResourceInfo[i]->FileName);
		SAFE_DELETE_ARRAY(m_vecResourceInfo[i]->PathName);
		SAFE_DELETE_ARRAY(m_vecResourceInfo[i]->FullPath);
		SAFE_DELETE(m_vecResourceInfo[i]->Image);
		SAFE_DELETE(m_vecResourceInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	TextureResourceInfo* pInfo = new TextureResourceInfo;

	m_vecResourceInfo.push_back(pInfo);

	SetName(Name);

	const PathInfo* PathInfo = CPathManager::GetInst()->FindPath(PathName);

	TCHAR* pFullPath = new TCHAR[MAX_PATH];
	memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

	if (PathInfo)
		lstrcpy(pFullPath, PathInfo->pPath); 

	lstrcat(pFullPath, FileName);

	pInfo->FullPath = pFullPath;

	pInfo->FileName = new TCHAR[MAX_PATH];
	memset(pInfo->FileName, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(pInfo->FileName, FileName);

	pInfo->PathName = new char[MAX_PATH];
	memset(pInfo->PathName, 0, sizeof(char) * MAX_PATH);

	strcpy_s(pInfo->PathName, PathName.length() + 1, PathName.c_str());

	ScratchImage* pImage = new ScratchImage;

	// 경로에서 확장자를 얻어온다.
	char    strExt[_MAX_EXT] = {};

	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, pFullPath);

#endif

	_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
		nullptr, 0, strExt, _MAX_EXT);

	// 확장자를 대문자로 바꿔준다.
	_strupr_s(strExt);

	if (strcmp(strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
			*pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else if (strcmp(strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else
	{
		if(FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	pInfo->Image = pImage;

	return CreateResource(0);
}

bool CTexture::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	TextureResourceInfo* pInfo = new TextureResourceInfo;

	m_vecResourceInfo.push_back(pInfo);

	SetName(Name);

	TCHAR* pFullPath = new TCHAR[MAX_PATH];
	memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(pFullPath, FullPath);

	pInfo->FullPath = pFullPath;

	// 파일 이름과 패스 이름을 얻어온다.
	pInfo->FileName = new TCHAR[MAX_PATH];
	memset(pInfo->FileName, 0, sizeof(TCHAR) * MAX_PATH);

	pInfo->PathName = new char[MAX_PATH];
	memset(pInfo->PathName, 0, sizeof(char) * MAX_PATH);

	strcpy_s(pInfo->PathName, strlen(BASE_PATH) + 1, BASE_PATH);

	int	PathLength = lstrlen(pFullPath);

	for (int i = PathLength - 1; i > 0; --i)
	{
		if ((pFullPath[i] == '\\' || pFullPath[i] == '/') && i >= 4)
		{
			if ((pFullPath[i - 1] == 'n' || pFullPath[i - 1] == 'N') &&
				(pFullPath[i - 2] == 'i' || pFullPath[i - 2] == 'I') &&
				(pFullPath[i - 3] == 'b' || pFullPath[i - 3] == 'B') &&
				(pFullPath[i - 4] == '\\' || pFullPath[i - 4] == '/'))
			{
				lstrcpy(pInfo->FileName, &pFullPath[i + 1]);
				break;
			}
		}
	}

	ScratchImage* pImage = new ScratchImage;

	// 경로에서 확장자를 얻어온다.
	char    strExt[_MAX_EXT] = {};

	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, pFullPath);

#endif

	_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
		nullptr, 0, strExt, _MAX_EXT);

	// 확장자를 대문자로 바꿔준다.
	_strupr_s(strExt);

	if (strcmp(strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
			*pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else if (strcmp(strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	pInfo->Image = pImage;

	return CreateResource(0);
}

bool CTexture::LoadTexture(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	m_ImageType = Image_Type::Frame;

	size_t	Size = vecFileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TextureResourceInfo* pInfo = new TextureResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		SetName(Name);

		const PathInfo* PathInfo = CPathManager::GetInst()->FindPath(PathName);

		TCHAR* pFullPath = new TCHAR[MAX_PATH];
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		if (PathInfo)
			lstrcpy(pFullPath, PathInfo->pPath);

		lstrcat(pFullPath, vecFileName[i]);

		pInfo->FullPath = pFullPath;

		pInfo->FileName = new TCHAR[MAX_PATH];
		memset(pInfo->FileName, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pInfo->FileName, vecFileName[i]);

		pInfo->PathName = new char[MAX_PATH];
		memset(pInfo->PathName, 0, sizeof(char) * MAX_PATH);

		strcpy_s(pInfo->PathName, PathName.length() + 1, PathName.c_str());

		ScratchImage* pImage = new ScratchImage;

		// 경로에서 확장자를 얻어온다.
		char    strExt[_MAX_EXT] = {};

		char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

		strcpy_s(FullPathMultibyte, pFullPath);

#endif

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
			nullptr, 0, strExt, _MAX_EXT);

		// 확장자를 대문자로 바꿔준다.
		_strupr_s(strExt);

		if (strcmp(strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
				*pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else if (strcmp(strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		pInfo->Image = pImage;

		bool Result = CreateResource((int)i);

		if (!Result)
			return false;
	}

	return true;
}

bool CTexture::LoadTextureFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	m_ImageType = Image_Type::Frame;

	size_t	Size = vecFullPath.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TextureResourceInfo* pInfo = new TextureResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		SetName(Name);

		TCHAR* pFullPath = new TCHAR[MAX_PATH];
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pFullPath, vecFullPath[i]);

		pInfo->FullPath = pFullPath;

		ScratchImage* pImage = new ScratchImage;

		// 경로에서 확장자를 얻어온다.
		char    strExt[_MAX_EXT] = {};

		char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

		strcpy_s(FullPathMultibyte, pFullPath);

#endif

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
			nullptr, 0, strExt, _MAX_EXT);

		// 확장자를 대문자로 바꿔준다.
		_strupr_s(strExt);

		if (strcmp(strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
				*pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else if (strcmp(strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		pInfo->Image = pImage;

		bool Result = CreateResource((int)i);

		if (!Result)
			return false;
	}

	return true;
}

bool CTexture::LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	m_ImageType = Image_Type::Array;

	SetName(Name);

	size_t	Size = vecFileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TextureResourceInfo* pInfo = new TextureResourceInfo;

		m_vecResourceInfo.push_back(pInfo);


		const PathInfo* PathInfo = CPathManager::GetInst()->FindPath(PathName);

		TCHAR* pFullPath = new TCHAR[MAX_PATH];
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		if (PathInfo)
			lstrcpy(pFullPath, PathInfo->pPath);

		lstrcat(pFullPath, vecFileName[i]);

		pInfo->FullPath = pFullPath;

		pInfo->FileName = new TCHAR[MAX_PATH];
		memset(pInfo->FileName, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pInfo->FileName, vecFileName[i]);

		pInfo->PathName = new char[MAX_PATH];
		memset(pInfo->PathName, 0, sizeof(char) * MAX_PATH);

		strcpy_s(pInfo->PathName, PathName.length() + 1, PathName.c_str());

		ScratchImage* pImage = new ScratchImage;

		// 경로에서 확장자를 얻어온다.
		char    strExt[_MAX_EXT] = {};

		char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

		strcpy_s(FullPathMultibyte, pFullPath);

#endif

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
			nullptr, 0, strExt, _MAX_EXT);

		// 확장자를 대문자로 바꿔준다.
		_strupr_s(strExt);

		if (strcmp(strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
				*pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else if (strcmp(strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		pInfo->Image = pImage;

		bool Result = CreateResource((int)i);

		if (!Result)
			return false;
	}

	return CreateResourceArray();
}

bool CTexture::LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	m_ImageType = Image_Type::Array;

	SetName(Name);

	size_t	Size = vecFullPath.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TextureResourceInfo* pInfo = new TextureResourceInfo;

		m_vecResourceInfo.push_back(pInfo);

		TCHAR* pFullPath = new TCHAR[MAX_PATH];
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		lstrcpy(pFullPath, vecFullPath[i]);

		pInfo->FullPath = pFullPath;

		ScratchImage* pImage = new ScratchImage;

		// 경로에서 확장자를 얻어온다.
		char    strExt[_MAX_EXT] = {};

		char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

		strcpy_s(FullPathMultibyte, pFullPath);

#endif

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
			nullptr, 0, strExt, _MAX_EXT);

		// 확장자를 대문자로 바꿔준다.
		_strupr_s(strExt);

		if (strcmp(strExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr,
				*pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else if (strcmp(strExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		pInfo->Image = pImage;

		bool Result = CreateResource((int)i);

		if (!Result)
			return false;
	}

	return CreateResourceArray();
}

bool CTexture::CreateResource(int Index)
{
	TextureResourceInfo* pInfo = m_vecResourceInfo[Index];

	if (FAILED(CreateShaderResourceView(DEVICE, pInfo->Image->GetImages(),
		pInfo->Image->GetImageCount(),
		pInfo->Image->GetMetadata(),
		&pInfo->SRV)))
		return false;

	pInfo->Width = (unsigned int)pInfo->Image->GetImages()[0].width;
	pInfo->Height = (unsigned int)pInfo->Image->GetImages()[0].height;

	return true;
}

bool CTexture::CreateResourceArray()
{
	ScratchImage* ArrayImage = new ScratchImage;

	size_t	MipLevel = m_vecResourceInfo[0]->Image->GetMetadata().mipLevels;
	size_t	Count = m_vecResourceInfo.size();

	if (FAILED(ArrayImage->Initialize2D(m_vecResourceInfo[0]->Image->GetMetadata().format,
		m_vecResourceInfo[0]->Image->GetMetadata().width,
		m_vecResourceInfo[0]->Image->GetMetadata().height,
		Count, MipLevel)))
		return false;

	for (size_t i = 0; i < Count; ++i)
	{
		const Image* Images = m_vecResourceInfo[i]->Image->GetImages();

		MipLevel = m_vecResourceInfo[i]->Image->GetMetadata().mipLevels;

		for (size_t j = 0; j < MipLevel; ++j)
		{
			const Image* Src = &ArrayImage->GetImages()[i * MipLevel + j];
			const Image* Dest = &Images[j];

			memcpy(Src->pixels, Dest->pixels, Src->slicePitch);
		}
	}

	ID3D11Texture2D* Texture = nullptr;

	if (FAILED(DirectX::CreateTextureEx(DEVICE, ArrayImage->GetImages(),
		ArrayImage->GetImageCount(),
		ArrayImage->GetMetadata(),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, false, (ID3D11Resource**)&Texture)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC	Desc = {};
	Desc.Format = m_vecResourceInfo[0]->Image->GetMetadata().format;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	Desc.Texture2DArray.MostDetailedMip = 0;
	Desc.Texture2DArray.MipLevels = m_vecResourceInfo[0]->Image->GetMetadata().mipLevels;
	Desc.Texture2DArray.FirstArraySlice = 0;
	Desc.Texture2DArray.ArraySize = Count;

	if (FAILED(DEVICE->CreateShaderResourceView(Texture, &Desc, &m_ArraySRV)))
		return false;

	SAFE_DELETE(ArrayImage);

	SAFE_RELEASE(Texture);

	return true;
}

void CTexture::SetShader(int Register, int ShaderType, int Index)
{
	if (m_ImageType != Image_Type::Array)
	{
		if (ShaderType & TST_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &m_vecResourceInfo[Index]->SRV);

		if (ShaderType & TST_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &m_vecResourceInfo[Index]->SRV);

		if (ShaderType & TST_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &m_vecResourceInfo[Index]->SRV);

		if (ShaderType & TST_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &m_vecResourceInfo[Index]->SRV);

		if (ShaderType & TST_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &m_vecResourceInfo[Index]->SRV);

		if (ShaderType & TST_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &m_vecResourceInfo[Index]->SRV);
	}

	else
	{
		if (ShaderType & TST_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &m_ArraySRV);

		if (ShaderType & TST_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &m_ArraySRV);

		if (ShaderType & TST_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &m_ArraySRV);

		if (ShaderType & TST_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &m_ArraySRV);

		if (ShaderType & TST_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &m_ArraySRV);

		if (ShaderType & TST_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &m_ArraySRV);
	}
}

void CTexture::ResetShader(int Register, int ShaderType, int Index)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if (m_ImageType != Image_Type::Array)
	{
		if (ShaderType & TST_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &SRV);
	}

	else
	{
		if (ShaderType & TST_VERTEX)
			CONTEXT->VSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_PIXEL)
			CONTEXT->PSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_DOMAIN)
			CONTEXT->DSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_HULL)
			CONTEXT->HSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_GEOMETRY)
			CONTEXT->GSSetShaderResources(Register, 1, &SRV);

		if (ShaderType & TST_COMPUTE)
			CONTEXT->CSSetShaderResources(Register, 1, &SRV);
	}
}

void CTexture::Save(FILE* pFile)
{
	int	Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), 1, Length, pFile);

	fwrite(&m_ImageType, sizeof(Image_Type), 1, pFile);

	int	InfoCount = (int)m_vecResourceInfo.size();

	fwrite(&InfoCount, sizeof(int), 1, pFile);

	for (int i = 0; i < InfoCount; ++i)
	{
		bool	Enable = false;

		if (m_vecResourceInfo[i]->FileName)
		{
			Enable = true;
			fwrite(&Enable, sizeof(bool), 1, pFile);
			Length = lstrlen(m_vecResourceInfo[i]->FileName);

			fwrite(&Length, sizeof(int), 1, pFile);
			fwrite(m_vecResourceInfo[i]->FileName, sizeof(TCHAR), Length, pFile);
		}

		else
		{
			fwrite(&Enable, sizeof(bool), 1, pFile);
		}

		if (m_vecResourceInfo[i]->PathName)
		{
			Enable = true;
			fwrite(&Enable, sizeof(bool), 1, pFile);
			Length = (int)strlen(m_vecResourceInfo[i]->PathName);

			fwrite(&Length, sizeof(int), 1, pFile);
			fwrite(m_vecResourceInfo[i]->PathName, sizeof(char), Length, pFile);
		}

		else
		{
			fwrite(&Enable, sizeof(bool), 1, pFile);
		}

		if (m_vecResourceInfo[i]->FullPath)
		{
			Enable = true;
			fwrite(&Enable, sizeof(bool), 1, pFile);
			Length = lstrlen(m_vecResourceInfo[i]->FullPath);

			fwrite(&Length, sizeof(int), 1, pFile);
			fwrite(m_vecResourceInfo[i]->FullPath, sizeof(TCHAR), Length, pFile);
		}

		else
		{
			fwrite(&Enable, sizeof(bool), 1, pFile);
		}
	}
}
