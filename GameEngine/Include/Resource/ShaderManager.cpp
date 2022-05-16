
#include "ShaderManager.h"
#include "ComputeShader.h"
#include "Standard2DShader.h"
#include "Standard2DTextureShader.h"
#include "ConstantBuffer.h"
#include "ColliderShader.h"
#include "ParticleUpdateShader.h"
#include "ParticleShader.h"
#include "FullScreenShader.h"
#include "FullScreenDistortionShader.h"
#include "UIImageShader.h"
#include "ProgressBarShader.h"
#include "NumberShader.h"
#include "MouseShader.h"
#include "TileMapShader.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
    {
        auto    iter = m_mapShader.begin();
        auto    iterEnd = m_mapShader.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_RELEASE(iter->second);
        }
    }

    {
        auto    iter = m_mapCBuffer.begin();
        auto    iterEnd = m_mapCBuffer.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_DELETE(iter->second);
        }
    }
}

bool CShaderManager::Init()
{
    if (!CreateShader<CStandard2DShader>("Standard2DShader"))
        return false;

    if (!CreateShader<CStandard2DTextureShader>("Standard2DTextureShader"))
        return false;

    if (!CreateShader<CColliderShader>("ColliderShader"))
        return false;

    if (!CreateShader<CParticleUpdateShader>("ParticleUpdateShader"))
        return false;

    if (!CreateShader<CParticleShader>("ParticleShader"))
        return false;

    if (!CreateShader<CFullScreenShader>("FullScreenShader"))
        return false;

    if (!CreateShader<CFullScreenDistortionShader>("FullScreenDistortionShader"))
        return false;

    if (!CreateShader<CUIImageShader>("UIImageShader"))
        return false;

    if (!CreateShader<CProgressBarShader>("ProgressBarShader"))
        return false;

    if (!CreateShader<CNumberShader>("NumberShader"))
        return false;

    if (!CreateShader<CMouseShader>("MouseShader"))
        return false;

    if (!CreateShader<CTileMapShader>("TileMapShader"))
        return false;

    // ������۸� ������ش�.
    CreateConstantBuffer("TransformCBuffer", sizeof(TransformCBuffer), 0, CBT_ALL);
    CreateConstantBuffer("MaterialCBuffer", sizeof(MaterialCBuffer), 1, CBT_PIXEL);
    CreateConstantBuffer("Animation2DCBuffer", sizeof(Animation2DCBuffer), 6, CBT_ALL);
    CreateConstantBuffer("ColliderCBuffer", sizeof(ColliderCBuffer), 11, CBT_PIXEL);
    CreateConstantBuffer("ParticleCBuffer", sizeof(ParticleCBuffer), 11, CBT_COMPUTE);
    CreateConstantBuffer("GlobalCBuffer", sizeof(GlobalCBuffer), 2, CBT_ALL);
    CreateConstantBuffer("PostProcessCBuffer", sizeof(PostProcessCBuffer), 3, CBT_ALL);
    CreateConstantBuffer("PaperBurnCBuffer", sizeof(PaperBurnCBuffer), 4, CBT_ALL);
    CreateConstantBuffer("Distortion2DCBuffer", sizeof(Distortion2DCBuffer), 5, CBT_ALL);
    CreateConstantBuffer("UITransformCBuffer", sizeof(TransformUICBuffer), 10, CBT_ALL);
    CreateConstantBuffer("UIColorTintCBuffer", sizeof(UIColorTintCBuffer), 11, CBT_PIXEL);
    CreateConstantBuffer("ProgressBarCBuffer", sizeof(ProgressBarCBuffer), 12, CBT_VERTEX);
    CreateConstantBuffer("NumberCBuffer", sizeof(NumberCBuffer), 13, CBT_VERTEX);
    CreateConstantBuffer("TileMapCBuffer", sizeof(TileMapCBuffer), 10, CBT_VERTEX);

    return true;
}

bool CShaderManager::LoadVertexShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    CShader* pShader = FindShader(Name);

    if (!pShader)
        return false;

    else if (pShader->GetShaderType() == Shader_Type::Compute)
        return false;

    return ((CGraphicShader*)pShader)->LoadVertexShader(Name, pEntryName, pFileName, PathName);
}

bool CShaderManager::LoadPixelShader(const std::string& Name, const char* pEntryName, 
    const TCHAR* pFileName, const std::string& PathName)
{
    CShader* pShader = FindShader(Name);

    if (!pShader)
        return false;

    else if (pShader->GetShaderType() == Shader_Type::Compute)
        return false;

    return ((CGraphicShader*)pShader)->LoadPixelShader(Name, pEntryName, pFileName, PathName);
}

bool CShaderManager::LoadDomainShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    CShader* pShader = FindShader(Name);

    if (!pShader)
        return false;

    else if (pShader->GetShaderType() == Shader_Type::Compute)
        return false;

    return ((CGraphicShader*)pShader)->LoadDomainShader(Name, pEntryName, pFileName, PathName);
}

bool CShaderManager::LoadHullShader(const std::string& Name, const char* pEntryName, 
    const TCHAR* pFileName, const std::string& PathName)
{
    CShader* pShader = FindShader(Name);

    if (!pShader)
        return false;

    else if (pShader->GetShaderType() == Shader_Type::Compute)
        return false;

    return ((CGraphicShader*)pShader)->LoadHullShader(Name, pEntryName, pFileName, PathName);
}

bool CShaderManager::LoadGeometryShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    CShader* pShader = FindShader(Name);

    if (!pShader)
        return false;

    else if (pShader->GetShaderType() == Shader_Type::Compute)
        return false;

    return ((CGraphicShader*)pShader)->LoadGeometryShader(Name, pEntryName, pFileName, PathName);
}

CShader* CShaderManager::FindShader(const std::string& Name)
{
    auto    iter = m_mapShader.find(Name);

    if (iter == m_mapShader.end())
        return nullptr;

    return iter->second;
}

bool CShaderManager::AddInputDesc(const std::string& ShaderName, const char* SemanticName,
    unsigned int SemanticIndex, DXGI_FORMAT Fmt, unsigned int InputSlot, unsigned int Size, 
    D3D11_INPUT_CLASSIFICATION InputSlotClass, unsigned int InstanceDataStepRate)
{
    CShader* pShader = FindShader(ShaderName);

    if (!pShader)
        return false;

    pShader->AddInputDesc(SemanticName, SemanticIndex, Fmt, InputSlot, Size,
        InputSlotClass, InstanceDataStepRate);

    return true;
}

bool CShaderManager::CreateInputLayout(const std::string& ShaderName)
{
    CShader* pShader = FindShader(ShaderName);

    if (!pShader)
        return false;

    return pShader->CreateInputLayout();
}

bool CShaderManager::CreateConstantBuffer(const std::string& Name, int Size, int Register, 
    int ConstantBufferType)
{
    CConstantBuffer* pBuffer = FindConstantBuffer(Name);

    if (pBuffer)
        return true;

    pBuffer = new CConstantBuffer;

    if (!pBuffer->Init(Name, Size, Register, ConstantBufferType))
    {
        SAFE_DELETE(pBuffer);
        return false;
    }

    m_mapCBuffer.insert(std::make_pair(Name, pBuffer));

    return true;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& Name)
{
    auto    iter = m_mapCBuffer.find(Name);

    if (iter == m_mapCBuffer.end())
        return nullptr;

    return iter->second;
}
