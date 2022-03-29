
#include "Viewport.h"

CViewport::CViewport()	:
	m_Camera(nullptr)
{
}

CViewport::~CViewport()
{

}

void CViewport::Start()
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

bool CViewport::Init()
{
	m_Camera = new CCamera;

	m_Camera->SetName("UICamera");
	m_Camera->m_pScene = m_Scene;

	m_Camera->Init();
	m_Camera->SetCameraType(Camera_Type::CamUI);

	m_Camera->Release();

	return true;
}

void CViewport::Update(float DeltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Update(DeltaTime);
	}
}

void CViewport::PostUpdate(float DeltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PostUpdate(DeltaTime);
	}
}

void CViewport::Render()
{


	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}
}
