#pragma once

#include "../GameEngine.h"

class CResourceManager
{
private:
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*>   m_mapSoundGroup;
	std::unordered_map<std::string, class CSound*>   m_mapSound;

private:
	//IDWriteFactory* m_WriteFactory;
	IDWriteFactory5* m_WriteFactory;
	std::unordered_map<std::string, IDWriteFontCollection1*>	m_mapFontFile;
	std::unordered_map<std::string, IDWriteTextFormat*>   m_mapFont;
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>   m_mapFontColor;
	TCHAR	m_FontFamilyName[256];
	char	m_FontFamilyNameMultibyte[256];

private:
	std::unordered_map<std::string, class CMesh*>   m_mapMesh;
	std::unordered_map<std::string, class CMaterial*>   m_mapMaterial;
	std::unordered_map<std::string, class CTexture*>   m_mapTexture;
	std::unordered_map<std::string, ID3D11SamplerState*>   m_mapSampler;
	std::unordered_map<std::string, class CAnimationSequence2D*>   m_mapAnimationSequence2D;
	std::unordered_map<std::string, class CParticleSystem*>   m_mapParticleSystem;


public:
	bool Init();
	void Update();

public:
	bool CreateMesh(Mesh_Type Type, const std::string& Name, void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* pIndices = nullptr, int IdxCount = 0, int IdxSize = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);

	void ReleaseMesh(const std::string& Name);

