
#include "GraphicShader.h"
#include "../PathManager.h"
#include "../Device.h"

CGraphicShader::CGraphicShader()    :
    m_pVS(nullptr),
    m_pPS(nullptr),
    m_pHS(nullptr),
    m_pDS(nullptr),
    m_pGS(nullptr),
    m_pVSBlob(nullptr),
    m_pPSBlob(nullptr),
    m_pHSBlob(nullptr),
    m_pDSBlob(nullptr),
    m_pGSBlob(nullptr)
{
    m_Type = Shader_Type::Graphic;
}

CGraphicShader::~CGraphicShader()
{
    SAFE_RELEASE(m_pVS);
    SAFE_RELEASE(m_pPS);
    SAFE_RELEASE(m_pHS);
    SAFE_RELEASE(m_pDS);
    SAFE_RELEASE(m_pGS);
    SAFE_RELEASE(m_pVSBlob);
    SAFE_RELEASE(m_pPSBlob);
    SAFE_RELEASE(m_pHSBlob);
    SAFE_RELEASE(m_pDSBlob);
    SAFE_RELEASE(m_pGSBlob);
}

bool CGraphicShader::LoadVertexShader(const std::string& Name, const char* pEntryName, 
    const TCHAR* pFileName, const std::string& PathName)
{
    SAFE_RELEASE(m_pVS);
    SAFE_RELEASE(m_pVSBlob);

    unsigned int    Flag = 0;

#ifdef _DEBUG
    Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

    if (pPath)
        lstrcpy(FullPath, pPath->pPath);

    lstrcat(FullPath, pFileName);

    ID3DBlob* pError = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName,
        "vs_5_0", Flag, 0, &m_pVSBlob, &pError)))
    {
        OutputDebugStringA((char*)pError->GetBufferPointer());
        OutputDebugStringA("\n");
        return false;
    }

    // 컴파일된 코드를 이용해서 Shader 객체를 만들어준다.
    if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(),
        nullptr, &m_pVS)))
        return false;

    return true;
}

bool CGraphicShader::LoadPixelShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    SAFE_RELEASE(m_pPS);
    SAFE_RELEASE(m_pPSBlob);

    unsigned int    Flag = 0;

#ifdef _DEBUG
    Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

    if (pPath)
        lstrcpy(FullPath, pPath->pPath);

    lstrcat(FullPath, pFileName);

    ID3DBlob* pError = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName,
        "ps_5_0", Flag, 0, &m_pPSBlob, &pError)))
    {
        OutputDebugStringA((char*)pError->GetBufferPointer());
        OutputDebugStringA("\n");
        return false;
    }

    // 컴파일된 코드를 이용해서 Shader 객체를 만들어준다.
    if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(),
        nullptr, &m_pPS)))
        return false;

    return true;
}

bool CGraphicShader::LoadHullShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    SAFE_RELEASE(m_pHS);
    SAFE_RELEASE(m_pHSBlob);

    unsigned int    Flag = 0;

#ifdef _DEBUG
    Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

    if (pPath)
        lstrcpy(FullPath, pPath->pPath);

    lstrcat(FullPath, pFileName);

    ID3DBlob* pError = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName,
        "hs_5_0", Flag, 0, &m_pHSBlob, &pError)))
    {
        OutputDebugStringA((char*)pError->GetBufferPointer());
        OutputDebugStringA("\n");
        return false;
    }

    // 컴파일된 코드를 이용해서 Shader 객체를 만들어준다.
    if (FAILED(DEVICE->CreateHullShader(m_pHSBlob->GetBufferPointer(), m_pHSBlob->GetBufferSize(),
        nullptr, &m_pHS)))
        return false;

    return true;
}

bool CGraphicShader::LoadDomainShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    SAFE_RELEASE(m_pDS);
    SAFE_RELEASE(m_pDSBlob);

    unsigned int    Flag = 0;

#ifdef _DEBUG
    Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

    if (pPath)
        lstrcpy(FullPath, pPath->pPath);

    lstrcat(FullPath, pFileName);

    ID3DBlob* pError = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName,
        "ds_5_0", Flag, 0, &m_pDSBlob, &pError)))
    {
        OutputDebugStringA((char*)pError->GetBufferPointer());
        OutputDebugStringA("\n");
        return false;
    }

    // 컴파일된 코드를 이용해서 Shader 객체를 만들어준다.
    if (FAILED(DEVICE->CreateDomainShader(m_pDSBlob->GetBufferPointer(), m_pDSBlob->GetBufferSize(),
        nullptr, &m_pDS)))
        return false;

    return true;
}

bool CGraphicShader::LoadGeometryShader(const std::string& Name, const char* pEntryName,
    const TCHAR* pFileName, const std::string& PathName)
{
    SAFE_RELEASE(m_pGS);
    SAFE_RELEASE(m_pGSBlob);

    unsigned int    Flag = 0;

#ifdef _DEBUG
    Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

    if (pPath)
        lstrcpy(FullPath, pPath->pPath);

    lstrcat(FullPath, pFileName);

    ID3DBlob* pError = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName,
        "gs_5_0", Flag, 0, &m_pGSBlob, &pError)))
    {
        OutputDebugStringA((char*)pError->GetBufferPointer());
        OutputDebugStringA("\n");
        return false;
    }

    // 컴파일된 코드를 이용해서 Shader 객체를 만들어준다.
    if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(),
        nullptr, &m_pGS)))
        return false;

    return true;
}

bool CGraphicShader::Init()
{
    return true;
}

bool CGraphicShader::CreateInputLayout()
{
    if (FAILED(DEVICE->CreateInputLayout(&m_vecInputDesc[0], (unsigned int)m_vecInputDesc.size(),
        m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_InputLayout)))
        return false;

    return true;
}

void CGraphicShader::SetShader()
{
    CONTEXT->VSSetShader(m_pVS, nullptr, 0);
    CONTEXT->PSSetShader(m_pPS, nullptr, 0);
    CONTEXT->HSSetShader(m_pHS, nullptr, 0);
    CONTEXT->DSSetShader(m_pDS, nullptr, 0);
    CONTEXT->GSSetShader(m_pGS, nullptr, 0);

    // 입력레이아웃을 설정한다.
    CONTEXT->IASetInputLayout(m_InputLayout);
}
