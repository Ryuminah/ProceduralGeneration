#pragma once

#include "ConstantBufferData.h"

class CTransformConstantBuffer :
    public CConstantBufferData
{
public:
    CTransformConstantBuffer();
    CTransformConstantBuffer(const CTransformConstantBuffer& buffer);
    virtual ~CTransformConstantBuffer();

protected:
    TransformCBuffer    m_BufferData;

public:
    virtual bool Init();
    virtual void UpdateCBuffer();
    virtual CTransformConstantBuffer* Clone();

public:
    void SetWorldMatrix(const Matrix& matWorld)
    {
        m_BufferData.matWorld = matWorld;
    }

    void SetViewMatrix(const Matrix& matView)
    {
        m_BufferData.matView = matView;
    }

    void SetProjMatrix(const Matrix& matProj)
    {
        m_BufferData.matProj = matProj;
    }

    void SetPivot(const Vector3& Pivot)
    {
        m_BufferData.Pivot = Pivot;
    }

    void SetMeshSize(const Vector3& MeshSize)
    {
        m_BufferData.MeshSize = MeshSize;
    }

    void SetAnimation2DEnable(bool Enable)
    {
        m_BufferData.Animation2DEnable = Enable ? 1 : 0;
    }
};

