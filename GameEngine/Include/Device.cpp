#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()  :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pTargetView(nullptr),
	m_pDepthView(nullptr),
	m_RS{},
	m_hWnd(nullptr),
	m_ClearColor{},
	m_2DTarget(nullptr),
	m_2DTargetWorld(nullptr),
	m_2DFactory(nullptr),
	m_DefaultState(nullptr),
	m_OriginState(nullptr)
{
}

CDevice::~CDevice()
{
	//m_pContext->RSSetState(m_OriginState);

	//SAFE_RELEASE(m_OriginState);
	//SAFE_RELEASE(m_DefaultState);

	SAFE_RELEASE(m_2DTargetWorld);
	SAFE_RELEASE(m_2DTarget);
	SAFE_RELEASE(m_2DFactory);

	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pSwapChain);

	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

Vector2 CDevice::GetResolutionRatio() const
{
	return m_ResolutionRatio;
}

bool CDevice::CreateWorld2DTarget(IDXGISurface* Surface)
{
	D2D1_RENDER_TARGET_PROPERTIES	props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(Surface,
		props, &m_2DTargetWorld)))
		return false;

	return true;
}

bool CDevice::Init(HWND hWnd, int Width, int Height, bool ScreenMode)
{
	m_hWnd = hWnd;
	m_RS.Width = Width;
	m_RS.Height = Height;

	unsigned int    Flag = 0;

#ifdef _DEBUG

	Flag = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	Flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	DXGI_SWAP_CHAIN_DESC    SwapDesc = {};

	SwapDesc.BufferDesc.Width = Width;
	SwapDesc.BufferDesc.Height = Height;
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapDesc.BufferCount = 1;
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapDesc.OutputWindow = hWnd;
	SwapDesc.SampleDesc.Count = 1;
	SwapDesc.SampleDesc.Quality = 0;
	SwapDesc.Windowed = ScreenMode;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL   Flevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL   Flevel1 = D3D_FEATURE_LEVEL_11_0;

	// Device, Context, SwapChain을 생성한다.
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &Flevel, 1,
		D3D11_SDK_VERSION, &SwapDesc, &m_pSwapChain, &m_pDevice, &Flevel1, &m_pContext)))
		return false;

	// SwapChain이 가지고 있는 백버퍼를 얻어온다.
	// ID3D11Texture2D : DirectX11에서 지원되는 Texture 객체이다.
	// SwapChain이 Texture로 BackBuffer 정보를 가지고 있기 때문에 이 타입의 변수를 선언하여
	// 버퍼를 가지고온다.
	ID3D11Texture2D* pBackBuffer = nullptr;

	// 2번 인자에 들어가는 타입의 ID를 이용하여 SwapChain이 가지고 있는 해당 타입의 버퍼를 얻어온다.
	// 이렇게 얻어온 버퍼는 RefCount가 증가하게 된다.
	// 그러므로 모두 사용시에는 반드시 Release를 해주어야 한다.
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	// 위에서 얻어온 BackBuffer를 이용해서 RenderTargetView를 생성하고 BackBuffer를 이 RenderTargetView에
	// 묶어준다.
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

	SAFE_RELEASE(pBackBuffer);

	// 깊이버퍼를 만들어준다.
	D3D11_TEXTURE2D_DESC    DepthDesc = {};

	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.ArraySize = 1;

	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;

	ID3D11Texture2D* pDepthStencilTex = nullptr;

	m_pDevice->CreateTexture2D(&DepthDesc, nullptr, &pDepthStencilTex);

	m_pDevice->CreateDepthStencilView(pDepthStencilTex, nullptr, &m_pDepthView);

	SAFE_RELEASE(pDepthStencilTex);

	//m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);


	// 실제 출력되는 2D 뷰포트 영역을 만들어준다.
	D3D11_VIEWPORT  VP = {};

	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &VP);

	// 2D Factory를 만들어준다.
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
		return false;

	// 3D BackBuffer의 Surface을 얻어온다.
	IDXGISurface* BackSurface = nullptr;

	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

	// 2D용 렌더타겟을 만들어준다.
	// 단, 출력할 Surface는 위에서 얻어온 BackBuffer의 Surface로 지정하여 2D도
	// 출력이 가능하도록 만들어 주어야 한다.
	D2D1_RENDER_TARGET_PROPERTIES	props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(BackSurface, 
		props, &m_2DTarget)))
		return false;

	SAFE_RELEASE(BackSurface);

	/*D3D11_RASTERIZER_DESC	RASDesc = {};

	RASDesc.AntialiasedLineEnable = true;
	RASDesc.CullMode = D3D11_CULL_BACK;
	RASDesc.DepthBias = 0;
	RASDesc.DepthBiasClamp = 0.f;
	RASDesc.DepthClipEnable = true;
	RASDesc.FillMode = D3D11_FILL_SOLID;
	RASDesc.FrontCounterClockwise = false;
	RASDesc.MultisampleEnable = true;
	RASDesc.ScissorEnable = false;
	RASDesc.SlopeScaledDepthBias = 0.f;

	if (FAILED(m_pDevice->CreateRasterizerState(&RASDesc, &m_DefaultState)))
		return false;*/

	//m_pContext->RSGetState(&m_OriginState);
	//m_pContext->RSSetState(m_DefaultState);

	return true;
}

void CDevice::Update()
{
	RECT	rcClient = {};
	GetClientRect(m_hWnd, &rcClient);

	float	Width = (float)(rcClient.right - rcClient.left);
	float	Height = (float)(rcClient.bottom - rcClient.top);

	m_ResolutionRatio.x = m_RS.Width / Width;
	m_ResolutionRatio.y = m_RS.Height / Height;
}

void CDevice::ClearTarget()
{
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

	m_pContext->ClearRenderTargetView(m_pTargetView, m_ClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f, 0);
}

void CDevice::Render()
{
	m_pSwapChain->Present(0, 0);
}
