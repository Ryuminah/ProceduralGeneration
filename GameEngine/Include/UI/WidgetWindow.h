#pragma once

#include "Widget.h"

class CWidgetWindow :
    public CWidget
{
    friend class CViewport;
    friend class CWidgetComponent;

protected:
    CWidgetWindow();
    CWidgetWindow(const CWidgetWindow& widget);
    virtual ~CWidgetWindow();

protected:
    std::vector<CSharedPtr<CWidget>>    m_vecWidget;
    class CWidgetComponent* m_OwnerComponent;

public:
    void SetWidgetComponent(class CWidgetComponent* WidgetComponent)
    {
        m_OwnerComponent = WidgetComponent;
    }

    class CWidgetComponent* GetWidgetComponent()    const
    {
        return m_OwnerComponent;
    }

    bool EmptyOwnerComponent()  const
    {
        return m_OwnerComponent == nullptr;
    }

public:
    virtual void SetZOrder(int ZOrder);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CWidgetWindow* Clone();
    virtual void CallAnimNotify(const std::string& Name);

public:
    template <typename T>
    T* FindWidget(const std::string& Name)
    {
        size_t  Size = m_vecWidget.size();

        for (size_t i = 0; i < Size; ++i)
        {
            if (m_vecWidget[i]->GetName() == Name)
                return (T*)m_vecWidget[i].Get();
        }

        return nullptr;
    }

    template <typename T>
    T* CreateWidget(const std::string& Name)
    {
        T* Widget = FindWidget<T>(Name);

        if (Widget)
            return nullptr;

        Widget = new T;

        Widget->m_Viewport = m_Viewport;
        Widget->m_Scene = m_Scene;
        Widget->m_Owner = this;

        if (!Widget->Init())
        {
            SAFE_DELETE(Widget);
            return nullptr;
        }

        m_vecWidget.push_back(Widget);
        Widget->Release();

        return Widget;
    }

private:
    //static int SortZOrder(const void* Src, const void* Dest);
    static bool SortZOrder(CSharedPtr<CWidget> Src, CSharedPtr<CWidget> Dest);
};

