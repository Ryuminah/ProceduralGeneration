#pragma once

#include "../GameEngine.h"

class CShaderManager
{
private:
    std::unordered_map<std::string, class CShader*> m_mapShader;
    std::unordered_map<std::string, class CConstantBuffer*> m_mapCBuffer;

public:
    bool Init();

public:
    bool LoadVertexShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
        const std::string& PathName);
    bool LoadPixelShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
        const std::string& PathName);
    bool LoadDomainShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
        const std::string& PathName);
    bool LoadHullShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
        const std::string& PathName);
    bool LoadGeometryShader(const std::string& Name, const char* pEntryName, const TCHAR* pFileName,
        const std::string& PathName);

    class CShader* FindShader(const std::string& Name);

public:
    bool AddInputDesc(const std::string& ShaderName,
        const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT Fmt,
        unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InputSlotClass,
        unsigned int InstanceDataStepRate);
    bool CreateInputLayout(const std::string& ShaderName);

public:
    bool CreateConstantBuffer(const std::string& Name, int Size, int Register, int ConstantBufferType);

    class CConstantBuffer* FindConstantBuffer(const std::string& Name);

public:
    template <typename T>
    bool CreateShader(const std::string& Name)
    {
        T* pShader = (T*)FindShader(Name);

        if (pShader)
            return true;

        pShader = new T;

        pShader->SetName(Name);

        if (!pShader->Init())
        {
            SAFE_RELEASE(pShader);
            return false;
        }

        m_mapShader.insert(std::make_pair(Name, pShader));

        return true;
    }

    DECLARE_SINGLE(CShaderManager)
};

