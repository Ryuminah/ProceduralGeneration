#pragma once

#include "Widget.h"
#include "../Resource/Texture.h"
#include "../Resource/Sound.h"

struct ButtonStateInfo
{
	CSharedPtr<CTexture>    Texture;
	Vector4         Tint;

	ButtonStateInfo()
	{
		Tint = Vector4::White;
	}
};

class CButton :
	public CWidget
{
	friend class CViewport;
	friend class CScene;
	friend class CWidgetWindow;

protected:
	CButton();
	CButton(const CButton& widget);
	virtual ~CButton();

protected:
	Button_State    m_State;
	ButtonStateInfo m_ButtonState[(int)Button_State::End];
	bool    m_Down;
	bool    m_Push;
	bool    m_Up;
	std::function<void()>	m_ClickCallback;
	CSharedPtr<CSound>	m_InteractionSound[(int)Button_Sound_State::End];

public:
	void EnableButton(bool Enable)
	{
		m_State = Enable ? Button_State::Normal : Button_State::Disabled;
	}

	void SetStateTexture(Button_State State, CTexture* Texture);
	bool SetStateTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetStateTextureFullPath(Button_State State, const std::string& Name, const TCHAR* FullPath);
	bool SetStateTexture(Button_State State, const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetStateTextureFullPath(Button_State State, const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);
	bool SetStateTextureArray(Button_State State, const std::string& Name,
		const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetStateTextureArrayFullPath(Button_State State, const std::string& Name,
		const std::vector<const TCHAR*>& vecFullPath);

	void SetStateTint(Button_State State, float r, float g, float b, float a);
	void SetStateTint(Button_State State, unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void SetStateTint(Button_State State, const Vector4& Color);

	void SetInteractionSound(Button_Sound_State State, const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CButton* Clone();

public:
	virtual void CollisionMouse(const Vector2& MousePos, float DeltaTime);
	virtual void CollisionReleaseMouse(const Vector2& MousePos, float DeltaTime);

public:
	template <typename T>
	void SetClickCallback(T* Obj, void(T::* Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}
};

