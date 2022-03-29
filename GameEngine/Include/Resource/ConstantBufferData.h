#pragma once

#include "../GameEngine.h"

class CConstantBufferData
{
public:
    CConstantBufferData();
    CConstantBufferData(const CConstantBufferData& buffer);
    virtual ~CConstantBufferData();

protected:
    class CConstantBuffer* m_pBuffer;

protected:
    void SetConstantBuffer(const std::string& Name);

public:
    virtual bool Init() = 0;
    virtual void UpdateCBuffer() = 0;
    virtual CConstantBufferData* Clone() = 0;
};

