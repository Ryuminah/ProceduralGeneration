#pragma once

#include "../GameEngine.h"

class CConstantBuffer
{
    friend class CShaderManager;

private:
    CConstantBuffer();
    ~CConstantBuffer();

private:
    std::string     m_Name;
    ID3D11Buffer* m_pBuffer;
    int             m_Size;
    int             m_Register;
    int             m_ConstantBufferType;

public:
    bool Init(const std::string& Name, int Size, int Register, int ConstantBufferType);
    void UpdateBuffer(void* pData);
};

