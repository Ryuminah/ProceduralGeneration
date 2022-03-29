#pragma once

#include "Widget.h"
#include "../Resource/Texture.h"

class CImage :
    public CWidget
{
    friend class CViewport;
    friend class CScene;
    friend class CWidgetWindow;

protected:
    CImage();
    CImage(const CImage& widget);
    virtual ~CImage();

protected:
    CSharedPtr<CTexture>    m_Texture;

public:
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
    virtual CImage* Clone();
};

