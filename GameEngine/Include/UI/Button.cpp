
#include "Button.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Input.h"
#include "../Resource/UITransformConstantBuffer.h"

CButton::CButton() :
	m_State(Button_State::Normal),
	m_Down(false),
	m_Push(false),
	m_Up(false)
{
}

CButton::CButton(const CButton& widget)
{
	m_Down = false;
	m_Push = false;
	m_Up = false;

	m_State = widget.m_State;

	for (int i = 0; i < (int)Button_State::End; ++i)
	{
		m_ButtonState[i] = widget.m_ButtonState[i];
	}
}

CButton::~CButton()
{
}

bool CButton::Init()
{
	CWidget::Init();

	m_Shader = CShaderManager::GetInst()->FindShader("UIImageShader");

	return true;
}

void CButton::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_Down && !m_Push)
			m_Down = true;

		else if (!m_Push)
		{
			m_Down = false;
			m_Push = true;
		}
	}

	else if (m_Push || m_Down)
	{
		m_Up = true;
		m_Push = false;
		m_Down = false;
	}

	else if (m_Up)
		m_Up = false;

	if (m_State == Button_State::MouseOn)
	{
		if (m_Down)
			m_State = Button_State::Click;
	}

	else if (m_State == Button_State::Click)
	{
		if (m_Up)
		{
			// 클릭 사운드 재생.
			if (m_InteractionSound[(int)Button_Sound_State::Click])
				m_InteractionSound[(int)Button_Sound_State::Click]->Play();

			if (m_ClickCallback)
				m_ClickCallback();

			m_State = Button_State::MouseOn;
		}
	}
}

void CButton::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CButton::Render()
{
	m_TintCBuffer->SetTint(m_ButtonState[(int)m_State].Tint);

	if (m_ButtonState[(int)m_State].Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);

	CWidget::Render();

	if (m_ButtonState[(int)m_State].Texture)
		m_ButtonState[(int)m_State].Texture->SetShader(0, TST_PIXEL);

	m_Mesh->Render();

	if (m_ButtonState[(int)m_State].Texture)
		m_ButtonState[(int)m_State].Texture->ResetShader(0, TST_PIXEL);
}

CButton* CButton::Clone()
{
	return new CButton(*this);
}

void CButton::CollisionMouse(const Vector2& MousePos, float DeltaTime)
{
	CWidget::CollisionMouse(MousePos, DeltaTime);

	if (m_State != Button_State::Disabled)
	{
		m_State = Button_State::MouseOn;

		// MouseOn Sound Play
		if (m_InteractionSound[(int)Button_Sound_State::MouseOn])
			m_InteractionSound[(int)Button_Sound_State::MouseOn]->Play();
	}
}

void CButton::CollisionReleaseMouse(const Vector2& MousePos, float DeltaTime)
{
	CWidget::CollisionReleaseMouse(MousePos, DeltaTime);

	if (m_State != Button_State::Disabled)
		m_State = Button_State::Normal;
}

void CButton::SetStateTexture(Button_State State, CTexture* Texture)
{
	m_ButtonState[(int)State].Texture = Texture;
}

bool CButton::SetStateTexture(Button_State State, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return false;

	m_ButtonState[(int)State].Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CButton::SetStateTextureFullPath(Button_State State, 
	const std::string& Name, const TCHAR* FullPath)
{
	if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
		return false;

	m_ButtonState[(int)State].Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CButton::SetStateTexture(Button_State State, const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
		return false;

	m_ButtonState[(int)State].Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CButton::SetStateTextureFullPath(Button_State State, 
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	m_ButtonState[(int)State].Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CButton::SetStateTextureArray(Button_State State, 
	const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTextureArray(Name, vecFileName, PathName))
		return false;

	m_ButtonState[(int)State].Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

bool CButton::SetStateTextureArrayFullPath(Button_State State,	
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (!m_Scene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath))
		return false;

	m_ButtonState[(int)State].Texture =
		m_Scene->GetResource()->FindTexture(Name);

	return true;
}

void CButton::SetStateTint(Button_State State, float r, float g, 
	float b, float a)
{
	m_ButtonState[(int)State].Tint = Vector4(r, g, b, a);
}

void CButton::SetStateTint(Button_State State, unsigned char r, 
	unsigned char g, unsigned char b, unsigned char a)
{
	m_ButtonState[(int)State].Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CButton::SetStateTint(Button_State State, const Vector4& Color)
{
	m_ButtonState[(int)State].Tint = Color;
}

void CButton::SetInteractionSound(Button_Sound_State State, 
	const std::string& Name)
{
	m_InteractionSound[(int)State] = m_Scene->GetResource()->FindSound(Name);
}
