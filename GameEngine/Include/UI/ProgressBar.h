#pragma once

#include "Widget.h"
#include "../Resource/Texture.h"
#include "../Resource/ProgressBarCBuffer.h"

struct ProgressBarStateInfo
{
    CSharedPtr<CTexture>    Texture;
    Vector4         Tint;

    ProgressBarStateInfo()
    {
        Tint = Vector4::White;
    }
};

class CProgressBar :
    public CWidget
{
    friend class CViewport;
    friend class CScene;
    friend class CWidgetWindow;

protected:
    CProgressBar();
    CProgressBar(const CProgressBar& widget);
    virtual ~CProgressBar();

protected:
    ProgressBarStateInfo    m_BarInfo;
    ProgressBarStateInfo    m_BackInfo;
    float                   m_Percent;
    ProgressBar_Dir         m_Dir;
    CProgressBarCBuffer* m_ProgressBarCBuffer;
    CSharedPtr<CShader> m_BackShader;

public:
    void SetPercent(float Percent)
    {
        m_Percent = Percent;

        m_ProgressBarCBuffer->SetPercent(Percent);
    }

    void SetDir(ProgressBar_Dir Dir)
    {
        m_Dir = Dir;

        m_ProgressBarCBuffer->SetDir(Dir);
    }

public:
    void SetTexture(CTexture* Texture);
    bool SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(const std::string& Name,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name,
        const std::vector<const TCHAR*>& vecFullPath);
    bool SetTextureArray(const std::string& Name,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureArrayFullPath(const std::string& Name,
        const std::vector<const TCHAR*>& vecFullPath);

    void SetBackTexture(CTexture* Texture);
    bool SetBackTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetBackTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool SetBackTexture(const std::string& Name,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetBackTextureFullPath(const std::string& Name,
        const std::vector<const TCHAR*>& vecFullPath);
    bool SetBackTextureArray(const std::string& Name,
        const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetBackTextureArrayFullPath(const std::string& Name,
        const std::vector<const TCHAR*>& vecFullPath);

    void SetTint(float r, float g, float b, float a);
    void SetTint(unsigned char r, unsigned char g,
        unsigned char b, unsigned char a);
    void SetTint(const Vector4& Color);

    void SetBackTint(float r, float g, float b, float a);
    void SetBackTint(unsigned char r, unsigned char g,
        unsigned char b, unsigned char a);
    void SetBackTint(const Vector4& Color);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CProgressBar* Clone();

public:
    virtual void CollisionMouse(const Vector2& MousePos, float DeltaTime);
    virtual void CollisionReleaseMouse(const Vector2& MousePos, float DeltaTime);
};

