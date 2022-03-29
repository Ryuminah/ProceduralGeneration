#pragma once

#include "Widget.h"
#include "../Resource/Animation2D.h"
#include "../Resource/UITransformConstantBuffer.h"

class CMouseWidget :
    public CWidget
{
    friend class CEngine;

protected:
    CMouseWidget();
    CMouseWidget(const CMouseWidget& widget);
    virtual ~CMouseWidget();

protected:
    CSharedPtr<CTexture>    m_Texture;
    CSharedPtr<CAnimation2D>    m_Animation;


public:
    CAnimation2D* GetAnimation2D()  const
    {
        return m_Animation;
    }

    void SetTexture(CTexture* Texture)
    {
        m_Texture = Texture;
    }

    void SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CMouseWidget* Clone();

public:
    template <typename T>
    void CreateAnimation2D()
    {
        m_Animation = new T;

        m_Animation->SetScene(m_Scene);
        m_Animation->SetOwner(this);

        m_Animation->Init();

        m_Animation->Release();

        m_TransformCBuffer->SetAnimation2DEnable(true);
        //SetAnimation2DEnable(true);
    }
};