	class CMesh* FindMesh(const std::string& Name);

public:
	bool CreateMaterial(const std::string& Name);
	class CMaterial* CreateMaterial();
	bool SetMaterialTransparency(const std::string& Name,
		bool bTransparency);
	bool SetMaterialOpacity(const std::string& Name,
		float Opacity);
	bool SetMaterialBaseColor(const std::string& Name,
		const Vector4& Color);
	bool SetMaterialBaseColor(const std::string& Name,
		float r, float g, float b, float a);
	bool SetMaterialBaseColor(const std::string& Name,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool SetMaterialEmissiveColor(const std::string& Name,
		const Vector4& Color);
	bool SetMaterialEmissiveColor(const std::string& Name,
		float r, float g, float b, float a);
	bool SetMaterialEmissiveColor(const std::string& Name,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool SetMaterialShader(const std::string& MaterialName,
		const std::string& ShaderName);

	bool AddMaterialTexture(const std::string& MaterialName,
		const std::string& TextureName);
	bool AddMaterialTexture(const std::string& MaterialName, 
		const std::string& TextureName, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool AddMaterialTextureFullPath(const std::string& MaterialName,
		const std::string& TextureName, const TCHAR* FullPath);
	bool AddMaterialTexture(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool AddMaterialTextureFullPath(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddMaterialTextureArray(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool AddMaterialTextureArrayFullPath(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFullPath);

	// 기존 Texture를 교체한다.
	bool SetMaterialTexture(const std::string& MaterialName,
		const std::string& TextureName);
	bool SetMaterialTexture(const std::string& MaterialName,
		const std::string& TextureName, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetMaterialTextureFullPath(const std::string& MaterialName,
		const std::string& TextureName, const TCHAR* FullPath);
	bool SetMaterialTexture(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetMaterialTextureFullPath(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFullPath);
	bool SetMaterialTextureArray(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetMaterialTextureArrayFullPath(const std::string& MaterialName,
		const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFullPath);

	bool SetMaterialTextureLink(const std::string& MaterialName,
		const std::string& TextureName, Texture_Link Link);
	bool SetMaterialTextureRegister(const std::string& MaterialName,
		const std::string& TextureName, int Register);
	bool SetMaterialTextureShaderType(const std::string& MaterialName,
		const std::string& TextureName, int ShaderType);

	void ReleaseMaterial(const std::string& Name);

	class CMaterial* FindMaterial(const std::string& Name);


public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);
	bool CreateTarget(const std::string& Name, unsigned int Width,
		unsigned int Height, DXGI_FORMAT PixelFormat);

	void ReleaseTexture(const std::string& Name);

	class CTexture* FindTexture(const std::string& Name);


public:
	bool CreateSampler(const std::string& Name, D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		UINT MaxAnisotropy = 1,
		D3D11_TEXTURE_ADDRESS_MODE AddrU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE AddrV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE AddrW = D3D11_TEXTURE_ADDRESS_WRAP);

	ID3D11SamplerState* FindSampler(const std::string& Name);


public:
	bool CreateAnimationSequence2D(const std::string& SequenceName, float PlayTime = 1.f,
		float PlayRate = 1.f);
	bool SetAnimationSequence2DTexture(const std::string& SequenceName, const std::string& TextureName);
	bool SetAnimationSequence2DTexture(const std::string& SequenceName, const std::string& TextureName,
		const TCHAR* pFileName, const std::string& PathName = TEXTURE_PATH);
	bool SetAnimationSequence2DTextureFullPath(const std::string& SequenceName, const std::string& TextureName,
		const TCHAR* FullPath);
	bool SetAnimationSequence2DTexture(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	bool SetAnimationSequence2DTextureFullPath(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFullPath);
	bool SetAnimationSequence2DTextureArray(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	bool SetAnimationSequence2DTextureArrayFullPath(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddAnimationSequence2DFrame(const std::string& SequenceName, const Vector2& Start,
		const Vector2& End);
	bool AddAnimationSequence2DFrame(const std::string& SequenceName, const Vector2& Start,
		const Vector2& End, int Count);
	bool AddAnimationSequence2DNotify(const std::string& SequenceName,
		const std::string& NotifyName, int Frame);


	void ReleaseAnimationSequence2D(const std::string& Name);

	class CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);



public:
	bool CreateParticle(const std::string& Name);
	void SetParticleMaterial(const std::string& Name, CMaterial* Material);
	void SetParticleMaxParticleCount(const std::string& Name, unsigned int Count);
	void SetParticleStartColor(const std::string& Name, const Vector4& Color);
	void SetParticleStartColor(const std::string& Name, float r, float g, float b, float a);
	void SetParticleEndColor(const std::string& Name, const Vector4& Color);
	void SetParticleEndColor(const std::string& Name, float r, float g, float b, float a);
	void SetParticleStartScale(const std::string& Name, const Vector3& Scale);
	void SetParticleStartScale(const std::string& Name, float x, float y, float z);
	void SetParticleEndScale(const std::string& Name, const Vector3& Scale);
	void SetParticleEndScale(const std::string& Name, float x, float y, float z);
	void SetParticleLifeTimeMin(const std::string& Name, float Min);
	void SetParticleLifeTimeMax(const std::string& Name, float Max);
	void SetParticleRange(const std::string& Name, const Vector3& Range);
	void SetParticleRange(const std::string& Name, float x, float y, float z);
	void SetParticleMinSpeed(const std::string& Name, float Min);
	void SetParticleMaxSpeed(const std::string& Name, float Max);
	void SetParticleMoveEnable(const std::string& Name, bool Move);
	void SetParticle2D(const std::string& Name, bool b2D);
	void SetParticleMoveDir(const std::string& Name, const Vector3& Dir);
	void SetParticleMoveDir(const std::string& Name, float x, float y, float z);
	void SetParticleMoveAngle(const std::string& Name, const Vector3& Angle);
	void SetParticleMoveAngle(const std::string& Name, float x, float y, float z);

	void ReleaseParticle(const std::string& Name);

	class CParticleSystem* FindParticle(const std::string& Name);

public:
	bool LoadSound(const std::string& GroupName, bool Loop, const std::string& Name,
		const char* FileName, const std::string& PathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& Name);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	void ReleaseSound(const std::string& Name);

	FMOD::ChannelGroup* FindSoundChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);


public:
	bool CreateFontFile(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool CreateTextFormat(const std::string& Name, const TCHAR* FontName,
		int Weight, float Size,
		const TCHAR* LocalName, int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	const TCHAR* GetFontFaceName(const std::string& Name);
	const char* GetFontFaceNameMultibyte(const std::string& Name);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	bool CreateFontColor(const Vector4& Color);
	bool CreateFontColor(unsigned int Color);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& Color);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* Text, IDWriteTextFormat* Font,
		float Width, float Height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* Text, const std::string& FontName,
		float Width, float Height);

	IDWriteFontCollection1* FindFontFile(const std::string& Name);
	IDWriteTextFormat* FindFont(const std::string& Name);

	DECLARE_SINGLE(CResourceManager)
};

