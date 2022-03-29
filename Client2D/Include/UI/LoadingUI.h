#pragma once

#include "UI\WidgetWindow.h"
#include "UI/ProgressBar.h"
#include "UI/Image.h"

class CLoadingUI :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CLoadingUI();
    CLoadingUI(const CLoadingUI& widget);
    virtual ~CLoadingUI();

private:
    CSharedPtr<CProgressBar>    m_LoadingBar;
    float   m_LoadingPercent;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CLoadingUI* Clone();
};

