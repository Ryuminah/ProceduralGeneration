#pragma once

#include "../GameEngine.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"
#include "../Resource/AnimationSequence2D.h"
#include "../Resource/AnimationSequence.h"
#include "../Resource/ParticleSystem.h"
#include "../Resource/Sound.h"

class CSceneResource
{
    friend class CScene;

protected:
    CSceneResource();
    ~CSceneResource();

private:
    class CScene* m_pScene;

public:
    bool Init();

private:
    std::unordered_map<std::string, CMesh*>   m_mapMesh;
    std::unordered_map<std::string, CMaterial*>   m_mapMaterial;
    std::unordered_map<std::string, CTexture*>   m_mapTexture;
    std::unordered_map<std::string, CAnimationSequence2D*>   m_mapAnimationSequence2D;
    std::unordered_map<std::string, CAnimationSequence*>   m_mapAnimationSequence;
    std::unordered_map<std::string, CParticleSystem*>   m_mapParticle;
    std::unordered_map<std::string, CSound*>   m_mapSound;

public:
    bool CreateMesh(Mesh_Type Type, const std::string& Name, void* pVertices, int VtxCount, int VtxSize, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
        void* pIndices = nullptr, int IdxCount = 0, int IdxSize = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
        DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
    bool LoadMesh(Mesh_Type Type, const std::string& Name, const TCHAR* Filename, const std::string& PathName = MESH_PATH);
    bool LoadMeshFullPath(Mesh_Type Type, const std::string& Name, const TCHAR* FullPath);
    bool LoadMeshMultibyte(Mesh_Type Type, const std::string& Name, const char* Filename, const std::string& PathName = MESH_PATH);
    bool LoadMeshFullPathMultibyte(Mesh_Type Type, const std::string& Name, const char* FullPath);
    bool SetAnimationMeshSkeleton(const std::string& MeshName, const std::string& Name, const TCHAR* FileName, const std::string& PathName = MESH_PATH);
    bool SetAnimationMeshSkeleton(const std::string& MeshName, class CSkeleton* Skeleton);

    class CMesh* FindMesh(const std::string& Name);

public:
    bool CreateMaterial(const std::string& Name);
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
        const std::string& FindName, const std::string& TextureName, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetMaterialTextureFullPath(const std::string& MaterialName,
        const std::string& FindName, const std::string& TextureName, const TCHAR* FullPath);
    bool SetMaterialTexture(const std::string& MaterialName,
        const std::string& FindName, const std::string& TextureName,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetMaterialTextureFullPath(const std::string& MaterialName,
        const std::string& FindName, const std::string& TextureName,
        const std::vector<const TCHAR*>& vecFullPath);
    bool SetMaterialTextureArray(const std::string& MaterialName,
        const std::string& FindName, const std::string& TextureName,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetMaterialTextureArrayFullPath(const std::string& MaterialName,
        const std::string& FindName, const std::string& TextureName,
        const std::vector<const TCHAR*>& vecFullPath);

    bool SetMaterialTextureLink(const std::string& MaterialName,
        const std::string& TextureName, Texture_Link Link);
    bool SetMaterialTextureRegister(const std::string& MaterialName,
        const std::string& TextureName, int Register);
    bool SetMaterialTextureShaderType(const std::string& MaterialName,
        const std::string& TextureName, int ShaderType);

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

    class CTexture* FindTexture(const std::string& Name);


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

    class CParticleSystem* FindParticle(const std::string& Name);

public:
    bool LoadSound(const std::string& GroupName, bool Loop, const std::string& Name,
        const char* FileName, const std::string& PathName = SOUND_PATH);
    bool SetVolume(int Volume);
    bool SetVolume(const std::string& GroupName, int Volume);
    bool SoundPlay(const std::string& Name);
    bool SoundStop(const std::string& Name);
    bool SoundPause(const std::string& Name);
    bool SoundResume(const std::string& Name);

    FMOD::ChannelGroup* FindSoundChannelGroup(const std::string& Name);
    class CSound* FindSound(const std::string& Name);


public:
    bool LoadAnimationSequence(const std::string& Name, bool Loop,
        struct _tagFbxAnimationClip* Clip);
    bool LoadAnimationSequence(const std::string& Name, bool Loop,
        int StartFrame, int EndFrame, float PlayTime,
        const std::vector<BoneKeyFrame*>& vecFrame);
    bool LoadAnimationSequence(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = ANIMATION_PATH);
    bool LoadAnimationSequenceFullPath(const std::string& Name, const TCHAR* FullPath);
    bool LoadAnimationSequenceMultibyte(const std::string& Name, const char* FileName,
        const std::string& PathName = ANIMATION_PATH);
    bool LoadAnimationSequenceFullPathMultibyte(const std::string& Name, const char* FullPath);
    CAnimationSequence* FindAnimationSequence(const std::string& Name);
};

