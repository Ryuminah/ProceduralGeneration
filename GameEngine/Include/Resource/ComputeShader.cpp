
#include "ComputeShader.h"
#include "../PathManager.h"
#include "../Device.h"

CComputeShader::CComputeShader() :
    m_CS(nullptr),
    m_CSBlob(nullptr)
{
    m_Type = Shader_Type::Compute;
}

CComputeShader::~CComputeShader()
{
    SAFE_RELEASE(m_CS);
    SAFE_RELEASE(m_CSBlob);
}

bool CComputeShader::LoadComputeShader(const std::string& Name, 
    const char* pEntryName, const TCHAR* pFileName,
    const std::string& PathName)
{
    SAFE_RELEASE(m_CS);
    SAFE_RELEASE(m_CSBlob);

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
        "cs_5_0", Flag, 0, &m_CSBlob, &pError)))
    {
        OutputDebugStringA((char*)pError->GetBufferPointer());
        OutputDebugStringA("\n");
        return false;
    }

    // 컴파일된 코드를 이용해서 Shader 객체를 만들어준다.
    if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(),
        nullptr, &m_CS)))
        return false;

    return true;
}

bool CComputeShader::Init()
{
    return true;
}

void CComputeShader::SetShader()
{
    CONTEXT->CSSetShader(m_CS, nullptr, 0);
}

void CComputeShader::Excute(unsigned int x, unsigned int y, unsigned int z)
{
    SetShader();
    CONTEXT->Dispatch(x, y, z);
    CONTEXT->CSSetShader(nullptr, nullptr, 0);
}
