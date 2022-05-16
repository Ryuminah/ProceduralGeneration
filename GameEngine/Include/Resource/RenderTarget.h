#pragma once
#include "Texture.h"
class CRenderTarget :
	public CTexture
{
	friend class CResourceManager;

protected:
	CRenderTarget();
	~CRenderTarget();

protected:
	ID3D11RenderTargetView* m_TargetView;
	ID3D11Texture2D* m_TargetTex;
	ID3D11RenderTargetView* m_PrevTargetView;
	ID3D11DepthStencilView* m_PrevDepthView;
	IDXGISurface* m_Surface;
	float   m_ClearColor[4];
	bool	m_DebugRender;
	Matrix	m_matDebugWVP;

public:
	IDXGISurface* GetSurface()	const
	{
		return m_Surface;
	}

	ID3D11RenderTargetView* GetTargetView()	const
	{
		return m_TargetView;
	}

	void SetDebugRender(bool Render)
	{
		m_DebugRender = Render;
	}

public:
	bool CreateTarget(const std::string& Name, unsigned int Width,
		unsigned int Height, DXGI_FORMAT PixelFormat);
	void ClearTarget();
	void SetTarget(ID3D11DepthStencilView* DepthView);
	void ResetTarget();
};

