#pragma once

#include "Widget.h"

enum class TEXT_ALIGN_H
{
    Left,
    Center,
    Right
};

enum class TEXT_ALIGN_V
{
    Top,
    Middle,
    Bottom
};

class CText :
    public CWidget
{
    friend class CViewport;
    friend class CScene;
    friend class CWidgetWindow;

protected:
    CText();
    CText(const CText& widget);
    virtual ~CText();

private:
    IDWriteTextFormat* m_Font;
    IDWriteTextLayout* m_Layout;
    ID2D1SolidColorBrush* m_ColorBrush;
    ID2D1RenderTarget* m_2DTarget;
    TCHAR* m_Text;
    int m_MaxCount;
    std::string     m_FontKey;
    TCHAR* m_FontName;
    float   m_FontSize;
    bool    m_Alpha;
    float   m_Opacity;
    Vector4 m_Color;

    // Shadow
    bool        m_ShadowEnable;
    ID2D1SolidColorBrush* m_ShadowColorBrush; 
    bool    m_ShadowAlpha;
    float   m_ShadowOpacity;
    Vector4 m_ShadowColor;
    Vector2 m_ShadowOffset;

    TEXT_ALIGN_H    m_AlignH;
    TEXT_ALIGN_V    m_AlignV;

public:
    const TCHAR* GetText()  const
    {
        return m_Text;
    }

    int GetTextCount()  const
    {
        return lstrlen(m_Text);
    }

public:
    void SetMaxTextCount(int MaxCount);
    void SetText(const TCHAR* Text);
    void AddText(const TCHAR* Text);
    void SetFont(const std::string& Name);
    void SetFontSize(float FontSize);
    void SetAlignH(TEXT_ALIGN_H Align);
    void SetAlignV(TEXT_ALIGN_V Align);
    void SetColor(float r, float g, float b);
    void SetColor(unsigned char r, unsigned char g, unsigned char b);
    void SetColor(const Vector3& Color);
    void SetColor(unsigned int Color);
    void SetAlphaEnable(bool AlphaEnable);
    void SetOpacity(float Opacity);

    void SetShadowEnable(bool ShadowEnable);
    void SetShadowOffset(const Vector2& ShadowOffset);
    void SetShadowColor(float r, float g, float b);
    void SetShadowColor(unsigned char r, unsigned char g, unsigned char b);
    void SetShadowColor(const Vector3& Color);
    void SetShadowColor(unsigned int Color);
    void SetShadowAlphaEnable(bool AlphaEnable);
    void SetShadowOpacity(float Opacity);

private:
    void CreateTextLayout();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CText* Clone();
};

