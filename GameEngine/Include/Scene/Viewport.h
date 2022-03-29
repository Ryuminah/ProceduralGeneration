#pragma once

#include "../GameEngine.h"
#include "../Component/Camera.h"
#include "../UI/WidgetWindow.h"

class CViewport
{
	friend class CScene;

private:
	CViewport();
	~CViewport();

private:
	class CScene* m_Scene;
	CSharedPtr<CCamera> m_Camera;
	std::vector<CSharedPtr<CWidgetWindow>>	m_vecWindow;

public:
	CCamera* GetCamera()	const
	{
		return m_Camera;
	}

	CWidgetWindow* FindWidgetWindow(const std::string& Name)
	{
		size_t	Size = m_vecWindow.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecWindow[i]->GetName() == Name)
				return m_vecWindow[i];
		}

		return nullptr;
	}

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render();


public:
	template <typename T>
	T* AddWindow(const std::string& Name)
	{
		T* Window = (T*)FindWidgetWindow(Name);

		if (Window)
			return Window;

		Window = new T;

		Window->m_Viewport = this;
		Window->m_Scene = m_Scene;

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_vecWindow.push_back(Window);
		Window->Release();

		return Window;
	}
};

