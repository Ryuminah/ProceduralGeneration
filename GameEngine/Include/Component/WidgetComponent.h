#pragma once

#include "PrimitiveComponent.h"
#include "../UI/WidgetWindow.h"
#include "../Scene/Scene.h"

class CWidgetComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;

protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& com);
    virtual ~CWidgetComponent();

protected:
    CSharedPtr<CWidgetWindow>     m_WidgetWindow;
    WidgetComponent_Space           m_Space;
    class CRenderState* m_AlphaBlend;

public:
    void SetWidgetSpace(WidgetComponent_Space Space)
    {
        m_Space = Space;
    }

    WidgetComponent_Space GetWidgetSpace()  const
    {
        return m_Space;
    }

    void SetWidget(CWidgetWindow* Widget)
    {
        m_WidgetWindow = Widget;

        m_WidgetWindow->SetWidgetComponent(this);
    }

    CWidgetWindow* GetWidget()  const
    {
        return m_WidgetWindow;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(float DeltaTime);
    virtual CWidgetComponent* Clone();

public:
    template <typename T>
    T* CreateWidget(const std::string& Name)
    {
        T* Window = new T;

        Window->m_Scene = m_pScene;

        if (m_pScene)
            Window->m_Viewport = m_pScene->GetViewport();

        if (!Window->Init())
        {
            SAFE_DELETE(Window);
            return nullptr;
        }

        Window->SetWidgetComponent(this);

        m_WidgetWindow = Window;
        Window->Release();

        return Window;
    }
};

