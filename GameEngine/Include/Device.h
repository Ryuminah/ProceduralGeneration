#pragma once

#include "GameEngine.h"

class CDevice
{
private:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;
    IDXGISwapChain* m_pSwapChain;
    // SwapChain이 가지고 있는 BackBuffer를 RenderTargetView에 묶어주고 출력을 이 RenderTargetView에 해주게되면
    // 묶여있는 BackBuffer에 출력이 되게 된다.
    ID3D11RenderTargetView* m_pTargetView;
    ID3D11DepthStencilView* m_pDepthView;
    Resolution              m_RS;
    HWND                    m_hWnd;
    float                   m_ClearColor[4];
    Vector2                 m_ResolutionRatio;

private:
    ID2D1RenderTarget* m_2DTarget;
    ID2D1RenderTarget* m_2DTargetWorld;
    ID2D1Factory* m_2DFactory;
    ID3D11RasterizerState* m_DefaultState;
    ID3D11RasterizerState* m_OriginState;

public:
    ID2D1RenderTarget* Get2DTarget()   const
    {
        return m_2DTarget;
    }

    ID2D1RenderTarget* Get2DTargetWorld()   const
    {
        return m_2DTargetWorld;
    }

    ID2D1Factory* Get2DFactory()   const
    {
        return m_2DFactory;
    }

public:
    ID3D11Device* GetDevice()   const
    {
        return m_pDevice;
    }

    ID3D11DeviceContext* GetContext()   const
    {
        return m_pContext;
    }

    IDXGISwapChain* GetSwapChain()   const
    {
        return m_pSwapChain;
    }

    Resolution GetResolution()  const
    {
        return m_RS;
    }

    Vector2 GetResolutionRatio()    const;

public:
    bool CreateWorld2DTarget(IDXGISurface* Surface);

public:
    bool Init(HWND hWnd, int Width, int Height, bool ScreenMode = true);
    void Update();
    void ClearTarget();
    void Render();

    DECLARE_SINGLE(CDevice)
};

